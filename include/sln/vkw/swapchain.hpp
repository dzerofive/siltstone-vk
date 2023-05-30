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
        
        const vk::SwapchainKHR& get() const noexcept;
        const vk::SwapchainKHR* operator->() const noexcept;
        
        const vk::SurfaceFormatKHR& format() const noexcept;
        const vk::Extent2D& extent() const noexcept;
        const std::vector<vk::ImageView> image_views() const noexcept;
        const vk::ImageView image_view(size_t index) const noexcept;

private:
        vk::SurfaceCapabilitiesKHR m_surface_capabilities; 
        vk::SwapchainKHR m_swapchain;

        std::vector<vk::SurfaceFormatKHR> m_surface_formats;
        std::vector<vk::ImageView> m_image_views;
        vk::Extent2D m_framebuffer_extent;
        vk::SurfaceFormatKHR m_surface_format;
};
} // namespace sln::vkw
