add_executable(Engine
    "src/Core/Application.cpp"
    "src/Core/Log.cpp"
    
    "src/Vulkan/VulkanAPI.cpp"
    "src/Vulkan/Instance.cpp"
    "src/Vulkan/Cleanup.cpp"
    "src/Vulkan/Extensions.cpp"
    "src/Vulkan/ValidationLayers.cpp"
    "src/Vulkan/IsSubset.cpp"
    "src/Vulkan/DebugMessenger.cpp"
    "src/Vulkan/PhysicalDevice.cpp"
    "src/Vulkan/QueueFamilies.cpp"
    "src/Vulkan/LogicalDevice.cpp"
    "src/Vulkan/SwapChain.cpp"
    "src/Vulkan/ImageViews.cpp"
    "src/Vulkan/GraphicsPipeline.cpp"
    "src/Vulkan/RenderPass.cpp"
    "src/Vulkan/Framebuffers.cpp"
    "src/Vulkan/CommandPool.cpp"
    "src/Vulkan/DrawFrame.cpp"
    "src/Vulkan/Semaphores.cpp"

    "src/GLFW/GLFW.cpp"
    "src/GLFW/SetCallbacks.cpp"    
    )

set(gcc_like_compile "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_compile "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

target_include_directories(Engine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include
    )

target_precompile_headers(Engine
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h
	)
    
target_compile_options(Engine
    PRIVATE "$<${gcc_like_compile}:$<BUILD_INTERFACE:-Wall;-Wextra;-pedantic>>"
    PRIVATE "$<${msvc_compile}:$<BUILD_INTERFACE:-W3>>"
    )

target_include_directories(Engine
    PRIVATE "$<${msvc_compile}:C:/VulkanSDK/1.2.170.0/Include>"
    )

target_link_directories(Engine
    PRIVATE "$<${msvc_compile}:C:/VulkanSDK/1.2.170.0/Bin>"
    PRIVATE "$<${msvc_compile}:C:/VulkanSDK/1.2.170.0/Lib>"
    )

target_link_libraries(Engine
    PRIVATE glfw
    PRIVATE spdlog::spdlog_header_only
    PRIVATE vulkan
    )

file(GLOB SHADER_SRC
    assets/shaders/*.spv
)

foreach(shaderpath ${SHADER_SRC})
    get_filename_component(shader ${shaderpath} NAME)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/${shader} ${CMAKE_BINARY_DIR}/assets/shaders/${shader} COPYONLY)
endforeach()