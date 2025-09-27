use std::env;
use std::path::PathBuf;

fn main() {
    let luau_dir = "luau-src";

    // Build Luau VM
    cc::Build::new()
        .cpp(true)
        .std("c++17")
        .warnings(false)
        .include(format!("{}/VM/include", luau_dir))
        .include(format!("{}/Compiler/include", luau_dir))
        .include(format!("{}/Ast/include", luau_dir))
        .include(format!("{}/Common/include", luau_dir))
        .files([
            // VM sources
            format!("{}/VM/src/lapi.cpp", luau_dir),
            format!("{}/VM/src/laux.cpp", luau_dir),
            format!("{}/VM/src/lbaselib.cpp", luau_dir),
            format!("{}/VM/src/lbitlib.cpp", luau_dir),
            format!("{}/VM/src/lbuiltins.cpp", luau_dir),
            format!("{}/VM/src/lcorolib.cpp", luau_dir),
            format!("{}/VM/src/ldblib.cpp", luau_dir),
            format!("{}/VM/src/ldebug.cpp", luau_dir),
            format!("{}/VM/src/ldo.cpp", luau_dir),
            format!("{}/VM/src/lfunc.cpp", luau_dir),
            format!("{}/VM/src/lgc.cpp", luau_dir),
            format!("{}/VM/src/lgcdebug.cpp", luau_dir),
            format!("{}/VM/src/linit.cpp", luau_dir),
            format!("{}/VM/src/lmathlib.cpp", luau_dir),
            format!("{}/VM/src/lmem.cpp", luau_dir),
            format!("{}/VM/src/lobject.cpp", luau_dir),
            format!("{}/VM/src/loslib.cpp", luau_dir),
            format!("{}/VM/src/lperf.cpp", luau_dir),
            format!("{}/VM/src/lstate.cpp", luau_dir),
            format!("{}/VM/src/lstring.cpp", luau_dir),
            format!("{}/VM/src/lstrlib.cpp", luau_dir),
            format!("{}/VM/src/ltable.cpp", luau_dir),
            format!("{}/VM/src/ltablib.cpp", luau_dir),
            format!("{}/VM/src/ltm.cpp", luau_dir),
            format!("{}/VM/src/ludata.cpp", luau_dir),
            format!("{}/VM/src/lutf8lib.cpp", luau_dir),
            format!("{}/VM/src/lvmexecute.cpp", luau_dir),
            format!("{}/VM/src/lvmload.cpp", luau_dir),
            format!("{}/VM/src/lvmutils.cpp", luau_dir),
            // Compiler sources
            format!("{}/Compiler/src/BytecodeBuilder.cpp", luau_dir),
            format!("{}/Compiler/src/Compiler.cpp", luau_dir),
            format!("{}/Compiler/src/Builtins.cpp", luau_dir),
            format!("{}/Compiler/src/ConstantFolding.cpp", luau_dir),
            format!("{}/Compiler/src/CostModel.cpp", luau_dir),
            format!("{}/Compiler/src/TableShape.cpp", luau_dir),
            format!("{}/Compiler/src/Types.cpp", luau_dir),
            format!("{}/Compiler/src/ValueTracking.cpp", luau_dir),
            format!("{}/Compiler/src/lcode.cpp", luau_dir),
            // AST sources
            format!("{}/Ast/src/Ast.cpp", luau_dir),
            format!("{}/Ast/src/Confusables.cpp", luau_dir),
            format!("{}/Ast/src/Lexer.cpp", luau_dir),
            format!("{}/Ast/src/Location.cpp", luau_dir),
            format!("{}/Ast/src/Parser.cpp", luau_dir),
            format!("{}/Ast/src/StringUtils.cpp", luau_dir),
            format!("{}/Ast/src/TimeTrace.cpp", luau_dir),
        ])
        .compile("luau");

    println!("cargo:rustc-link-lib=static=luau");
    println!("cargo:rustc-link-lib=dylib=stdc++");
}