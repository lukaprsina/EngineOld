project(EngineSDK VERSION 1.0)

add_library(EngineSDK
    "src/Core/EngineSDK.cpp"
)

target_include_directories(EngineSDK
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include    
)

target_compile_definitions(EngineSDK
    PUBLIC ${ENG_PLATFORM}
)

set_property(TARGET EngineSDK PROPERTY POSITION_INDEPENDENT_CODE ON)