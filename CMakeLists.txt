cmake_minimum_required(VERSION 3.1)
project (ts_project)

# Set up build directory for binary output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build)

# Set required C++ standard
set(CMAKE_CXX_STANDARD 11)

# Import Boost libraries
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost REQUIRED COMPONENTS system)
add_library(boost INTERFACE IMPORTED)
set_property(TARGET boost PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})

## To fix auto-linking of date_time or regex in VC++
add_definitions(-DBOOST_DATE_TIME_NO_LIB)
add_definitions(-DBOOST_REGEX_NO_LIB)

# We don't need ZERO_CHECK
set(CMAKE_SUPPRESS_REGENERATION true)

# Set compiler flags depending on OS
if (WIN32)
    message( STATUS "Setting MSVC flags" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHc" )
elseif(UNIX)
    message( STATUS "Setting GCC flags" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11 -fexceptions -g -Wall" )
endif()

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

# Add ts_project sub-projects
add_subdirectory(shared)
add_subdirectory(ts_receiver)
add_subdirectory(ts_sender)
