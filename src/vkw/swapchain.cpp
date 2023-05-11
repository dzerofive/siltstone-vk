#include <sln/vkw/swapchain.hpp>
#include <algorithm>

vk::Extent2D get_glfw_extent(GLFWwindow* window) {
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

        uint32_t q = 0;
        vk::SwapchainCreateInfoKHR swap_info{};
        swap_info.surface = surface.get();
        swap_info.minImageCount = std::max(uint32_t{2}, m_surface_capabilities.minImageCount);

        swap_info.imageFormat = m_surface_format.format;
        swap_info.imageColorSpace = m_surface_format.colorSpace;
        swap_info.imageExtent = get_glfw_extent(window.get());
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

        m_swapchain = device.get().createSwapchainKHR(swap_info);
}
