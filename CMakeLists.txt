cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(FileSystem CXX)
add_library(
	FileSystem STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/src/fs.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/file.cpp
)
target_include_directories(FileSystem
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src/
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include/
)

if(WIN32 OR CYGWIN)
	target_compile_definitions(FileSystem PUBLIC FS_TARGET_WINDOWS)
endif()

