#LIBGFX

include(CMake/CMakeLists_lib.cmake)
set(GRAPH_TARGET lib)

add_library(${GRAPH_TARGET} SHARED ${lib_SOURCES})


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${lib_SOURCES})

include(GenerateExportHeader)
generate_export_header(${GRAPH_TARGET}
			BASE_NAME ${GRAPH_TARGET}
			EXPORT_MACRO_NAME ${GRAPH_TARGET}_API
			EXPORT_FILE_NAME ${GRAPH_TARGET}_Export.h
			STATIC_DEFINE ${GRAPH_TARGET}_STATIC
)

if(MSVC)
target_compile_options(${GRAPH_TARGET} PRIVATE /W4 /WX)
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT output)
if(ipo_supported)
    message("--LIBGFX: IPO is supported!")
    set_property(TARGET ${GRAPH_TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
else()
	message(WARNING "IPO is not supported: ${output}")
endif()

set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor)
set(LIBGFX_DIR ${PROJECT_SOURCE_DIR}/libgfx/source)
set(GLAD_SOURCE ${VENDOR_DIR}/glad45/src/glad.c)

#set in C++
set_source_files_properties(${GLAD_SOURCE} PROPERTIES LANGUAGE CXX)

#--------------------------------------------------------GRAPH_TARGET
target_include_directories(${GRAPH_TARGET} 
							PUBLIC
							${CMAKE_CURRENT_BINARY_DIR}
							${VENDOR_DIR}/STB
							${LIBGFX_DIR}
							${VENDOR_DIR}/glad45/include
							${VENDOR_DIR}/glad45/include/glad)

target_sources(${GRAPH_TARGET} PRIVATE ${GLAD_SOURCE})

set_property(TARGET ${GRAPH_TARGET} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")