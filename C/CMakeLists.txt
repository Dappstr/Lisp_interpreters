cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
	C_MAL
	VERSION 1.0.0
	LANGUAGES C
)

add_executable(mal
		src/main.c
	)
cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Suppress warnings globally for deprecated and "unsafe" functions
target_compile_definitions(mal PRIVATE
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
)

# Apply compiler-specific flags
if(MSVC)
    # For Microsoft Visual C++
    target_compile_options(mal PRIVATE /wd4996)
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    # For GCC or Clang
    target_compile_options(mal PRIVATE -Wno-deprecated-declarations)
endif()

# Include directories (if needed)
# target_include_directories(my_executable PRIVATE include/)
