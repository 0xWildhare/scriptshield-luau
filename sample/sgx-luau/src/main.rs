use std::fs;
use std::ffi::{CString, CStr};
use std::os::raw::{c_char, c_int};

// FFI declarations for our C++ wrapper
#[repr(C)]
struct LuauState {
    _private: [u8; 0],
}

extern "C" {
    fn luau_new_state() -> *mut LuauState;
    fn luau_close_state(state: *mut LuauState);
    fn luau_run_simple_test(state: *mut LuauState) -> c_int;
    fn luau_run_production_test(state: *mut LuauState, script_content: *const c_char) -> c_int;
}

fn read_file(filename: &str) -> Result<String, std::io::Error> {
    fs::read_to_string(filename)
}

fn run_production_luau_test() {
    println!("\n=== Testing Production GitHub Mapper ===");

    unsafe {
        let state = luau_new_state();
        if state.is_null() {
            eprintln!("Failed to create Luau state");
            return;
        }

        // Load the production script - try multiple paths
        let script_paths = [
            "/production-script.lua",
            "/scriptshield-luau/production-script.lua",
            "production-script.lua",
        ];

        let mut script_content = String::new();

        for path in &script_paths {
            println!("Trying to load script from: {}", path);
            match read_file(path) {
                Ok(content) if !content.is_empty() => {
                    println!("Successfully loaded production script from: {}", path);
                    println!("Script size: {} bytes", content.len());
                    script_content = content;
                    break;
                }
                _ => {
                    println!("  File not found or empty");
                }
            }
        }

        if script_content.is_empty() {
            eprintln!("Failed to load production script from any location");
            luau_close_state(state);
            return;
        }

        // Convert to C string
        let script_cstr = CString::new(script_content).expect("CString conversion failed");

        // Run the production test
        let result = luau_run_production_test(state, script_cstr.as_ptr());

        if result != 0 {
            eprintln!("Production test failed");
        }

        luau_close_state(state);
    }
}

fn run_simple_test() {
    println!("=== Simple Test ===");

    unsafe {
        let state = luau_new_state();
        if state.is_null() {
            eprintln!("Failed to create Luau state");
            return;
        }

        let result = luau_run_simple_test(state);

        if result != 0 {
            eprintln!("Simple test failed");
        }

        luau_close_state(state);
    }
}

fn main() {
    println!("ScriptShield-Luau SGX Starting...");

    // Run simple test first
    run_simple_test();

    // Run production test
    run_production_luau_test();
}