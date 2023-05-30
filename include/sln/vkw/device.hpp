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

private:	
        vk::Device m_device;

};
} // namespace d05
