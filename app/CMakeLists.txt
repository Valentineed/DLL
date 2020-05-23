#APP

include(CMake/CMakeLists_app.cmake)
set(APP_TARGET app)
add_executable(${APP_TARGET} ${app_SOURCES})


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${app_SOURCES})

if(MSVC)
target_compile_options(${APP_TARGET} PRIVATE /W4 /WX)
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT output)
if(ipo_supported)
    message("--APP: IPO is supported!")
    set_property(TARGET ${APP_TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
else()
	message(WARNING "IPO is not supported: ${output}")
endif()


set(APP_DIR ${PROJECT_SOURCE_DIR}/app/source)
set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor)
set(GLFW3_ROOT ${VENDOR_DIR}/glfw3/)
set(GLFW_DLL ${PROJECT_SOURCE_DIR}/vendor/glfw3/lib/glfw3.dll)

#set in C++
set_source_files_properties(${GLAD_SOURCE} PROPERTIES LANGUAGE CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMake/")
find_package(GLFW3 REQUIRED)
							
target_include_directories(${APP_TARGET}
							PRIVATE 
							${APP_DIR}
							${VENDOR_DIR}/glfw3/include
							${GLFW3_INCLUDE_DIR}
							)
							
target_link_libraries(${APP_TARGET} PRIVATE ${GLFW3_LIBRARY} lib)

#copie the dll and deplace
add_custom_command(TARGET ${APP_TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GLFW_DLL} $<TARGET_FILE_DIR:${APP_TARGET}>)


#start the project in my target
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${APP_TARGET})

set_property(TARGET ${APP_TARGET} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")