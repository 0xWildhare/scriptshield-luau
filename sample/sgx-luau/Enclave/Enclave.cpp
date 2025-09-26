/*
 * Copyright (C) 2011-2016 Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Enclave.h"
#include "Enclave_t.h"

// Luau includes
#include "../luau-src/VM/include/lua.h"
#include "../luau-src/VM/include/lualib.h"
#include "../luau-src/Compiler/include/luacode.h"

void test_printf(){
	printf("TEST\n");
}

void test_scanf(){
	char s[17];
	printf("Enter a string: \n");
	scanf("%16s", s);
	printf("You entered \"%s\".\n",s);
}

void test_fwrite(){
	void* fp;
	char s[] = "Output text";
	fp = fopen("testfile.txt","w");
	fwrite(s, 1, strlen(s), fp);
	fclose(fp);
}

void test_fread(){
	void* fp;
	int amountRead = 0;
	char s[16];
	fp = fopen("testfile.txt","r");
	amountRead = fread(s, 1, 16, fp);
	printf("File contained: %s (read %d chars)\n", s, amountRead);
	fclose(fp);
}

void test_time(){
	long t = time(NULL);
	printf("Time: %ld\n", t);
}

// New Luau execution function
void run_luau(const char* script_source, size_t source_len) {
    printf("ScriptShield: Initializing Luau execution environment\n");

    // Create Luau state
    lua_State* L = luaL_newstate();
    if (!L) {
        printf("Error: Failed to create Luau state\n");
        return;
    }

    // Open minimal standard libraries
    luaL_openlibs(L);

    printf("ScriptShield: Compiling Luau script (%zu bytes)\n", source_len);

    // Set up compilation options
    lua_CompileOptions options = {};
    options.optimizationLevel = 1;  // Basic optimization
    options.debugLevel = 1;         // Line info for backtraces
    options.coverageLevel = 0;      // No coverage

    // Compile script to bytecode
    size_t bytecode_size;
    char* bytecode = luau_compile(script_source, source_len, &options, &bytecode_size);

    if (!bytecode) {
        printf("Error: Failed to compile Luau script\n");
        lua_close(L);
        return;
    }

    printf("ScriptShield: Script compiled to %zu bytes of bytecode\n", bytecode_size);

    // Load and execute bytecode
    if (luau_load(L, "script", bytecode, bytecode_size, 0) != LUA_OK) {
        printf("Error loading bytecode: %s\n", lua_tostring(L, -1));
        free(bytecode);
        lua_close(L);
        return;
    }

    printf("ScriptShield: Executing Luau script in SGX enclave\n");

    // Execute the script
    int result = lua_pcall(L, 0, 0, 0);
    if (result != LUA_OK) {
        printf("Script execution error: %s\n", lua_tostring(L, -1));
    } else {
        printf("ScriptShield: Script executed successfully\n");
    }

    // Cleanup
    free(bytecode);
    lua_close(L);
}

// Legacy function for backward compatibility
void run_lua(int argc, char *argv[]){
    printf("ScriptShield: Legacy run_lua called, switching to Luau\n");

    // Simple test script
    const char* test_script = R"(
        print("Hello from Luau running inside SGX enclave!")
        print("ScriptShield successfully modernized to use Luau")

        -- Test basic Luau features
        local x = 42
        local y = "test"
        print("Basic variables work:", x, y)

        -- Test function
        local function greet(name)
            return "Hello, " .. name .. "!"
        end

        print(greet("SGX"))
    )";

    run_luau(test_script, strlen(test_script));
}
