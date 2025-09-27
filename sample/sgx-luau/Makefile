# Gramine SGX-Luau Makefile for ScriptShield (Rust version)

# Target executable
APP_NAME = app

.PHONY: all clean

all: $(APP_NAME)

# Build with Cargo
$(APP_NAME):
	@echo "Building Rust application with embedded Luau..."
	@cargo build --release
	@cp target/release/$(APP_NAME) ./$(APP_NAME)
	@echo "LINK =>  $@"

clean:
	@cargo clean
	@rm -f $(APP_NAME)
	@echo "Cleaned all build artifacts"