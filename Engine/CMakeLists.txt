add_executable(Engine
    "src/Core/Application.cpp"
    
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

    "src/GLFW/GLFW.cpp"
    "src/GLFW/GLFWSetCallbacks.cpp"
    )

target_include_directories(Engine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include
    )

target_precompile_headers(Engine
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h
	)

target_link_libraries(Engine
    PRIVATE glfw
    PRIVATE vulkan
    )