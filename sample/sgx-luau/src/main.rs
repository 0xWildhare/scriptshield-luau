use std::ffi::{CStr, CString};
use std::fs;
use std::os::raw::{c_char, c_int};
use std::ptr;
use serde_json::{Value, Map};

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
    fn execute_mapper_with_parsed_data(state: *mut LuaState, mapper_code: *const c_char, payload_json: *const c_char) -> *mut LuauResult;
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

    pub fn execute_mapper_with_parsed_data(&self, mapper_code: &str, payload_json: &str) -> Result<String, String> {
        unsafe {
            let mapper_code_cstr = CString::new(mapper_code).map_err(|e| format!("Invalid mapper code: {}", e))?;
            let payload_json_cstr = CString::new(payload_json).map_err(|e| format!("Invalid payload JSON: {}", e))?;

            let result_ptr = execute_mapper_with_parsed_data(self.state, mapper_code_cstr.as_ptr(), payload_json_cstr.as_ptr());

            if result_ptr.is_null() {
                return Err("Failed to execute mapper with parsed data".to_string());
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

    // Create test parameters with nested JSON structure
    let params_json_str = r#"{
        "payload": {
            "default": "{\"login\":\"ospfranco\",\"id\":1634213,\"node_id\":\"MDQ6VXNlcjE2MzQyMTM=\",\"name\":\"Oscar Franco\",\"bio\":\"Freelance Dev\",\"blog\":\"ospfranco.com\",\"location\":\"Barcelona, Spain\",\"twitter_username\":\"ospfranco\",\"public_repos\":47,\"public_gists\":14,\"followers\":533,\"following\":6,\"created_at\":\"2012-04-11T19:00:30Z\",\"updated_at\":\"2025-09-10T20:03:58Z\",\"disk_usage\":325487,\"two_factor_authentication\":true,\"plan\":{\"name\":\"free\",\"private_repos\":10000}}"
        }
    }"#;

    println!("🔍 Parsing JSON parameters...");

    // Parse the JSON parameters in Rust
    let params: Value = match serde_json::from_str(params_json_str) {
        Ok(params) => params,
        Err(e) => {
            eprintln!("❌ Failed to parse params JSON: {}", e);
            return;
        }
    };

    // Extract the payload.default string and parse it as JSON
    let payload_json_str = match params.get("payload")
        .and_then(|p| p.get("default"))
        .and_then(|d| d.as_str())
    {
        Some(json_str) => json_str,
        None => {
            eprintln!("❌ Failed to extract payload.default from params");
            return;
        }
    };

    println!("🔍 Parsing payload JSON...");
    let payload: Value = match serde_json::from_str(payload_json_str) {
        Ok(payload) => payload,
        Err(e) => {
            eprintln!("❌ Failed to parse payload JSON: {}", e);
            return;
        }
    };

    // Verify we have expected GitHub data
    if let Some(login) = payload.get("login").and_then(|v| v.as_str()) {
        println!("✅ Parsed GitHub data for user: {}", login);
    } else {
        eprintln!("❌ Invalid GitHub data structure");
        return;
    }

    // Execute the mapper with the raw payload JSON (the C++ side will parse it)
    println!("🚀 Executing mapper with parsed GitHub data...");
    match luau_env.execute_mapper_with_parsed_data(&mapper_code, payload_json_str) {
        Ok(result) => {
            println!("✅ Mapper execution successful!");
            println!("📊 Result JSON: {}", result);

            // Parse the result to do proper verification
            match serde_json::from_str::<Value>(&result) {
                Ok(result_json) => {
                    // Verify that node_id is not in the result
                    if result_json.get("node_id").is_some() {
                        println!("❌ VERIFICATION FAILED: node_id found in output");
                    } else {
                        println!("✅ VERIFICATION PASSED: node_id correctly filtered out");
                    }

                    // Show what fields are present
                    if let Some(obj) = result_json.as_object() {
                        println!("📋 Result contains fields: {:?}", obj.keys().collect::<Vec<_>>());
                    }
                }
                Err(e) => {
                    println!("⚠️  Could not parse result as JSON: {}", e);
                    // Still check string-based for compatibility
                    if result.contains("node_id") {
                        println!("❌ VERIFICATION FAILED: node_id found in output");
                    } else {
                        println!("✅ VERIFICATION PASSED: node_id correctly filtered out");
                    }
                }
            }
        }
        Err(e) => {
            eprintln!("❌ Mapper execution failed: {}", e);
        }
    }
}

fn main() {
    println!("🔒 ScriptShield-Luau SGX Starting...");

    // Run original tests
    unsafe {
        run_all_tests();
    }

    // Run enhanced Rust integration test (now running inside the enclave with Gramine)
    run_enhanced_test();
}