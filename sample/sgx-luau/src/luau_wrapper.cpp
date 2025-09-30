#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cmath>

// Luau includes
#include "lua.h"
#include "lualib.h"
#include "luacode.h"

// Just copy the exact working C++ code
std::string read_file(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Forward declarations
static void skip_whitespace(const char*& json);
static void parse_json_value(lua_State* L, const char*& json, int depth = 0);
static void parse_json_object(lua_State* L, const char*& json, int depth);
static void parse_json_array(lua_State* L, const char*& json, int depth);
static void parse_json_string(lua_State* L, const char*& json);
static void parse_json_number(lua_State* L, const char*& json);

// Maximum recursion depth to prevent stack overflow
static const int MAX_JSON_DEPTH = 100;

// Helper function to skip whitespace
static void skip_whitespace(const char*& json) {
    while (*json && (*json == ' ' || *json == '\t' || *json == '\n' || *json == '\r')) {
        json++;
    }
}

// Parse a JSON string (assumes json points to opening quote)
static void parse_json_string(lua_State* L, const char*& json) {
    if (*json != '"') {
        lua_pushnil(L);
        return;
    }

    json++; // Skip opening quote
    std::string result;

    while (*json && *json != '"') {
        if (*json == '\\') {
            json++; // Skip backslash
            if (!*json) {
                // Unexpected end of string
                lua_pushnil(L);
                return;
            }
            switch (*json) {
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                case '/': result += '/'; break;
                case 'b': result += '\b'; break;
                case 'f': result += '\f'; break;
                case 'n': result += '\n'; break;
                case 'r': result += '\r'; break;
                case 't': result += '\t'; break;
                case 'u': {
                    // Unicode escape - need 4 hex digits
                    json++;
                    if (strlen(json) < 4) {
                        lua_pushnil(L);
                        return;
                    }
                    // For simplicity, just include the \u and 4 hex digits literally
                    result += "\\u";
                    for (int i = 0; i < 4; i++) {
                        if (!isxdigit(json[i])) {
                            lua_pushnil(L);
                            return;
                        }
                        result += json[i];
                    }
                    json += 3; // Will be incremented by 1 at end of loop
                    break;
                }
                default:
                    // Invalid escape sequence
                    result += *json;
                    break;
            }
        } else if (*json < 0x20) {
            // Control characters must be escaped in JSON
            lua_pushnil(L);
            return;
        } else {
            result += *json;
        }
        json++;
    }

    if (*json != '"') {
        // Missing closing quote
        lua_pushnil(L);
        return;
    }

    json++; // Skip closing quote
    lua_pushstring(L, result.c_str());
}

// Parse a JSON number
static void parse_json_number(lua_State* L, const char*& json) {
    const char* start = json;

    if (*json == '-') json++;

    // Must have at least one digit
    if (!(*json >= '0' && *json <= '9')) {
        lua_pushnil(L);
        return;
    }

    // Handle zero specially - if first digit is 0, next char must be . or e/E or end
    if (*json == '0') {
        json++;
        if (*json >= '0' && *json <= '9') {
            // Invalid: leading zeros not allowed
            lua_pushnil(L);
            return;
        }
    } else {
        while (*json && (*json >= '0' && *json <= '9')) json++;
    }

    if (*json == '.') {
        json++;
        const char* frac_start = json;
        while (*json && (*json >= '0' && *json <= '9')) json++;
        // Must have at least one digit after decimal point
        if (json == frac_start) {
            lua_pushnil(L);
            return;
        }
    }

    if (*json == 'e' || *json == 'E') {
        json++;
        if (*json == '+' || *json == '-') json++;
        const char* exp_start = json;
        while (*json && (*json >= '0' && *json <= '9')) json++;
        // Must have at least one digit in exponent
        if (json == exp_start) {
            lua_pushnil(L);
            return;
        }
    }

    std::string num_str(start, json - start);

    try {
        double value = std::stod(num_str);
        // Check for overflow/underflow
        if (std::isfinite(value)) {
            lua_pushnumber(L, value);
        } else {
            lua_pushnil(L);
        }
    } catch (...) {
        lua_pushnil(L);
    }
}

// Parse a JSON array
static void parse_json_array(lua_State* L, const char*& json, int depth) {
    if (depth > MAX_JSON_DEPTH) {
        lua_pushnil(L);
        return;
    }

    if (*json != '[') {
        lua_pushnil(L);
        return;
    }

    json++; // Skip '['
    lua_newtable(L);

    skip_whitespace(json);

    if (*json == ']') {
        json++;
        return;
    }

    int index = 1;
    while (true) {
        skip_whitespace(json);
        if (!*json) break; // Unexpected end

        parse_json_value(L, json, depth + 1);
        lua_rawseti(L, -2, index++);

        skip_whitespace(json);
        if (*json == ',') {
            json++;
            continue;
        } else if (*json == ']') {
            json++;
            break;
        } else {
            // Malformed JSON
            break;
        }
    }
}

// Parse a JSON object
static void parse_json_object(lua_State* L, const char*& json, int depth) {
    if (depth > MAX_JSON_DEPTH) {
        lua_pushnil(L);
        return;
    }

    if (*json != '{') {
        lua_pushnil(L);
        return;
    }

    json++; // Skip '{'
    lua_newtable(L);

    skip_whitespace(json);

    if (*json == '}') {
        json++;
        return;
    }

    while (true) {
        skip_whitespace(json);
        if (!*json) break; // Unexpected end

        // Parse key (must be string)
        if (*json != '"') break;
        parse_json_string(L, json);

        skip_whitespace(json);
        if (*json != ':') break;
        json++; // Skip ':'

        skip_whitespace(json);
        parse_json_value(L, json, depth + 1);

        // Set table[key] = value
        lua_settable(L, -3);

        skip_whitespace(json);
        if (*json == ',') {
            json++;
            continue;
        } else if (*json == '}') {
            json++;
            break;
        } else {
            // Malformed JSON
            break;
        }
    }
}

// Parse any JSON value
static void parse_json_value(lua_State* L, const char*& json, int depth) {
    skip_whitespace(json);

    if (!*json) {
        lua_pushnil(L);
        return;
    }

    if (*json == '"') {
        parse_json_string(L, json);
    } else if (*json == '{') {
        parse_json_object(L, json, depth);
    } else if (*json == '[') {
        parse_json_array(L, json, depth);
    } else if (*json == 't') {
        if (strncmp(json, "true", 4) == 0) {
            json += 4;
            lua_pushboolean(L, 1);
        } else {
            lua_pushnil(L);
        }
    } else if (*json == 'f') {
        if (strncmp(json, "false", 5) == 0) {
            json += 5;
            lua_pushboolean(L, 0);
        } else {
            lua_pushnil(L);
        }
    } else if (*json == 'n') {
        if (strncmp(json, "null", 4) == 0) {
            json += 4;
            lua_pushnil(L);
        } else {
            lua_pushnil(L);
        }
    } else if (*json == '-' || (*json >= '0' && *json <= '9')) {
        parse_json_number(L, json);
    } else {
        lua_pushnil(L);
    }
}

// Main parse_json function - converts JSON string to Lua table
static int parse_json(lua_State* L) {
    const char* json_str = luaL_checkstring(L, 1);
    if (!json_str) {
        lua_pushnil(L);
        return 1;
    }

    const char* json = json_str;
    parse_json_value(L, json, 0);
    return 1;
}

// Forward declaration for recursion
std::string lua_table_to_json_helper(lua_State* L, int index, int depth);

// Generic function to convert a Lua table to JSON string
// This recursively handles any table structure without hardcoded fields
std::string lua_table_to_json(lua_State* L, int index) {
    return lua_table_to_json_helper(L, index, 0);
}

// Helper function with recursion depth protection
std::string lua_table_to_json_helper(lua_State* L, int index, int depth) {
    // Prevent infinite recursion
    if (depth > 20) {
        return "{}";
    }

    // Convert negative index to absolute index to handle stack changes
    if (index < 0) {
        index = lua_gettop(L) + index + 1;
    }

    // Check if this is an array (all keys are consecutive integers starting from 1)
    bool is_array = true;
    int array_size = 0;

    lua_pushnil(L);
    while (lua_next(L, index) != 0) {
        if (!lua_isnumber(L, -2) || lua_tonumber(L, -2) != array_size + 1) {
            is_array = false;
            lua_pop(L, 2); // Pop key and value
            break;
        }
        array_size++;
        lua_pop(L, 1); // Pop value, keep key for next iteration
    }

    std::string result;
    bool first = true;

    if (is_array && array_size > 0) {
        result = "[";
        for (int i = 1; i <= array_size; i++) {
            if (!first) result += ",";
            first = false;

            lua_rawgeti(L, index, i);

            if (lua_isstring(L, -1)) {
                std::string value = lua_tostring(L, -1);
                result += "\"";
                for (char c : value) {
                    if (c == '"') result += "\\\"";
                    else if (c == '\\') result += "\\\\";
                    else if (c == '\n') result += "\\n";
                    else if (c == '\r') result += "\\r";
                    else if (c == '\t') result += "\\t";
                    else result += c;
                }
                result += "\"";
            } else if (lua_isnumber(L, -1)) {
                double num = lua_tonumber(L, -1);
                if (num == (long long)num) {
                    result += std::to_string((long long)num);
                } else {
                    result += std::to_string(num);
                }
            } else if (lua_isboolean(L, -1)) {
                result += lua_toboolean(L, -1) ? "true" : "false";
            } else if (lua_istable(L, -1)) {
                result += lua_table_to_json_helper(L, -1, depth + 1);
            } else if (lua_isnil(L, -1)) {
                result += "null";
            } else {
                result += "null";
            }

            lua_pop(L, 1);
        }
        result += "]";
    } else {
        result = "{";
        lua_pushnil(L);
        while (lua_next(L, index) != 0) {
            if (!first) result += ",";
            first = false;

            // Get key
            std::string key;
            if (lua_isstring(L, -2)) {
                key = lua_tostring(L, -2);
            } else if (lua_isnumber(L, -2)) {
                key = std::to_string((long long)lua_tonumber(L, -2));
            } else {
                key = "unknown";
            }

            result += "\"" + key + "\":";

            // Get value
            if (lua_isstring(L, -1)) {
                std::string value = lua_tostring(L, -1);
                result += "\"";
                for (char c : value) {
                    if (c == '"') result += "\\\"";
                    else if (c == '\\') result += "\\\\";
                    else if (c == '\n') result += "\\n";
                    else if (c == '\r') result += "\\r";
                    else if (c == '\t') result += "\\t";
                    else result += c;
                }
                result += "\"";
            } else if (lua_isnumber(L, -1)) {
                double num = lua_tonumber(L, -1);
                if (num == (long long)num) {
                    result += std::to_string((long long)num);
                } else {
                    result += std::to_string(num);
                }
            } else if (lua_isboolean(L, -1)) {
                result += lua_toboolean(L, -1) ? "true" : "false";
            } else if (lua_istable(L, -1)) {
                result += lua_table_to_json_helper(L, -1, depth + 1);
            } else if (lua_isnil(L, -1)) {
                result += "null";
            } else {
                result += "null";
            }

            lua_pop(L, 1);
        }
        result += "}";
    }

    return result;
}

void run_simple_json_test() {
    std::cout << "\n=== Testing Simple JSON Parser ===" << std::endl;

    lua_State* L = luaL_newstate();
    if (!L) {
        std::cerr << "Failed to create Lua state" << std::endl;
        return;
    }

    luaL_openlibs(L);

    // Add parse_json function to global scope
    lua_pushcfunction(L, parse_json, "parse_json");
    lua_setglobal(L, "parse_json");

    // Test simple JSON parsing
    const char* test_json = R"({"name":"test","value":42,"nested":{"flag":true}})";

    std::cout << "Testing JSON: " << test_json << std::endl;

    lua_getglobal(L, "parse_json");
    lua_pushstring(L, test_json);

    if (lua_pcall(L, 1, 1, 0) == 0) {
        std::cout << "✅ JSON parsing succeeded" << std::endl;

        if (lua_istable(L, -1)) {
            std::cout << "✅ Result is a table" << std::endl;

            // Test conversion back to JSON
            std::string json_result = lua_table_to_json(L, -1);
            std::cout << "✅ Table to JSON conversion: " << json_result << std::endl;
        } else {
            std::cout << "❌ Result is not a table, type: " << lua_typename(L, lua_type(L, -1)) << std::endl;
        }
    } else {
        std::cout << "❌ JSON parsing failed: " << lua_tostring(L, -1) << std::endl;
    }

    lua_close(L);
}

void run_production_luau_test() {
    std::cout << "\n=== Testing Production GitHub Mapper ===" << std::endl;

    lua_State* L = luaL_newstate();
    if (!L) {
        std::cerr << "Failed to create Lua state" << std::endl;
        return;
    }

    luaL_openlibs(L);

    // Add parse_json function to global scope
    lua_pushcfunction(L, parse_json, "parse_json");
    lua_setglobal(L, "parse_json");

    // Load the production script - try multiple paths
    const char* script_paths[] = {
        "/production-script.lua",
        "/scriptshield-luau/production-script.lua",
        "production-script.lua",
        nullptr
    };

    std::string script_content;
    for (int i = 0; script_paths[i] != nullptr; i++) {
        std::cout << "Trying to load script from: " << script_paths[i] << std::endl;
        script_content = read_file(script_paths[i]);
        if (!script_content.empty()) {
            std::cout << "Successfully loaded production script from: " << script_paths[i] << std::endl;
            std::cout << "Script size: " << script_content.length() << " bytes" << std::endl;
            break;
        } else {
            std::cout << "  File not found or empty" << std::endl;
        }
    }

    if (script_content.empty()) {
        std::cerr << "Failed to load production script from any location" << std::endl;
        lua_close(L);
        return;
    }

    // Compile the script
    size_t bytecode_size = 0;
    lua_CompileOptions options = {};
    char* bytecode = luau_compile(script_content.c_str(), script_content.length(), &options, &bytecode_size);

    if (bytecode) {
        // Load the bytecode
        if (luau_load(L, "production-script", bytecode, bytecode_size, 0) == 0) {
            // Execute the script (this loads the mapper function)
            if (lua_pcall(L, 0, 1, 0) == 0) {
                // The script should return a mapper function
                if (lua_isfunction(L, -1)) {
                    // Create test payload
                    lua_newtable(L); // params table

                    lua_newtable(L); // payload table
                    lua_pushstring(L, R"({"login":"ospfranco","id":1634213,"node_id":"MDQ6VXNlcjE2MzQyMTM=","name":"Oscar Franco","bio":"Freelance Dev","blog":"ospfranco.com","location":"Barcelona, Spain","twitter_username":"ospfranco","public_repos":47,"public_gists":14,"followers":533,"following":6,"created_at":"2012-04-11T19:00:30Z","updated_at":"2025-09-10T20:03:58Z","disk_usage":325487,"two_factor_authentication":true,"plan":{"name":"free","private_repos":10000}})");
                    lua_setfield(L, -2, "default");
                    lua_setfield(L, -2, "payload");

                    // Call the mapper function
                    if (lua_pcall(L, 1, 1, 0) == 0) {
                        // Print the result
                        std::cout << "Mapper executed successfully!" << std::endl;
                        std::cout << "Result type: " << lua_typename(L, lua_type(L, -1)) << std::endl;

                        if (lua_istable(L, -1)) {
                            std::cout << "Processed data contains:" << std::endl;

                            // Check for some expected fields
                            lua_getfield(L, -1, "username");
                            if (lua_isstring(L, -1)) {
                                std::cout << "  username: " << lua_tostring(L, -1) << std::endl;
                            }
                            lua_pop(L, 1);

                            lua_getfield(L, -1, "display_name");
                            if (lua_isstring(L, -1)) {
                                std::cout << "  display_name: " << lua_tostring(L, -1) << std::endl;
                            }
                            lua_pop(L, 1);

                            lua_getfield(L, -1, "followers");
                            if (lua_isnumber(L, -1)) {
                                std::cout << "  followers: " << lua_tonumber(L, -1) << std::endl;
                            }
                            lua_pop(L, 1);

                            lua_getfield(L, -1, "public_repos");
                            if (lua_isnumber(L, -1)) {
                                std::cout << "  public_repos: " << lua_tonumber(L, -1) << std::endl;
                            }
                            lua_pop(L, 1);

                            // Verify that node_id is NOT in the output (proving processing occurred)
                            lua_getfield(L, -1, "node_id");
                            if (lua_isnil(L, -1)) {
                                std::cout << "✅ VERIFICATION: node_id correctly filtered out (not in output)" << std::endl;
                            } else {
                                std::cout << "❌ VERIFICATION FAILED: node_id found in output: " << lua_tostring(L, -1) << std::endl;
                            }
                            lua_pop(L, 1);
                        }
                    } else {
                        std::cerr << "Mapper call failed: " << lua_tostring(L, -1) << std::endl;
                    }
                } else {
                    std::cerr << "Script did not return a function" << std::endl;
                }
            } else {
                std::cerr << "Script execution failed: " << lua_tostring(L, -1) << std::endl;
            }
        } else {
            std::cerr << "Script load failed: " << lua_tostring(L, -1) << std::endl;
        }

        free(bytecode);
    } else {
        std::cerr << "Script compilation failed" << std::endl;
    }

    lua_close(L);
}

void run_simple_test() {
    std::cout << "=== Simple Test ===" << std::endl;

    lua_State* L = luaL_newstate();
    if (!L) {
        std::cerr << "Failed to create Lua state" << std::endl;
        return;
    }

    luaL_openlibs(L);

    const char* test_script = R"(
        print("Hello from Luau in SGX!")
        print("2 + 2 =", 2 + 2)

        local function factorial(n)
            if n <= 1 then
                return 1
            else
                return n * factorial(n - 1)
            end
        end

        print("5! =", factorial(5))
    )";

    // Compile the script
    size_t bytecode_size = 0;
    lua_CompileOptions options = {};
    char* bytecode = luau_compile(test_script, strlen(test_script), &options, &bytecode_size);

    if (bytecode) {
        // Load and run the bytecode
        if (luau_load(L, "simple-test", bytecode, bytecode_size, 0) == 0) {
            if (lua_pcall(L, 0, 0, 0) != 0) {
                std::cerr << "Runtime error: " << lua_tostring(L, -1) << std::endl;
            }
        } else {
            std::cerr << "Load error: " << lua_tostring(L, -1) << std::endl;
        }

        free(bytecode);
    } else {
        std::cerr << "Compilation failed" << std::endl;
    }

    lua_close(L);
}

// C wrapper functions for Rust FFI
extern "C" {
    // Structure to hold the result of Luau execution
    struct LuauResult {
        bool success;
        char* result_json;
        char* error_message;
    };

    // Create a new Luau environment
    lua_State* create_luau_env() {
        lua_State* L = luaL_newstate();
        if (!L) {
            return nullptr;
        }

        luaL_openlibs(L);

        // Add generic parse_json function
        lua_pushcfunction(L, parse_json, "parse_json");
        lua_setglobal(L, "parse_json");

        return L;
    }

    // Close a Luau environment
    void close_luau_env(lua_State* L) {
        if (L) {
            lua_close(L);
        }
    }

    // Execute a mapper with given parameters
    LuauResult* execute_mapper(lua_State* L, const char* mapper_code, const char* params_json) {
        LuauResult* result = new LuauResult{false, nullptr, nullptr};

        if (!L || !mapper_code || !params_json) {
            result->error_message = strdup("Invalid parameters");
            return result;
        }

        // Compile the mapper code
        size_t bytecode_size = 0;
        lua_CompileOptions options = {};
        char* bytecode = luau_compile(mapper_code, strlen(mapper_code), &options, &bytecode_size);

        if (!bytecode) {
            result->error_message = strdup("Failed to compile mapper code");
            return result;
        }

        // Load the bytecode
        if (luau_load(L, "mapper", bytecode, bytecode_size, 0) != 0) {
            result->error_message = strdup(lua_tostring(L, -1));
            free(bytecode);
            return result;
        }

        // Execute the script to get the mapper function
        if (lua_pcall(L, 0, 1, 0) != 0) {
            result->error_message = strdup(lua_tostring(L, -1));
            free(bytecode);
            return result;
        }

        // Check if the result is a function
        if (!lua_isfunction(L, -1)) {
            result->error_message = strdup("Mapper code did not return a function");
            free(bytecode);
            return result;
        }

        // TODO: Parse the JSON parameters properly
        // For now, we'll use the hardcoded test data

        // For now, create a simple test payload structure
        // In real implementation, this would parse the actual params_json
        lua_newtable(L); // params table

        lua_newtable(L); // payload table
        lua_pushstring(L, R"({"login":"ospfranco","id":1634213,"node_id":"MDQ6VXNlcjE2MzQyMTM=","name":"Oscar Franco","bio":"Freelance Dev","blog":"ospfranco.com","location":"Barcelona, Spain","twitter_username":"ospfranco","public_repos":47,"public_gists":14,"followers":533,"following":6,"created_at":"2012-04-11T19:00:30Z","updated_at":"2025-09-10T20:03:58Z","disk_usage":325487,"two_factor_authentication":true,"plan":{"name":"free","private_repos":10000}})");
        lua_setfield(L, -2, "default");
        lua_setfield(L, -2, "payload");

        // Call the mapper function
        if (lua_pcall(L, 1, 1, 0) != 0) {
            result->error_message = strdup(lua_tostring(L, -1));
            free(bytecode);
            return result;
        }

        // Convert the result to JSON using generic converter
        if (lua_istable(L, -1)) {
            std::string json_result = lua_table_to_json(L, -1);
            result->result_json = strdup(json_result.c_str());
            result->success = true;
        } else if (lua_isstring(L, -1)) {
            // If the script returns a string, use it directly
            result->result_json = strdup(lua_tostring(L, -1));
            result->success = true;
        } else if (lua_isnumber(L, -1)) {
            // If the script returns a number, convert to string
            std::string num_result = std::to_string(lua_tonumber(L, -1));
            result->result_json = strdup(num_result.c_str());
            result->success = true;
        } else if (lua_isboolean(L, -1)) {
            // If the script returns a boolean, convert to string
            result->result_json = strdup(lua_toboolean(L, -1) ? "true" : "false");
            result->success = true;
        } else {
            result->error_message = strdup("Mapper returned an unsupported type");
        }

        free(bytecode);
        return result;
    }

    // Execute a mapper with already parsed payload data (generic version)
    LuauResult* execute_mapper_with_parsed_data(lua_State* L, const char* mapper_code, const char* payload_json) {
        LuauResult* result = new LuauResult{false, nullptr, nullptr};

        if (!L || !mapper_code || !payload_json) {
            result->error_message = strdup("Invalid parameters");
            return result;
        }

        // Compile the mapper code
        size_t bytecode_size = 0;
        lua_CompileOptions options = {};
        char* bytecode = luau_compile(mapper_code, strlen(mapper_code), &options, &bytecode_size);

        if (!bytecode) {
            result->error_message = strdup("Failed to compile mapper code");
            return result;
        }

        // Load the bytecode
        if (luau_load(L, "mapper", bytecode, bytecode_size, 0) != 0) {
            result->error_message = strdup(lua_tostring(L, -1));
            free(bytecode);
            return result;
        }

        // Execute the script to get the mapper function
        if (lua_pcall(L, 0, 1, 0) != 0) {
            result->error_message = strdup(lua_tostring(L, -1));
            free(bytecode);
            return result;
        }

        // Check if the result is a function
        if (!lua_isfunction(L, -1)) {
            result->error_message = strdup("Mapper code did not return a function");
            free(bytecode);
            return result;
        }

        // Create params table with the parsed payload
        lua_newtable(L); // params table

        lua_newtable(L); // payload table
        lua_pushstring(L, payload_json);  // Use the already-parsed payload directly
        lua_setfield(L, -2, "default");
        lua_setfield(L, -2, "payload");

        // Call the mapper function
        if (lua_pcall(L, 1, 1, 0) != 0) {
            result->error_message = strdup(lua_tostring(L, -1));
            free(bytecode);
            return result;
        }

        // Convert the result using the same generic converter
        if (lua_istable(L, -1)) {
            std::string json_result = lua_table_to_json(L, -1);
            result->result_json = strdup(json_result.c_str());
            result->success = true;
        } else if (lua_isstring(L, -1)) {
            result->result_json = strdup(lua_tostring(L, -1));
            result->success = true;
        } else if (lua_isnumber(L, -1)) {
            std::string num_result = std::to_string(lua_tonumber(L, -1));
            result->result_json = strdup(num_result.c_str());
            result->success = true;
        } else if (lua_isboolean(L, -1)) {
            result->result_json = strdup(lua_toboolean(L, -1) ? "true" : "false");
            result->success = true;
        } else {
            result->error_message = strdup("Mapper returned an unsupported type");
        }

        free(bytecode);
        return result;
    }

    // Free a LuauResult
    void free_luau_result(LuauResult* result) {
        if (result) {
            if (result->result_json) {
                free(result->result_json);
            }
            if (result->error_message) {
                free(result->error_message);
            }
            delete result;
        }
    }

    int run_all_tests() {
        std::cout << "ScriptShield-Luau SGX Starting..." << std::endl;

        // Run simple test first
        run_simple_test();

        // Run simple JSON parser test
        run_simple_json_test();

        // Run production test
        run_production_luau_test();

        return 0;
    }
}