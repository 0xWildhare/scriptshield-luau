#include <iostream>
#include <string>
#include <cstring>

// Luau includes
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "luacode.h"
}

// C wrapper functions for Rust FFI
extern "C" {
    struct LuauState {
        lua_State* L;
    };

    LuauState* luau_new_state() {
        LuauState* state = new LuauState;
        state->L = luaL_newstate();
        if (state->L) {
            luaL_openlibs(state->L);
        }
        return state;
    }

    void luau_close_state(LuauState* state) {
        if (state && state->L) {
            lua_close(state->L);
        }
        delete state;
    }

    int luau_run_simple_test(LuauState* state) {
        if (!state || !state->L) return -1;

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

        if (!bytecode) {
            std::cerr << "Compilation failed" << std::endl;
            return -1;
        }

        // Load and run the bytecode
        if (luau_load(state->L, "simple-test", bytecode, bytecode_size, 0) == 0) {
            if (lua_pcall(state->L, 0, 0, 0) != 0) {
                std::cerr << "Runtime error: " << lua_tostring(state->L, -1) << std::endl;
                free(bytecode);
                return -1;
            }
        } else {
            std::cerr << "Load error: " << lua_tostring(state->L, -1) << std::endl;
            free(bytecode);
            return -1;
        }

        free(bytecode);
        return 0;
    }

    // Mock parse_json function for Luau
    static int parse_json_mock(lua_State* L) {
        const char* json_str = luaL_checkstring(L, 1);

        // Check if this is our test GitHub data
        if (strstr(json_str, "\"login\":\"ospfranco\"")) {
            // Create a Lua table representing the parsed GitHub data
            lua_newtable(L);

            // Add basic fields
            lua_pushstring(L, "ospfranco");
            lua_setfield(L, -2, "login");

            lua_pushinteger(L, 1634213);
            lua_setfield(L, -2, "id");

            lua_pushstring(L, "MDQ6VXNlcjE2MzQyMTM=");
            lua_setfield(L, -2, "node_id");

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

    int luau_run_production_test(LuauState* state, const char* script_content) {
        if (!state || !state->L || !script_content) return -1;

        // Add mock parse_json function to global scope
        lua_pushcfunction(state->L, parse_json_mock, "parse_json");
        lua_setglobal(state->L, "parse_json");

        // Compile the script
        size_t bytecode_size = 0;
        lua_CompileOptions options = {};
        char* bytecode = luau_compile(script_content, strlen(script_content), &options, &bytecode_size);

        if (!bytecode) {
            std::cerr << "Script compilation failed" << std::endl;
            return -1;
        }

        // Load the bytecode
        if (luau_load(state->L, "production-script", bytecode, bytecode_size, 0) != 0) {
            std::cerr << "Script load failed: " << lua_tostring(state->L, -1) << std::endl;
            free(bytecode);
            return -1;
        }

        // Execute the script (this loads the mapper function)
        if (lua_pcall(state->L, 0, 1, 0) != 0) {
            std::cerr << "Script execution failed: " << lua_tostring(state->L, -1) << std::endl;
            free(bytecode);
            return -1;
        }

        // The script should return a mapper function
        if (lua_isfunction(state->L, -1)) {
            // Create test payload
            lua_newtable(state->L); // params table

            lua_newtable(state->L); // payload table
            const char* json_data = R"({"login":"ospfranco","id":1634213,"node_id":"MDQ6VXNlcjE2MzQyMTM=","name":"Oscar Franco","bio":"Freelance Dev","blog":"ospfranco.com","location":"Barcelona, Spain","twitter_username":"ospfranco","public_repos":47,"public_gists":14,"followers":533,"following":6,"created_at":"2012-04-11T19:00:30Z","updated_at":"2025-09-10T20:03:58Z","disk_usage":325487,"two_factor_authentication":true,"plan":{"name":"free","private_repos":10000}})";
            lua_pushstring(state->L, json_data);
            lua_setfield(state->L, -2, "default");
            lua_setfield(state->L, -2, "payload");

            // Call the mapper function
            if (lua_pcall(state->L, 1, 1, 0) == 0) {
                std::cout << "Mapper executed successfully!" << std::endl;
                std::cout << "Result type: " << lua_typename(state->L, lua_type(state->L, -1)) << std::endl;

                if (lua_istable(state->L, -1)) {
                    std::cout << "Processed data contains:" << std::endl;

                    // Check for some expected fields
                    lua_getfield(state->L, -1, "username");
                    if (lua_isstring(state->L, -1)) {
                        std::cout << "  username: " << lua_tostring(state->L, -1) << std::endl;
                    }
                    lua_pop(state->L, 1);

                    lua_getfield(state->L, -1, "display_name");
                    if (lua_isstring(state->L, -1)) {
                        std::cout << "  display_name: " << lua_tostring(state->L, -1) << std::endl;
                    }
                    lua_pop(state->L, 1);

                    lua_getfield(state->L, -1, "followers");
                    if (lua_isnumber(state->L, -1)) {
                        std::cout << "  followers: " << lua_tonumber(state->L, -1) << std::endl;
                    }
                    lua_pop(state->L, 1);

                    lua_getfield(state->L, -1, "public_repos");
                    if (lua_isnumber(state->L, -1)) {
                        std::cout << "  public_repos: " << lua_tonumber(state->L, -1) << std::endl;
                    }
                    lua_pop(state->L, 1);

                    // Verify that node_id is NOT in the output (proving processing occurred)
                    lua_getfield(state->L, -1, "node_id");
                    if (lua_isnil(state->L, -1)) {
                        std::cout << "✅ VERIFICATION: node_id correctly filtered out (not in output)" << std::endl;
                    } else {
                        std::cout << "❌ VERIFICATION FAILED: node_id found in output: " << lua_tostring(state->L, -1) << std::endl;
                    }
                    lua_pop(state->L, 1);
                }

                free(bytecode);
                return 0;
            } else {
                std::cerr << "Mapper call failed: " << lua_tostring(state->L, -1) << std::endl;
            }
        } else {
            std::cerr << "Script did not return a function" << std::endl;
        }

        free(bytecode);
        return -1;
    }
}