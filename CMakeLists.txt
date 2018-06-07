cmake_minimum_required(VERSION 3.1)

project(moviemaker-cpp)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)

# Search path for CMake include files.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(FFmpeg REQUIRED)
find_package(Numa REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(RSVG REQUIRED)
find_package(Threads REQUIRED)
find_package(x264 REQUIRED)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Manual build type selection (for debugging purposes)
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

# Enable C++ 11 support
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${FFMPEG_INCLUDE_DIRS})
include_directories(${GTK3_INCLUDE_DIRS})
include_directories(${RSVG_INCLUDE_DIR})
include_directories(${SWSCALE_INCLUDE_DIRS})
include_directories(${X264_INCLUDE_DIR})

file(GLOB_RECURSE LIBRARY_SRC "src/reader.cpp" "src/writer.cpp")

add_library(${PROJECT_NAME} SHARED ${LIBRARY_SRC})
target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES} ${X264_LIBRARIES} ${FFMPEG_LIBRARIES} ${SWSCALE_LIBRARIES} ${RSVG_LIBRARY} ${NUMA_LIBRARY})

# Enable ExternalProject CMake module
include(ExternalProject)

# Download and install GoogleTest
ExternalProject_Add(
	gtest
	GIT_REPOSITORY https://github.com/google/googletest.git
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
	# Disable install step
	INSTALL_COMMAND ""
)

# Get GTest source and binary directories from CMake project
ExternalProject_Get_Property(gtest source_dir binary_dir)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

# Set libgtest properties
set_target_properties(libgtest PROPERTIES
	"IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
	"IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}")

include_directories("${source_dir}/googletest/include")

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/data" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

file(GLOB_RECURSE TIGER_SVG "${CMAKE_CURRENT_BINARY_DIR}/data/tiger.svg")

add_executable(${PROJECT_NAME}_test "src/test.cpp")
target_compile_definitions(${PROJECT_NAME}_test PUBLIC TESTING)
target_compile_definitions(${PROJECT_NAME}_test PUBLIC TIGER_SVG="${TIGER_SVG}")
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} libgtest ${Thread_LIBRARIES})

