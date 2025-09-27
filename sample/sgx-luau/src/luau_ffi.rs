use libc::{c_char, c_int, c_void, size_t};
use std::ffi::{CStr, CString};

// Lua state type
#[repr(C)]
pub struct lua_State {
    _private: [u8; 0],
}

// Lua types
pub const LUA_TNIL: c_int = 0;
pub const LUA_TBOOLEAN: c_int = 1;
pub const LUA_TLIGHTUSERDATA: c_int = 2;
pub const LUA_TNUMBER: c_int = 3;
pub const LUA_TSTRING: c_int = 4;
pub const LUA_TTABLE: c_int = 5;
pub const LUA_TFUNCTION: c_int = 6;
pub const LUA_TUSERDATA: c_int = 7;
pub const LUA_TTHREAD: c_int = 8;

// Lua compile options
#[repr(C)]
pub struct lua_CompileOptions {
    pub optimization_level: c_int,
    pub debug_level: c_int,
    pub coverage_level: c_int,
    pub vector_lib: *const c_char,
    pub vector_ctor: *const c_char,
    pub vector_type: *const c_char,
    pub mutable_globals: *const *const c_char,
}

impl Default for lua_CompileOptions {
    fn default() -> Self {
        Self {
            optimization_level: 1,
            debug_level: 1,
            coverage_level: 0,
            vector_lib: std::ptr::null(),
            vector_ctor: std::ptr::null(),
            vector_type: std::ptr::null(),
            mutable_globals: std::ptr::null(),
        }
    }
}

// C function type for Lua
pub type lua_CFunction = unsafe extern "C" fn(L: *mut lua_State) -> c_int;

extern "C" {
    // Basic Lua API
    pub fn luaL_newstate() -> *mut lua_State;
    pub fn lua_close(L: *mut lua_State);
    pub fn luaL_openlibs(L: *mut lua_State);

    // Compilation and loading
    pub fn luau_compile(
        source: *const c_char,
        size: size_t,
        options: *const lua_CompileOptions,
        outsize: *mut size_t,
    ) -> *mut c_char;

    pub fn luau_load(
        L: *mut lua_State,
        chunkname: *const c_char,
        data: *const c_char,
        size: size_t,
        env: c_int,
    ) -> c_int;

    // Execution
    pub fn lua_pcall(L: *mut lua_State, nargs: c_int, nresults: c_int, errfunc: c_int) -> c_int;

    // Stack manipulation
    pub fn lua_gettop(L: *mut lua_State) -> c_int;
    pub fn lua_settop(L: *mut lua_State, idx: c_int);
    pub fn lua_type(L: *mut lua_State, idx: c_int) -> c_int;
    pub fn lua_typename(L: *mut lua_State, tp: c_int) -> *const c_char;

    // Value access
    pub fn lua_tostring(L: *mut lua_State, idx: c_int) -> *const c_char;
    pub fn lua_tonumber(L: *mut lua_State, idx: c_int) -> f64;
    pub fn lua_toboolean(L: *mut lua_State, idx: c_int) -> c_int;

    // Table operations
    pub fn lua_newtable(L: *mut lua_State);
    pub fn lua_getfield(L: *mut lua_State, idx: c_int, k: *const c_char);
    pub fn lua_setfield(L: *mut lua_State, idx: c_int, k: *const c_char);

    // Push operations
    pub fn lua_pushstring(L: *mut lua_State, s: *const c_char);
    pub fn lua_pushnumber(L: *mut lua_State, n: f64);
    pub fn lua_pushinteger(L: *mut lua_State, n: i64);
    pub fn lua_pushboolean(L: *mut lua_State, b: c_int);
    pub fn lua_pushnil(L: *mut lua_State);
    pub fn lua_pushcclosurek(
        L: *mut lua_State,
        fn_: lua_CFunction,
        debugname: *const c_char,
        nup: c_int,
        cont: *mut c_void,
    );

    // Global operations
    pub fn lua_setglobal(L: *mut lua_State, name: *const c_char);

    // Type checking
    pub fn lua_isnil(L: *mut lua_State, idx: c_int) -> c_int;
    pub fn lua_isstring(L: *mut lua_State, idx: c_int) -> c_int;
    pub fn lua_isnumber(L: *mut lua_State, idx: c_int) -> c_int;
    pub fn lua_isfunction(L: *mut lua_State, idx: c_int) -> c_int;
    pub fn lua_istable(L: *mut lua_State, idx: c_int) -> c_int;

    // Memory management
    pub fn free(ptr: *mut c_void);
}

// Helper function to push a C function with debug name
pub unsafe fn lua_pushcfunction(L: *mut lua_State, f: lua_CFunction, debugname: &str) {
    let name_cstr = CString::new(debugname).unwrap();
    lua_pushcclosurek(L, f, name_cstr.as_ptr(), 0, std::ptr::null_mut());
}

// Helper to convert Rust string to C string safely
pub fn to_cstring(s: &str) -> CString {
    CString::new(s).expect("CString conversion failed")
}

// Helper to convert C string to Rust string safely
pub unsafe fn from_cstr(ptr: *const c_char) -> Option<String> {
    if ptr.is_null() {
        None
    } else {
        CStr::from_ptr(ptr).to_str().ok().map(|s| s.to_string())
    }
}