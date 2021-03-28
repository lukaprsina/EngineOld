project(Sandbox VERSION 1.0)

add_library(Sandbox SHARED
    "src/Core/Sandbox.cpp"
)

target_include_directories(Sandbox
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE ${CMAKE_SOURCE_DIR}/EngineSDK/include
)


target_compile_definitions(EngineSDK
    PUBLIC ${ENG_PLATFORM}
    )

target_link_libraries(Sandbox
    PRIVATE EngineSDK
)