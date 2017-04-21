cmake_minimum_required(VERSION 3.1)
project (ts_project)

# Set up build directory for binary output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build)

# Import Boost libraries
find_package(Boost REQUIRED)
add_library(boost INTERFACE IMPORTED)
set_property(TARGET boost PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})

# We don't need ZERO_CHECK
set(CMAKE_SUPPRESS_REGENERATION true)

add_subdirectory(shared)
add_subdirectory(ts_receiver)
add_subdirectory(ts_sender)
