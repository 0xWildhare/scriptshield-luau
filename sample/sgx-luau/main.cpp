#include <iostream>
#include <string>
#include <cstring>

// Luau includes
#include "lua.h"
#include "lualib.h"
#include "luacode.h"

void run_luau_script(const char* script) {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // Compile the script
    size_t bytecode_size = 0;
    lua_CompileOptions options = {};
    char* bytecode = luau_compile(script, strlen(script), &options, &bytecode_size);

    if (bytecode) {
        // Load and run the bytecode
        if (luau_load(L, "script", bytecode, bytecode_size, 0) == 0) {
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

    // Test script
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

    run_luau_script(test_script);

    return 0;
}