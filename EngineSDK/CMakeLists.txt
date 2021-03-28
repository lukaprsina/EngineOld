project(EngineSDK VERSION 1.0)

add_library(EngineSDK
    "src/Core/EngineSDK.cpp"
)

target_include_directories(EngineSDK
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(EngineSDK
    PUBLIC ${ENG_PLATFORM}
)

target_compile_options(EngineSDK
    PRIVATE -fPIC
)