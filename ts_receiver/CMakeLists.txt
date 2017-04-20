cmake_minimum_required(VERSION 3.1)

# Include headers
include_directories("include")

# Collect and specify platform-specific source files
file(GLOB tsrecv_srcs
    "src/Main.cpp"
    "src/*.cpp"
    "src/*.h"
)
file(GLOB linux_handler_src
	"src/handlers/linux_implementation/*.cpp"
)
file(GLOB win_handler_src
	"src/handlers/win_implementation/*.cpp"
)
file(GLOB tsrecv_headers
    "include/*.h"
    "include/handlers/*.h"
)

if (UNIX)
	add_executable(ts_receiver 
        ${tsrecv_srcs}
        ${linux_handler_src}
        ${tsrecv_headers}
    )
	
	message( STATUS "Setting GCC flags" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -g -Wall" )
elseif(WIN32)
	add_executable(ts_receiver 
        ${tsrecv_srcs}
        ${win_handler_src}
        ${tsrecv_headers}
    )
	
	# Set up Visual Studio source groups
	set( REG_EXT "[^/]*([.]cpp|[.]c|[.]h|[.]hpp)$")
	source_group("include" 				                REGULAR_EXPRESSION ".*/include/${REG_EXT}")
    source_group("include\\handlers" 				    REGULAR_EXPRESSION ".*/include/handlers/${REG_EXT}")
    source_group("src" 				                    REGULAR_EXPRESSION ".*/src/${REG_EXT}")
	source_group("src\\handlers" 	                    REGULAR_EXPRESSION ".*/src/handlers/${REG_EXT}")
    source_group("src\\handlers\\win_implementation" 	REGULAR_EXPRESSION ".*/src/handlers/win_implementation/${REG_EXT}")

	message( STATUS "Setting MSVC flags" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHc-" )
else()
	message( FATAL_ERROR "This platform is not supported for ts_receiver" )
endif()

# Link common library
target_link_libraries(ts_receiver shared)

message( STATUS "** CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}" )