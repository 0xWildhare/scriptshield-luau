#include <iostream>
#include <string>
#include <cstring>

// Test includes
#include "luau-src/VM/include/lua.h"
#include "luau-src/VM/include/lualib.h"
#include "luau-src/Compiler/include/luacode.h"

int main() {
    std::cout << "Testing Luau includes and basic functionality..." << std::endl;

    // Simple test script
    const char* test_script = R"(
        print("Hello from Luau!")
        local x = 42
        print("Answer:", x)
    )";

    // Set up compilation options
    lua_CompileOptions options = {};
    options.optimizationLevel = 1;
    options.debugLevel = 1;
    options.coverageLevel = 0;

    // Compile script to bytecode
    size_t bytecode_size;
    char* bytecode = luau_compile(test_script, strlen(test_script), &options, &bytecode_size);

    if (!bytecode) {
        std::cout << "Error: Failed to compile Luau script" << std::endl;
        return 1;
    }

    std::cout << "Script compiled successfully to " << bytecode_size << " bytes" << std::endl;

    // Create VM state
    lua_State* L = luaL_newstate();
    if (!L) {
        std::cout << "Error: Failed to create Luau state" << std::endl;
        free(bytecode);
        return 1;
    }

    // Load standard libraries
    luaL_openlibs(L);

    // Load bytecode
    if (luau_load(L, "test", bytecode, bytecode_size, 0) != LUA_OK) {
        std::cout << "Error loading bytecode: " << lua_tostring(L, -1) << std::endl;
        free(bytecode);
        lua_close(L);
        return 1;
    }

    // Execute script
    int result = lua_pcall(L, 0, 0, 0);
    if (result != LUA_OK) {
        std::cout << "Script execution error: " << lua_tostring(L, -1) << std::endl;
    } else {
        std::cout << "Script executed successfully!" << std::endl;
    }

    // Cleanup
    free(bytecode);
    lua_close(L);

    return 0;
}