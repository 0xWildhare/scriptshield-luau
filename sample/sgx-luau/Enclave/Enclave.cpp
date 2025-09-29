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

// SGX-compatible includes for size_t
#include <stddef.h>

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

// New Luau execution function (MINIMAL TEST VERSION)
void run_luau(const char* script_source, size_t source_len) {
    printf("ScriptShield: TEST MODE - Luau execution simulation\n");
    printf("ScriptShield: Received script of %zu bytes\n", source_len);
    printf("ScriptShield: Script content preview: %.50s%s\n",
           script_source,
           source_len > 50 ? "..." : "");
    printf("ScriptShield: SGX Enclave is working! Luau integration pending.\n");
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

// Mock parse_json function for SGX environment
static int parse_json_mock(lua_State* L) {
    const char* json_str = luaL_checkstring(L, 1);

    // For this test, we'll create a simple mock that recognizes our test GitHub data
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

// SGX-compatible file reading function
char* read_file_sgx(const char* filename) {
    void* fp = fopen(filename, "r");
    if (!fp) {
        return NULL;
    }

    // Get file size by seeking to end
    // Note: In real SGX, you'd want proper file handling
    // This is a simplified version for the test
    char* buffer = (char*)malloc(8192); // Allocate reasonable buffer
    if (!buffer) {
        fclose(fp);
        return NULL;
    }

    size_t total_read = 0;
    size_t bytes_read;

    // Read the file in chunks
    while ((bytes_read = fread(buffer + total_read, 1, 1024, fp)) > 0 && total_read < 7168) {
        total_read += bytes_read;
    }

    buffer[total_read] = '\0'; // Null terminate
    fclose(fp);

    if (total_read == 0) {
        free(buffer);
        return NULL;
    }

    return buffer;
}

// Enhanced Rust test function running inside SGX enclave
void run_enhanced_rust_test() {
    printf("\n=== Enhanced Rust Integration Test (Inside SGX Enclave) ===\n");

    // Create Luau environment
    lua_State* L = luaL_newstate();
    if (!L) {
        printf("Failed to create Luau environment\n");
        return;
    }

    luaL_openlibs(L);

    // Add mock parse_json function to global scope
    lua_pushcfunction(L, parse_json_mock, "parse_json");
    lua_setglobal(L, "parse_json");

    // Load the production mapper script
    const char* script_paths[] = {
        "/production-script.lua",
        "/scriptshield-luau/production-script.lua",
        "production-script.lua",
        NULL
    };

    char* mapper_code = NULL;
    for (int i = 0; script_paths[i] != NULL; i++) {
        printf("Trying to load mapper from: %s\n", script_paths[i]);
        mapper_code = read_file_sgx(script_paths[i]);
        if (mapper_code && strlen(mapper_code) > 0) {
            printf("✅ Loaded mapper script from: %s\n", script_paths[i]);
            printf("   Script size: %zu bytes\n", strlen(mapper_code));
            break;
        } else {
            printf("   ❌ File not found or empty\n");
            if (mapper_code) {
                free(mapper_code);
                mapper_code = NULL;
            }
        }
    }

    if (!mapper_code) {
        printf("Failed to load mapper script from any location\n");
        lua_close(L);
        return;
    }

    // Compile the script
    size_t bytecode_size = 0;
    lua_CompileOptions options = {};
    char* bytecode = luau_compile(mapper_code, strlen(mapper_code), &options, &bytecode_size);

    if (!bytecode) {
        printf("❌ Script compilation failed\n");
        free(mapper_code);
        lua_close(L);
        return;
    }

    // Load the bytecode
    if (luau_load(L, "production-script", bytecode, bytecode_size, 0) != 0) {
        printf("❌ Script load failed: %s\n", lua_tostring(L, -1));
        free(bytecode);
        free(mapper_code);
        lua_close(L);
        return;
    }

    // Execute the script (this loads the mapper function)
    if (lua_pcall(L, 0, 1, 0) != 0) {
        printf("❌ Script execution failed: %s\n", lua_tostring(L, -1));
        free(bytecode);
        free(mapper_code);
        lua_close(L);
        return;
    }

    // Check if the result is a function
    if (!lua_isfunction(L, -1)) {
        printf("❌ Script did not return a function\n");
        free(bytecode);
        free(mapper_code);
        lua_close(L);
        return;
    }

    // Create test parameters (GitHub API data)
    lua_newtable(L); // params table

    lua_newtable(L); // payload table
    lua_pushstring(L, "{\"login\":\"ospfranco\",\"id\":1634213,\"node_id\":\"MDQ6VXNlcjE2MzQyMTM=\",\"name\":\"Oscar Franco\",\"bio\":\"Freelance Dev\",\"blog\":\"ospfranco.com\",\"location\":\"Barcelona, Spain\",\"twitter_username\":\"ospfranco\",\"public_repos\":47,\"public_gists\":14,\"followers\":533,\"following\":6,\"created_at\":\"2012-04-11T19:00:30Z\",\"updated_at\":\"2025-09-10T20:03:58Z\",\"disk_usage\":325487,\"two_factor_authentication\":true,\"plan\":{\"name\":\"free\",\"private_repos\":10000}}");
    lua_setfield(L, -2, "default");
    lua_setfield(L, -2, "payload");

    // Execute the mapper
    printf("🚀 Executing mapper with GitHub data...\n");
    if (lua_pcall(L, 1, 1, 0) != 0) {
        printf("❌ Mapper execution failed: %s\n", lua_tostring(L, -1));
        free(bytecode);
        free(mapper_code);
        lua_close(L);
        return;
    }

    printf("✅ Mapper execution successful!\n");

    // Verify results
    if (lua_istable(L, -1)) {
        printf("📊 Result analysis:\n");

        // Check for expected fields
        lua_getfield(L, -1, "username");
        if (lua_isstring(L, -1)) {
            printf("  username: %s\n", lua_tostring(L, -1));
        }
        lua_pop(L, 1);

        lua_getfield(L, -1, "display_name");
        if (lua_isstring(L, -1)) {
            printf("  display_name: %s\n", lua_tostring(L, -1));
        }
        lua_pop(L, 1);

        lua_getfield(L, -1, "followers");
        if (lua_isnumber(L, -1)) {
            printf("  followers: %.0f\n", lua_tonumber(L, -1));
        }
        lua_pop(L, 1);

        lua_getfield(L, -1, "public_repos");
        if (lua_isnumber(L, -1)) {
            printf("  public_repos: %.0f\n", lua_tonumber(L, -1));
        }
        lua_pop(L, 1);

        // Verify that node_id is not in the result
        lua_getfield(L, -1, "node_id");
        if (lua_isnil(L, -1)) {
            printf("✅ VERIFICATION PASSED: node_id correctly filtered out\n");
        } else {
            printf("❌ VERIFICATION FAILED: node_id found in output\n");
        }
        lua_pop(L, 1);

    } else {
        printf("❌ Mapper did not return a table\n");
    }

    // Clean up
    free(bytecode);
    free(mapper_code);
    lua_close(L);
}
