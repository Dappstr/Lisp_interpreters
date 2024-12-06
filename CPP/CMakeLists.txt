cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    CPP_MAL
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the executable target
add_executable(mal
    	src/main.cpp
    	src/scanner.cpp
	src/parser.cpp
	src/ast.cpp
	src/evaluator.cpp
	src/environment.cpp
	src/builtins.cpp
)

# Add the include directory for the 'mal' target
target_include_directories(mal PRIVATE include)

