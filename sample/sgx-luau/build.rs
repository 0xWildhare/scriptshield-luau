fn main() {
    // Tell Cargo to look for the pre-built libluau.a in the current directory
    println!("cargo:rustc-link-search=native=.");
    println!("cargo:rustc-link-lib=static=luau");
    println!("cargo:rustc-link-lib=dylib=stdc++");

    // Re-run if the library changes
    println!("cargo:rerun-if-changed=libluau.a");
}