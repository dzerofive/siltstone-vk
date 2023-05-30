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
private:	
        vk::Queue m_graphic_queue;
        vk::Device m_device;

};
} // namespace d05
