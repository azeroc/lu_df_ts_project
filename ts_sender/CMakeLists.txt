cmake_minimum_required(VERSION 3.1)

# Include headers
include_directories("include")

# Collect and specify platform-specific source files
file(GLOB tssend_srcs
    "src/Main.cpp"
    "src/*.cpp"
    "src/*.h"
)
file(GLOB tssend_headers
    "include/*.h"
)

# Configure this sub-project as executable
add_executable(ts_sender 
    ${tssend_srcs}
    ${tssend_headers}
)

# Link shared library
target_link_libraries(ts_sender shared)

# Set up Visual Studio source groups
set( REG_EXT "[^/]*([.]cpp|[.]c|[.]h|[.]hpp)$")
source_group("include"  REGULAR_EXPRESSION ".*/include/${REG_EXT}")
source_group("src"      REGULAR_EXPRESSION ".*/src/${REG_EXT}")