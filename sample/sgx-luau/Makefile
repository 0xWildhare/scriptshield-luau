# Gramine SGX-Luau Makefile for ScriptShield

# Luau directories
LUAU_DIR = luau-src

# Compiler settings
CXX = g++
CC = gcc
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
CFLAGS = -O2 -Wall -Wextra

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

# Application source files
APP_SRCS = main.cpp
APP_OBJS = $(APP_SRCS:.cpp=.o)

# Target executable
APP_NAME = app

.PHONY: all clean luau

all: $(APP_NAME)

# Compile Luau source files
$(LUAU_DIR)/%.o: $(LUAU_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) $(LUAU_INCLUDES) -c $< -o $@
	@echo "CXX  <=  $<"

luau: $(LUAU_OBJS)
	@echo "Built Luau components"

# Compile application
%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(LUAU_INCLUDES) -c $< -o $@
	@echo "CXX  <=  $<"

# Link final application
$(APP_NAME): luau $(APP_OBJS)
	@$(CXX) $(APP_OBJS) $(LUAU_OBJS) -o $@
	@echo "LINK =>  $@"

clean:
	@rm -f $(APP_NAME) $(APP_OBJS)
	@find $(LUAU_DIR) -name "*.o" -type f -delete 2>/dev/null || true
	@echo "Cleaned all build artifacts"