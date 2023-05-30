cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(vk6)

add_executable(vk6 
        main.cpp 
        src/windowing_instance.cpp
	src/vkw/instance.cpp
        src/window.cpp
        src/surface.cpp
	src/vkw/physical_device.cpp
        src/vkw/device.cpp
        src/vkw/swapchain.cpp
        src/vkw/pipeline.cpp
        src/vkw/spv_shader.cpp

        src/vkw/print_info.cpp
        src/print_info.cpp)
target_include_directories(vk6 PUBLIC include)
target_link_libraries(vk6 PUBLIC vulkan glfw)
