#pragma once

#include <vulkan/vulkan.hpp>

#include <sln/vkw/device.hpp>
#include <sln/vkw/physical_device.hpp>
#include <sln/window.hpp>
#include <sln/surface.hpp>

namespace sln::vkw {
class Swapchain {
public:
        Swapchain(sln::vkw::Device device, 
                  sln::vkw::PhysicalDevice p_device,
                  sln::Window window, 
                  sln::Surface surface, 
                  vk::PresentModeKHR present_mode);
        void get();

private:
        vk::SurfaceCapabilitiesKHR m_surface_capabilities; 
        vk::SwapchainKHR m_swapchain;

        std::vector<vk::SurfaceFormatKHR> m_surface_formats;
        vk::SurfaceFormatKHR m_surface_format;
};
} // namespace sln::vkw
