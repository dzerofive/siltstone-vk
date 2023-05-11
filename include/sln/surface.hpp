#pragma once

#include <vulkan/vulkan.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <sln/vkw/instance.hpp>
#include <sln/window.hpp>

namespace sln {
class Surface {
public:
        Surface(sln::Window window, sln::vkw::Instance vkw_instance);
        vk::SurfaceKHR get();

private:
        vk::SurfaceKHR m_surface;
};
} // namespace sln
