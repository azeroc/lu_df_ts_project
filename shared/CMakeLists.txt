cmake_minimum_required(VERSION 3.1)

# Collect and specify platform-specific source files
file(GLOB shared_srcs
    "src/*.cpp"
)
file(GLOB shared_headers
    "include/*.h"
)

# Configure this sub-project as library
add_library(shared 
    ${shared_srcs}
    ${shared_headers}
)

# Setup headers for use by the library itself and projects what link to this library.
target_include_directories(shared 
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE 
        src)
        
# Link to boost compiled component libraries and the header-only library
target_link_libraries(shared
    ${Boost_LIBRARIES}
    boost)

# Set up Visual Studio source groups
set( REG_EXT "[^/]*([.]cpp|[.]c|[.]h|[.]hpp)$")
source_group("include"  REGULAR_EXPRESSION ".*/include/${REG_EXT}")
source_group("src"      REGULAR_EXPRESSION ".*/src/${REG_EXT}")