cmake_minimum_required(VERSION 3.1)

# Include headers
include_directories("include")

# Collect and specify platform-specific source files
file(GLOB tssend_srcs
    "src/main.cpp"
    "include/*.h"
    "src/*.cpp"
)
file(GLOB tssend_linux_impl
    "include/linux_impl/*.h"
    "src/linux_impl/*.cpp"
)
# Configure this sub-project as executable
if (UNIX)
    add_executable(ts_sender 
        ${tssend_srcs}
        ${tssend_linux_impl}
    )
elseif(WIN32)
    add_executable(ts_sender 
        ${tssend_srcs}
    )
else()
    message( FATAL_ERROR "This platform is not supported for ts_receiver" )
endif()

# Set executable name
set_target_properties(ts_sender PROPERTIES
    OUTPUT_NAME "ts_sender" 
)

# Add configuration file
configure_file("dist/ts_sender.ini.dist" "${CMAKE_BINARY_DIR}/build/ts_sender.ini" COPYONLY)

# Link shared library
target_link_libraries(ts_sender shared)

# Set up Visual Studio source groups
set( REG_EXT "[^/]*([.]cpp|[.]c|[.]h|[.]hpp)$")
source_group("include"                REGULAR_EXPRESSION ".*/include/${REG_EXT}")
source_group("include\\windows_impl"  REGULAR_EXPRESSION ".*/include/windows_impl/${REG_EXT}")
source_group("src"                    REGULAR_EXPRESSION ".*/src/${REG_EXT}")
source_group("src\\windows_impl"      REGULAR_EXPRESSION ".*/src/windows_impl/${REG_EXT}")
