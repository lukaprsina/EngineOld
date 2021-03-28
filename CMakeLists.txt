cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Engine VERSION 1.0)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

if (WIN32)
	set(ENG_PLATFORM ENG_PLATFORM_WINDOWS)
endif (WIN32)
if (UNIX)
	set(ENG_PLATFORM ENG_PLATFORM_UNIX)
endif (UNIX)

add_subdirectory("Engine/vendor/GLFW")
add_subdirectory("Engine/vendor/spdlog")
add_subdirectory("Engine/vendor/glm")
#add_subdirectory("EngineSDK")


project(EngineSDK VERSION 1.0)

add_library(EngineSDK STATIC
    "EngineSDK/src/Core/EngineSDK.cpp"
)

target_include_directories(EngineSDK
    PUBLIC ${CMAKE_SOURCE_DIR}/EngineSDK/src
    INTERFACE ${CMAKE_SOURCE_DIR}/EngineSDK/include
    INTERFACE ${CMAKE_SOURCE_DIR}/EngineSDK/src
)

target_compile_definitions(EngineSDK
    PUBLIC ${ENG_PLATFORM}
)

target_compile_options(EngineSDK
    PRIVATE -fPIC
)

add_subdirectory("Sandbox")
add_subdirectory("Engine")