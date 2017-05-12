cmake_minimum_required(VERSION 3.1)

# Include headers
include_directories("include")

# Collect and specify platform-specific source files
file(GLOB tssend_srcs
    "src/main.cpp"
    "src/*.cpp"
)
file(GLOB tssend_headers
    "include/*.h"
)

# Configure this sub-project as executable
add_executable(ts_sender 
    ${tssend_srcs}
    ${tssend_headers}
)

# Set executable name
set_target_properties(ts_sender PROPERTIES
    OUTPUT_NAME "ts_sender" 
)

# Link shared library
target_link_libraries(ts_sender shared)

# Set up Visual Studio source groups
set( REG_EXT "[^/]*([.]cpp|[.]c|[.]h|[.]hpp)$")
source_group("include"  REGULAR_EXPRESSION ".*/include/${REG_EXT}")
source_group("src"      REGULAR_EXPRESSION ".*/src/${REG_EXT}")