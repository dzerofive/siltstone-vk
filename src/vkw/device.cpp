#include <sln/vkw/device.hpp>

sln::vkw::Device::Device(const sln::vkw::PhysicalDevice pdevice, std::vector<const char*> extensions) {
	std::vector<vk::DeviceQueueCreateInfo> queue_infos;

	vk::DeviceQueueCreateInfo queue_info{};
	float priority = 1.0f;
	queue_info.pQueuePriorities = &priority;
	queue_info.queueCount = 1;
	queue_info.queueFamilyIndex = 0;

	queue_infos.push_back(queue_info);

	vk::DeviceCreateInfo device_info{};
	device_info.pQueueCreateInfos = queue_infos.data();
	device_info.queueCreateInfoCount = 1;
	device_info.enabledExtensionCount = extensions.size();
	device_info.ppEnabledExtensionNames = extensions.data();
	
        m_device = pdevice.get().createDevice(device_info);

        m_graphic_queue = m_device.getQueue(0, 0);
}

const vk::Device& sln::vkw::Device::get() const noexcept {
        return m_device;
}
const vk::Device* sln::vkw::Device::operator->() const noexcept {
        return &m_device;
}
const vk::Queue& sln::vkw::Device::graphic_queue() const noexcept {
        return m_graphic_queue;
}
