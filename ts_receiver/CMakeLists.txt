cmake_minimum_required(VERSION 3.1)

# Include headers
include_directories("include")

# Collect and specify platform-specific source files
file(GLOB tsrecv_srcs
    "src/main.cpp"
    "src/*.cpp"
)
file(GLOB linux_src
	"src/linux/*.cpp"
)
file(GLOB win_src
	"src/windows/*.cpp"
)
file(GLOB tsrecv_headers
    "include/*.h"
)

if (UNIX)
	add_executable(ts_receiver 
        ${tsrecv_srcs}
        ${linux_src}
        ${tsrecv_headers}
    )
elseif(WIN32)
	add_executable(ts_receiver 
        ${tsrecv_srcs}
        ${win_src}
        ${tsrecv_headers}
    )
	
	# Set up Visual Studio source groups
	set( REG_EXT "[^/]*([.]cpp|[.]c|[.]h|[.]hpp)$")
	source_group("include" 				                REGULAR_EXPRESSION ".*/include/${REG_EXT}")
    source_group("src" 				                    REGULAR_EXPRESSION ".*/src/${REG_EXT}")
    source_group("src\\windows" 	                    REGULAR_EXPRESSION ".*/src/windows/${REG_EXT}")
else()
	message( FATAL_ERROR "This platform is not supported for ts_receiver" )
endif()

# Set executable name
set_target_properties(ts_receiver PROPERTIES
    OUTPUT_NAME "ts_receiver"
)

# Link common library
target_link_libraries(ts_receiver shared)