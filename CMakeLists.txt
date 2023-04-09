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
target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES} ${X264_LIBRARIES} ${FFMPEG_LIBRARIES} ${SWSCALE_LIBRARIES} ${RSVG_LIBRARIES} ${NUMA_LIBRARIES})

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory(ThirdParty/googletest EXCLUDE_FROM_ALL)

include_directories("${gtest_SOURCE_DIR}/include")
include_directories("${gmock_SOURCE_DIR}/include")

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/data" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

file(GLOB_RECURSE TIGER_SVG "${CMAKE_CURRENT_BINARY_DIR}/data/tiger.svg")

add_executable(${PROJECT_NAME}_test "src/test.cpp")
target_compile_definitions(${PROJECT_NAME}_test PUBLIC TESTING)
target_compile_definitions(${PROJECT_NAME}_test PUBLIC TIGER_SVG="${TIGER_SVG}")
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} gtest ${Thread_LIBRARIES})

