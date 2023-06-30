#pragma once

#include <vulkan/vulkan.hpp>
#include <sln/vkw/physical_device.hpp>

#include <vector>

namespace sln::vkw {
class Device {
public:
        Device(const sln::vkw::PhysicalDevice physical_device, std::vector<const char*> extensions);
        const vk::Device& get() const noexcept;
        const vk::Device* operator->() const noexcept;

        const vk::Queue& graphic_queue() const noexcept;
        const uint32_t graphic_family() const noexcept;
        const sln::vkw::PhysicalDevice& parent_device() const noexcept;
private:	
        vk::Queue m_graphic_queue;
        uint32_t m_graphic_family;
        vk::Device m_device;
        const sln::vkw::PhysicalDevice m_parent_device;
};
} // namespace d05
