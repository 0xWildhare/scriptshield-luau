use std::fs;
use std::ffi::CString;
use std::ptr;
use libc::{c_char, c_int, size_t};

mod luau_ffi;
use luau_ffi::*;

fn read_file(filename: &str) -> Result<String, std::io::Error> {
    fs::read_to_string(filename)
}

// Mock parse_json function for Luau
unsafe extern "C" fn parse_json_mock(L: *mut lua_State) -> c_int {
    // Get the JSON string from Lua stack
    let json_str_ptr = lua_tostring(L, 1);
    if json_str_ptr.is_null() {
        lua_pushnil(L);
        return 1;
    }

    let json_str = match from_cstr(json_str_ptr) {
        Some(s) => s,
        None => {
            lua_pushnil(L);
            return 1;
        }
    };

    // Check if this is our test GitHub data
    if json_str.contains("\"login\":\"ospfranco\"") {
        // Create a Lua table representing the parsed GitHub data
        lua_newtable(L);

        // Add basic fields
        lua_pushstring(L, to_cstring("ospfranco").as_ptr());
        lua_setfield(L, -2, to_cstring("login").as_ptr());

        lua_pushinteger(L, 1634213);
        lua_setfield(L, -2, to_cstring("id").as_ptr());

        lua_pushstring(L, to_cstring("MDQ6VXNlcjE2MzQyMTM=").as_ptr());
        lua_setfield(L, -2, to_cstring("node_id").as_ptr());

        lua_pushstring(L, to_cstring("Oscar Franco").as_ptr());
        lua_setfield(L, -2, to_cstring("name").as_ptr());

        lua_pushstring(L, to_cstring("Freelance Dev").as_ptr());
        lua_setfield(L, -2, to_cstring("bio").as_ptr());

        lua_pushstring(L, to_cstring("ospfranco.com").as_ptr());
        lua_setfield(L, -2, to_cstring("blog").as_ptr());

        lua_pushstring(L, to_cstring("Barcelona, Spain").as_ptr());
        lua_setfield(L, -2, to_cstring("location").as_ptr());

        lua_pushstring(L, to_cstring("ospfranco").as_ptr());
        lua_setfield(L, -2, to_cstring("twitter_username").as_ptr());

        lua_pushinteger(L, 47);
        lua_setfield(L, -2, to_cstring("public_repos").as_ptr());

        lua_pushinteger(L, 14);
        lua_setfield(L, -2, to_cstring("public_gists").as_ptr());

        lua_pushinteger(L, 533);
        lua_setfield(L, -2, to_cstring("followers").as_ptr());

        lua_pushinteger(L, 6);
        lua_setfield(L, -2, to_cstring("following").as_ptr());

        lua_pushstring(L, to_cstring("2012-04-11T19:00:30Z").as_ptr());
        lua_setfield(L, -2, to_cstring("created_at").as_ptr());

        lua_pushstring(L, to_cstring("2025-09-10T20:03:58Z").as_ptr());
        lua_setfield(L, -2, to_cstring("updated_at").as_ptr());

        lua_pushinteger(L, 325487);
        lua_setfield(L, -2, to_cstring("disk_usage").as_ptr());

        lua_pushboolean(L, 1);
        lua_setfield(L, -2, to_cstring("two_factor_authentication").as_ptr());

        // Add plan table
        lua_newtable(L);
        lua_pushstring(L, to_cstring("free").as_ptr());
        lua_setfield(L, -2, to_cstring("name").as_ptr());
        lua_pushinteger(L, 10000);
        lua_setfield(L, -2, to_cstring("private_repos").as_ptr());
        lua_setfield(L, -2, to_cstring("plan").as_ptr());

        return 1;
    }

    lua_pushnil(L);
    1
}

fn run_production_luau_test() {
    println!("\n=== Testing Production GitHub Mapper ===");

    unsafe {
        let L = luaL_newstate();
        if L.is_null() {
            eprintln!("Failed to create Lua state");
            return;
        }

        luaL_openlibs(L);

        // Add mock parse_json function to global scope
        lua_pushcfunction(L, parse_json_mock, "parse_json");
        lua_setglobal(L, to_cstring("parse_json").as_ptr());

        // Load the production script - try multiple paths
        let script_paths = [
            "/production-script.lua",
            "/scriptshield-luau/production-script.lua",
            "production-script.lua",
        ];

        let mut script_content = String::new();
        let mut found_path = None;

        for path in &script_paths {
            println!("Trying to load script from: {}", path);
            match read_file(path) {
                Ok(content) if !content.is_empty() => {
                    println!("Successfully loaded production script from: {}", path);
                    println!("Script size: {} bytes", content.len());
                    script_content = content;
                    found_path = Some(*path);
                    break;
                }
                _ => {
                    println!("  File not found or empty");
                }
            }
        }

        if script_content.is_empty() {
            eprintln!("Failed to load production script from any location");
            lua_close(L);
            return;
        }

        // Compile the script
        let mut bytecode_size: size_t = 0;
        let options = lua_CompileOptions::default();
        let script_cstr = to_cstring(&script_content);

        let bytecode = luau_compile(
            script_cstr.as_ptr(),
            script_content.len(),
            &options,
            &mut bytecode_size,
        );

        if bytecode.is_null() {
            eprintln!("Script compilation failed");
            lua_close(L);
            return;
        }

        // Load the bytecode
        let chunkname = to_cstring("production-script");
        if luau_load(L, chunkname.as_ptr(), bytecode, bytecode_size, 0) != 0 {
            let error_msg = from_cstr(lua_tostring(L, -1)).unwrap_or("Unknown error".to_string());
            eprintln!("Script load failed: {}", error_msg);
            free(bytecode as *mut _);
            lua_close(L);
            return;
        }

        // Execute the script (this loads the mapper function)
        if lua_pcall(L, 0, 1, 0) != 0 {
            let error_msg = from_cstr(lua_tostring(L, -1)).unwrap_or("Unknown error".to_string());
            eprintln!("Script execution failed: {}", error_msg);
            free(bytecode as *mut _);
            lua_close(L);
            return;
        }

        // The script should return a mapper function
        if lua_isfunction(L, -1) != 0 {
            // Create test payload
            lua_newtable(L); // params table

            lua_newtable(L); // payload table
            let json_data = r#"{"login":"ospfranco","id":1634213,"node_id":"MDQ6VXNlcjE2MzQyMTM=","name":"Oscar Franco","bio":"Freelance Dev","blog":"ospfranco.com","location":"Barcelona, Spain","twitter_username":"ospfranco","public_repos":47,"public_gists":14,"followers":533,"following":6,"created_at":"2012-04-11T19:00:30Z","updated_at":"2025-09-10T20:03:58Z","disk_usage":325487,"two_factor_authentication":true,"plan":{"name":"free","private_repos":10000}}"#;
            lua_pushstring(L, to_cstring(json_data).as_ptr());
            lua_setfield(L, -2, to_cstring("default").as_ptr());
            lua_setfield(L, -2, to_cstring("payload").as_ptr());

            // Call the mapper function
            if lua_pcall(L, 1, 1, 0) == 0 {
                // Print the result
                println!("Mapper executed successfully!");
                let type_name = from_cstr(lua_typename(L, lua_type(L, -1)))
                    .unwrap_or("unknown".to_string());
                println!("Result type: {}", type_name);

                if lua_istable(L, -1) != 0 {
                    println!("Processed data contains:");

                    // Check for some expected fields
                    lua_getfield(L, -1, to_cstring("username").as_ptr());
                    if lua_isstring(L, -1) != 0 {
                        if let Some(username) = from_cstr(lua_tostring(L, -1)) {
                            println!("  username: {}", username);
                        }
                    }
                    lua_settop(L, -2); // pop the value

                    lua_getfield(L, -1, to_cstring("display_name").as_ptr());
                    if lua_isstring(L, -1) != 0 {
                        if let Some(display_name) = from_cstr(lua_tostring(L, -1)) {
                            println!("  display_name: {}", display_name);
                        }
                    }
                    lua_settop(L, -2); // pop the value

                    lua_getfield(L, -1, to_cstring("followers").as_ptr());
                    if lua_isnumber(L, -1) != 0 {
                        let followers = lua_tonumber(L, -1);
                        println!("  followers: {}", followers);
                    }
                    lua_settop(L, -2); // pop the value

                    lua_getfield(L, -1, to_cstring("public_repos").as_ptr());
                    if lua_isnumber(L, -1) != 0 {
                        let public_repos = lua_tonumber(L, -1);
                        println!("  public_repos: {}", public_repos);
                    }
                    lua_settop(L, -2); // pop the value

                    // Verify that node_id is NOT in the output (proving processing occurred)
                    lua_getfield(L, -1, to_cstring("node_id").as_ptr());
                    if lua_isnil(L, -1) != 0 {
                        println!("✅ VERIFICATION: node_id correctly filtered out (not in output)");
                    } else {
                        let node_id = from_cstr(lua_tostring(L, -1))
                            .unwrap_or("unknown".to_string());
                        println!("❌ VERIFICATION FAILED: node_id found in output: {}", node_id);
                    }
                    lua_settop(L, -2); // pop the value
                }
            } else {
                let error_msg = from_cstr(lua_tostring(L, -1)).unwrap_or("Unknown error".to_string());
                eprintln!("Mapper call failed: {}", error_msg);
            }
        } else {
            eprintln!("Script did not return a function");
        }

        free(bytecode as *mut _);
        lua_close(L);
    }
}

fn run_simple_test() {
    println!("=== Simple Test ===");

    unsafe {
        let L = luaL_newstate();
        if L.is_null() {
            eprintln!("Failed to create Lua state");
            return;
        }

        luaL_openlibs(L);

        let test_script = r#"
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
        "#;

        // Compile the script
        let mut bytecode_size: size_t = 0;
        let options = lua_CompileOptions::default();
        let script_cstr = to_cstring(test_script);

        let bytecode = luau_compile(
            script_cstr.as_ptr(),
            test_script.len(),
            &options,
            &mut bytecode_size,
        );

        if bytecode.is_null() {
            eprintln!("Compilation failed");
            lua_close(L);
            return;
        }

        // Load and run the bytecode
        let chunkname = to_cstring("simple-test");
        if luau_load(L, chunkname.as_ptr(), bytecode, bytecode_size, 0) == 0 {
            if lua_pcall(L, 0, 0, 0) != 0 {
                let error_msg = from_cstr(lua_tostring(L, -1)).unwrap_or("Unknown error".to_string());
                eprintln!("Runtime error: {}", error_msg);
            }
        } else {
            let error_msg = from_cstr(lua_tostring(L, -1)).unwrap_or("Unknown error".to_string());
            eprintln!("Load error: {}", error_msg);
        }

        free(bytecode as *mut _);
        lua_close(L);
    }
}

fn main() {
    println!("ScriptShield-Luau SGX Starting...");

    // Run simple test first
    run_simple_test();

    // Run production test
    run_production_luau_test();
}