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
