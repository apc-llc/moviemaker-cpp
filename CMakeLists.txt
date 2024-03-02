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

add_library(${PROJECT_NAME} SHARED "src/reader.cpp" "src/writer.cpp" )
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${GTK3_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${RSVG_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${SWSCALE_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${X264_INCLUDE_DIR})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES} ${X264_LIBRARIES} ${FFMPEG_LIBRARIES} ${SWSCALE_LIBRARIES} ${RSVG_LIBRARIES} ${NUMA_LIBRARIES})

if (NOT TARGET gtest)
# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory(ThirdParty/googletest EXCLUDE_FROM_ALL)
endif()

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/data" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

file(GLOB_RECURSE TIGER_SVG "${CMAKE_CURRENT_BINARY_DIR}/data/tiger.svg")

add_executable(${PROJECT_NAME}_test "src/test.cpp" "include/movie.h")
target_compile_definitions(${PROJECT_NAME}_test PUBLIC TESTING)
target_compile_definitions(${PROJECT_NAME}_test PUBLIC TIGER_SVG="${TIGER_SVG}")
set_property(TARGET ${PROJECT_NAME}_test PROPERTY CXX_STANDARD 11)
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} gtest ${Thread_LIBRARIES})

