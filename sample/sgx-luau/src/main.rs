use std::ffi::{CStr, CString};
use std::fs;
use std::os::raw::{c_char, c_int};
use std::ptr;

// FFI bindings for the C++ Luau wrapper
#[repr(C)]
struct LuauResult {
    success: bool,
    result_json: *mut c_char,
    error_message: *mut c_char,
}

#[repr(C)]
struct LuaState {
    _private: [u8; 0],
}

extern "C" {
    fn run_all_tests() -> c_int;
    fn create_luau_env() -> *mut LuaState;
    fn close_luau_env(state: *mut LuaState);
    fn execute_mapper(state: *mut LuaState, mapper_code: *const c_char, params_json: *const c_char) -> *mut LuauResult;
    fn free_luau_result(result: *mut LuauResult);
}

// Safe Rust wrapper for Luau operations
pub struct LuauEnvironment {
    state: *mut LuaState,
}

impl LuauEnvironment {
    pub fn new() -> Result<Self, String> {
        unsafe {
            let state = create_luau_env();
            if state.is_null() {
                Err("Failed to create Luau environment".to_string())
            } else {
                Ok(LuauEnvironment { state })
            }
        }
    }

    pub fn execute_mapper(&self, mapper_code: &str, params_json: &str) -> Result<String, String> {
        unsafe {
            let mapper_code_cstr = CString::new(mapper_code).map_err(|e| format!("Invalid mapper code: {}", e))?;
            let params_json_cstr = CString::new(params_json).map_err(|e| format!("Invalid params JSON: {}", e))?;

            let result_ptr = execute_mapper(self.state, mapper_code_cstr.as_ptr(), params_json_cstr.as_ptr());

            if result_ptr.is_null() {
                return Err("Failed to execute mapper".to_string());
            }

            let result = &*result_ptr;

            if result.success {
                if result.result_json.is_null() {
                    free_luau_result(result_ptr);
                    return Err("Mapper succeeded but returned null result".to_string());
                }

                let json_str = CStr::from_ptr(result.result_json)
                    .to_str()
                    .map_err(|e| format!("Invalid UTF-8 in result: {}", e))?
                    .to_string();

                free_luau_result(result_ptr);
                Ok(json_str)
            } else {
                let error_msg = if result.error_message.is_null() {
                    "Unknown error".to_string()
                } else {
                    CStr::from_ptr(result.error_message)
                        .to_str()
                        .unwrap_or("Unknown error")
                        .to_string()
                };

                free_luau_result(result_ptr);
                Err(error_msg)
            }
        }
    }
}

impl Drop for LuauEnvironment {
    fn drop(&mut self) {
        unsafe {
            close_luau_env(self.state);
        }
    }
}

fn run_enhanced_test() {
    println!("\n=== Enhanced Rust Integration Test ===");

    // Create Luau environment
    let luau_env = match LuauEnvironment::new() {
        Ok(env) => env,
        Err(e) => {
            eprintln!("Failed to create Luau environment: {}", e);
            return;
        }
    };

    // Load the production mapper script
    let script_paths = [
        "/production-script.lua",
        "/scriptshield-luau/production-script.lua",
        "production-script.lua",
    ];

    let mut mapper_code = String::new();
    for path in &script_paths {
        println!("Trying to load mapper from: {}", path);
        match fs::read_to_string(path) {
            Ok(content) if !content.is_empty() => {
                println!("✅ Loaded mapper script from: {}", path);
                println!("   Script size: {} bytes", content.len());
                mapper_code = content;
                break;
            }
            _ => {
                println!("   ❌ File not found or empty");
            }
        }
    }

    if mapper_code.is_empty() {
        eprintln!("Failed to load mapper script from any location");
        return;
    }

    // Create test parameters (GitHub API data)
    let params_json = r#"{
        "payload": {
            "default": "{\"login\":\"ospfranco\",\"id\":1634213,\"node_id\":\"MDQ6VXNlcjE2MzQyMTM=\",\"name\":\"Oscar Franco\",\"bio\":\"Freelance Dev\",\"blog\":\"ospfranco.com\",\"location\":\"Barcelona, Spain\",\"twitter_username\":\"ospfranco\",\"public_repos\":47,\"public_gists\":14,\"followers\":533,\"following\":6,\"created_at\":\"2012-04-11T19:00:30Z\",\"updated_at\":\"2025-09-10T20:03:58Z\",\"disk_usage\":325487,\"two_factor_authentication\":true,\"plan\":{\"name\":\"free\",\"private_repos\":10000}}"
        }
    }"#;

    // Execute the mapper
    println!("🚀 Executing mapper with GitHub data...");
    match luau_env.execute_mapper(&mapper_code, params_json) {
        Ok(result) => {
            println!("✅ Mapper execution successful!");
            println!("📊 Result JSON: {}", result);

            // Verify that node_id is not in the result
            if result.contains("node_id") {
                println!("❌ VERIFICATION FAILED: node_id found in output");
            } else {
                println!("✅ VERIFICATION PASSED: node_id correctly filtered out");
            }
        }
        Err(e) => {
            eprintln!("❌ Mapper execution failed: {}", e);
        }
    }
}

fn main() {
    println!("🔒 ScriptShield-Luau SGX Starting...");
    println!("Note: Enhanced Rust test logic has been moved into the SGX enclave for better security.");

    // Run original tests (these are now mostly running inside the enclave)
    unsafe {
        run_all_tests();
    }

    println!("✅ Rust host application completed. The enhanced test is now running inside the SGX enclave.");
}