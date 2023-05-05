#pragma once

#include <vulkan/vulkan.hpp>
#include <sln/vkw/instance.hpp>

namespace sln::vkw {
class PhysicalDevice {
public:
	PhysicalDevice(sln::vkw::Instance instance, int index = 0);
	void pick(int index);

	vk::PhysicalDevice& get() noexcept;
        const vk::PhysicalDevice& get() const noexcept;
        const size_t get_count() const noexcept;

private:	
        vk::PhysicalDevice m_pdevice;
	std::vector<vk::PhysicalDevice> m_pdevices;
};
} // namespace sln
