cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(FILESYSTEM_BUILD_TESTS "Build tests" ON)

project(FileSystem CXX)
add_library(
	FileSystem STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/src/fs.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/file.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/helper.cpp
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

if(FILESYSTEM_BUILD_TESTS)
	project(FileSystem_Test CXX)
	add_executable(FileSystem_Test ${CMAKE_CURRENT_SOURCE_DIR}/test/test.cpp)
	target_include_directories(FileSystem_Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test/minunit/)
	target_link_libraries(FileSystem_Test PUBLIC FileSystem)
	if(UNIX AND NOT APPLE)
		target_link_libraries(FileSystem_Test PUBLIC rt)
	endif()
	if(WIN32 OR CYGWIN)
		target_compile_definitions(FileSystem_Test PUBLIC TEST_TARGET_WINDOWS)
	endif()
endif()

