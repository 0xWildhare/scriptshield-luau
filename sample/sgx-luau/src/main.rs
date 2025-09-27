use std::os::raw::c_int;

extern "C" {
    fn run_all_tests() -> c_int;
}

fn main() {
    unsafe {
        run_all_tests();
    }
}