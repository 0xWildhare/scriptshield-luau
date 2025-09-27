# Gramine SGX-Luau Makefile for ScriptShield (Rust version)

# Luau directories
LUAU_DIR = luau-src

# Target executable
APP_NAME = app

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra

# Include paths for Luau
LUAU_INCLUDES = -I./$(LUAU_DIR)/VM/include -I./$(LUAU_DIR)/Compiler/include -I./$(LUAU_DIR)/Ast/include -I./$(LUAU_DIR)/Common/include

# Define Luau source files
LUAU_VM_SRCS := $(wildcard $(LUAU_DIR)/VM/src/*.cpp)
LUAU_COMPILER_SRCS := $(wildcard $(LUAU_DIR)/Compiler/src/*.cpp)
LUAU_AST_SRCS := $(wildcard $(LUAU_DIR)/Ast/src/*.cpp)

LUAU_VM_OBJS := $(LUAU_VM_SRCS:.cpp=.o)
LUAU_COMPILER_OBJS := $(LUAU_COMPILER_SRCS:.cpp=.o)
LUAU_AST_OBJS := $(LUAU_AST_SRCS:.cpp=.o)
LUAU_OBJS := $(LUAU_VM_OBJS) $(LUAU_COMPILER_OBJS) $(LUAU_AST_OBJS)

.PHONY: all clean luau

all: $(APP_NAME)

# Compile Luau source files
$(LUAU_DIR)/%.o: $(LUAU_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) $(LUAU_INCLUDES) -c $< -o $@
	@echo "CXX  <=  $<"

# Compile C++ wrapper
src/luau_wrapper.o: src/luau_wrapper.cpp
	@$(CXX) $(CXXFLAGS) $(LUAU_INCLUDES) -c $< -o $@
	@echo "CXX  <=  $<"

# Build Luau static library with C++ wrapper
luau: $(LUAU_OBJS) src/luau_wrapper.o
	@ar rcs libluau.a $(LUAU_OBJS) src/luau_wrapper.o
	@echo "AR   =>  libluau.a"

# Build with Cargo after Luau library is ready
$(APP_NAME): luau
	@echo "Building Rust application with embedded Luau..."
	@cargo build --release
	@cp target/release/$(APP_NAME) ./$(APP_NAME)
	@echo "LINK =>  $@"

clean:
	@cargo clean
	@rm -f $(APP_NAME) libluau.a src/luau_wrapper.o
	@find $(LUAU_DIR) -name "*.o" -type f -delete 2>/dev/null || true
	@echo "Cleaned all build artifacts"