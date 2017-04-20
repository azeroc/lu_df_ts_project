cmake_minimum_required(VERSION 3.1)
project (ts_project)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# We don't need ZERO_CHECK
set(CMAKE_SUPPRESS_REGENERATION true)

add_subdirectory(shared)
add_subdirectory(ts_receiver)
add_subdirectory(ts_sender)