#include <sln/vkw/physical_device.hpp>

sln::vkw::PhysicalDevice::PhysicalDevice(sln::vkw::Instance instance, int index) {
	m_pdevices = instance.get().enumeratePhysicalDevices();
	// Public method to pick from this vector ^
	pick(index);
}

void sln::vkw::PhysicalDevice::pick(int index) {
	m_pdevice = m_pdevices[index];
}
vk::PhysicalDevice& sln::vkw::PhysicalDevice::get() noexcept {
	return m_pdevice;
}
const vk::PhysicalDevice& sln::vkw::PhysicalDevice::get() const noexcept {
	return m_pdevice;
}
const size_t sln::vkw::PhysicalDevice::get_count() const noexcept {
        return m_pdevices.size();
}
