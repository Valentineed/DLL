cmake_minimum_required(VERSION 3.16.3)

#Create C++ project
set(PROJECT_NAME "DLLProject")
project(${PROJECT_NAME} CXX)

#C++ Version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(libgfx/)
add_subdirectory(app/)

target_compile_definitions(app PRIVATE "GLFW_INCLUDE_NONE")


