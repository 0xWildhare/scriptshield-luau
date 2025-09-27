#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>

// Luau includes
#include "lua.h"
#include "lualib.h"
#include "luacode.h"

std::string read_file(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Mock parse_json function for Luau
static int parse_json_mock(lua_State* L) {
    const char* json_str = luaL_checkstring(L, 1);

    // For this test, we'll create a simple mock that recognizes our test GitHub data
    // In production, this would be a proper JSON parser
    if (strstr(json_str, "\"login\":\"ospfranco\"")) {
        // Create a Lua table representing the parsed GitHub data
        lua_newtable(L);

        // Add basic fields
        lua_pushstring(L, "ospfranco");
        lua_setfield(L, -2, "login");

        lua_pushinteger(L, 1634213);
        lua_setfield(L, -2, "id");

        lua_pushstring(L, "Oscar Franco");
        lua_setfield(L, -2, "name");

        lua_pushstring(L, "Freelance Dev");
        lua_setfield(L, -2, "bio");

        lua_pushstring(L, "ospfranco.com");
        lua_setfield(L, -2, "blog");

        lua_pushstring(L, "Barcelona, Spain");
        lua_setfield(L, -2, "location");

        lua_pushstring(L, "ospfranco");
        lua_setfield(L, -2, "twitter_username");

        lua_pushinteger(L, 47);
        lua_setfield(L, -2, "public_repos");

        lua_pushinteger(L, 14);
        lua_setfield(L, -2, "public_gists");

        lua_pushinteger(L, 533);
        lua_setfield(L, -2, "followers");

        lua_pushinteger(L, 6);
        lua_setfield(L, -2, "following");

        lua_pushstring(L, "2012-04-11T19:00:30Z");
        lua_setfield(L, -2, "created_at");

        lua_pushstring(L, "2025-09-10T20:03:58Z");
        lua_setfield(L, -2, "updated_at");

        lua_pushinteger(L, 325487);
        lua_setfield(L, -2, "disk_usage");

        lua_pushboolean(L, 1);
        lua_setfield(L, -2, "two_factor_authentication");

        // Add plan table
        lua_newtable(L);
        lua_pushstring(L, "free");
        lua_setfield(L, -2, "name");
        lua_pushinteger(L, 10000);
        lua_setfield(L, -2, "private_repos");
        lua_setfield(L, -2, "plan");

        return 1;
    }

    lua_pushnil(L);
    return 1;
}

void run_production_luau_test() {
    std::cout << "\n=== Testing Production GitHub Mapper ===" << std::endl;

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // Add mock parse_json function to global scope
    lua_pushcfunction(L, parse_json_mock);
    lua_setglobal(L, "parse_json");

    // Load the production script
    std::string script_content = read_file("/production-script.lua");
    if (script_content.empty()) {
        std::cerr << "Failed to load production script" << std::endl;
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
                    lua_pushstring(L, R"({"login":"ospfranco","id":1634213,"name":"Oscar Franco","bio":"Freelance Dev","blog":"ospfranco.com","location":"Barcelona, Spain","twitter_username":"ospfranco","public_repos":47,"public_gists":14,"followers":533,"following":6,"created_at":"2012-04-11T19:00:30Z","updated_at":"2025-09-10T20:03:58Z","disk_usage":325487,"two_factor_authentication":true,"plan":{"name":"free","private_repos":10000}})");
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

int main() {
    std::cout << "ScriptShield-Luau SGX Starting..." << std::endl;

    // Run simple test first
    run_simple_test();

    // Run production test
    run_production_luau_test();

    return 0;
}