fn main() {
    // Compile Luau sources
    let mut build = cc::Build::new();
    build.cpp(true)
         .std("c++17")
         .opt_level(2)
         .flag("-Wno-unused-parameter")
         .flag("-Wno-missing-field-initializers")
         .flag("-Wno-maybe-uninitialized");

    // Include directories
    build.include("luau-src/VM/include")
         .include("luau-src/Compiler/include")
         .include("luau-src/Ast/include")
         .include("luau-src/Common/include");

    // Collect source files from each component
    for component in ["VM", "Compiler", "Ast"] {
        let src_dir = format!("luau-src/{}/src", component);
        if std::path::Path::new(&src_dir).exists() {
            let sources = std::fs::read_dir(&src_dir)
                .unwrap_or_else(|_| panic!("Failed to read {} src directory", src_dir))
                .filter_map(|entry| entry.ok())
                .filter(|entry| entry.path().extension().map_or(false, |ext| ext == "cpp"))
                .map(|entry| entry.path());

            for source in sources {
                build.file(source);
            }
        }
    }

    // Add our C++ wrapper
    build.file("src/luau_wrapper.cpp");

    // Compile static library
    build.compile("luau");

    // Link with standard library
    println!("cargo:rustc-link-lib=dylib=stdc++");

    // Rebuild conditions
    println!("cargo:rerun-if-changed=src/luau_wrapper.cpp");
    println!("cargo:rerun-if-changed=luau-src");
}