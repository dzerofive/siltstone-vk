#include <sln/vkw/swapchain.hpp>
#include <algorithm>
#include <iostream>

inline vk::Extent2D get_glfw_extent(GLFWwindow* window) {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        return vk::Extent2D{static_cast<uint32_t>(w), static_cast<uint32_t>(h)};
}

sln::vkw::Swapchain::Swapchain(sln::vkw::Device         device, 
                               sln::vkw::PhysicalDevice p_device,
                               sln::Window              window,
                               sln::Surface             surface,
                               vk::PresentModeKHR       present_mode) {
        m_surface_capabilities = p_device.get().getSurfaceCapabilitiesKHR(surface.get());
        m_surface_formats = p_device.get().getSurfaceFormatsKHR(surface.get());
        m_surface_format = m_surface_formats[0];
        m_framebuffer_extent = window.framebuffer_extent();

        uint32_t q = 0; // queue family index
        vk::SwapchainCreateInfoKHR swap_info{};
        swap_info.surface = surface.get();
        swap_info.minImageCount = std::max(uint32_t{2}, m_surface_capabilities.minImageCount);

        swap_info.imageFormat = m_surface_format.format;
        swap_info.imageColorSpace = m_surface_format.colorSpace;
        swap_info.imageExtent = m_framebuffer_extent;
        swap_info.imageArrayLayers = 1;
        swap_info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
        swap_info.imageSharingMode = vk::SharingMode::eExclusive;

        swap_info.queueFamilyIndexCount = 1;
        swap_info.pQueueFamilyIndices = &q; 
        swap_info.preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
        swap_info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        swap_info.presentMode = present_mode;
        swap_info.clipped = true;
        swap_info.oldSwapchain = VK_NULL_HANDLE;

        m_swapchain = device->createSwapchainKHR(swap_info);

        auto images = device->getSwapchainImagesKHR(m_swapchain);
        vk::ImageViewCreateInfo image_view_info{};
        image_view_info.format = vk::Format::eB8G8R8A8Unorm;
        image_view_info.viewType = vk::ImageViewType::e2D;
        image_view_info.subresourceRange = {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1};
        for(const auto& i : images) {
                image_view_info.image = i;
                m_image_views.push_back(device->createImageView(image_view_info));
        }
}

const vk::SwapchainKHR& sln::vkw::Swapchain::get() const noexcept {
        return m_swapchain;
}
const vk::SwapchainKHR* sln::vkw::Swapchain::operator->() const noexcept {
        return &m_swapchain;
}
const vk::SurfaceFormatKHR& sln::vkw::Swapchain::format() const noexcept {
        return m_surface_format;
}
const vk::Extent2D& sln::vkw::Swapchain::extent() const noexcept {
        return m_framebuffer_extent; 
}
const std::vector<vk::ImageView>& sln::vkw::Swapchain::image_views() const noexcept {
        return m_image_views;
}
const vk::ImageView& sln::vkw::Swapchain::image_view(size_t index) const noexcept {
        return m_image_views[index];
}
