add_executable(Engine
    "src/Core/Application.cpp"
    "src/Core/Log.cpp"
    "src/Scene/Scene.cpp"    
    
    "src/VulkanAPI/VulkanAPI.cpp"
    "src/VulkanAPI/IsSubset.cpp"
    "src/VulkanAPI/VMA.cpp"

    "src/VulkanAPI/Instance.cpp"
    "src/VulkanAPI/Device.cpp"
    "src/VulkanAPI/SwapChain.cpp"
    "src/VulkanAPI/GraphicsPipeline.cpp"
    "src/VulkanAPI/Framebuffers.cpp"
    "src/VulkanAPI/Commands.cpp"
    "src/VulkanAPI/SyncObjects.cpp"
    "src/VulkanAPI/DrawFrame.cpp"
    "src/VulkanAPI/Buffers.cpp"
    "src/VulkanAPI/Descriptors.cpp"

    "src/GLFW/GLFW.cpp"
    "src/GLFW/SetCallbacks.cpp"    
    )

find_package(Vulkan REQUIRED)

set(gcc_like_compile "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_compile "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

target_include_directories(Engine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC ${CMAKE_SOURCE_DIR}/EngineSDK/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/VMA/src
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/libtool/libltdl
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include
    PRIVATE ${Vulkan_INCLUDE_DIRS}    
    )

target_precompile_headers(Engine
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h
	)
    
target_compile_options(Engine
    PRIVATE "$<${gcc_like_compile}:$<BUILD_INTERFACE:-Wall;-Wextra;-pedantic>>"
    PRIVATE "$<${msvc_compile}:$<BUILD_INTERFACE:-W3>>"
    )

target_link_libraries(Engine
    PRIVATE glfw
    PRIVATE spdlog::spdlog_header_only
    PRIVATE glm    
    PRIVATE ${Vulkan_LIBRARIES}
    )

file(GLOB SHADER_SRC
    assets/shaders/*.spv
)

target_compile_definitions(Engine
    PUBLIC ${ENG_PLATFORM}
    )

foreach(shaderpath ${SHADER_SRC})
    get_filename_component(shader ${shaderpath} NAME)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/${shader} ${CMAKE_BINARY_DIR}/assets/shaders/${shader} COPYONLY)
endforeach()