cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(vk6)

add_executable(vk6 main.cpp devices.cpp loop.cpp vk06.cpp)
target_include_directories(vk6 PUBLIC include)
target_link_libraries(vk6 PUBLIC vulkan glfw)
