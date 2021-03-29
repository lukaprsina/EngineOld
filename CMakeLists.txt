cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Engine VERSION 1.0)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

#if (POLICY CMP0077)
#    cmake_policy(SET CMP0077 NEW)
#endif()

if (WIN32)
	set(ENG_PLATFORM ENG_PLATFORM_WINDOWS)
endif (WIN32)
if (UNIX)
	set(ENG_PLATFORM ENG_PLATFORM_UNIX)
endif (UNIX)

add_subdirectory("Engine/vendor/GLFW")
add_subdirectory("Engine/vendor/spdlog")
add_subdirectory("Engine/vendor/glm")
add_subdirectory("EngineSDK")
add_subdirectory("Sandbox")
add_subdirectory("Engine")

install(
    TARGETS EngineSDK 
    )