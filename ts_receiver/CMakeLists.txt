cmake_minimum_required(VERSION 3.1)

# Include headers
include_directories("include")

# Collect and specify platform-specific source files
file(GLOB tsrecv_srcs
    "src/main.cpp"
    "include/*.h"
    "src/*.cpp"
)
file(GLOB tsrecv_linux_impl
    "include/linux_impl/*.h"
    "src/linux_impl/*.cpp"
)
file(GLOB tsrecv_windows_impl
    "include/windows_impl/*.h"
    "src/windows_impl/*.cpp"
)

if (UNIX)
	add_executable(ts_receiver 
        ${tsrecv_srcs}
        ${tsrecv_linux_impl}
    )
elseif(WIN32)
	add_executable(ts_receiver 
        ${tsrecv_srcs}
        ${tsrecv_windows_impl}
    )
else()
	message( FATAL_ERROR "This platform is not supported for ts_receiver" )
endif()

# Set executable name
set_target_properties(ts_receiver PROPERTIES
    OUTPUT_NAME "ts_receiver"
)

# Link common library
target_link_libraries(ts_receiver shared)

# Set up Visual Studio source groups
set( REG_EXT "[^/]*([.]cpp|[.]c|[.]h|[.]hpp)$")
source_group("include"                REGULAR_EXPRESSION ".*/include/${REG_EXT}")
source_group("include\\windows_impl"  REGULAR_EXPRESSION ".*/include/windows_impl/${REG_EXT}")
source_group("src"                    REGULAR_EXPRESSION ".*/src/${REG_EXT}")
source_group("src\\windows_impl"      REGULAR_EXPRESSION ".*/src/windows_impl/${REG_EXT}")
