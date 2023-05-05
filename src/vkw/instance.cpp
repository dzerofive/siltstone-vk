#include <sln/vkw/instance.hpp>

sln::vkw::Instance::Instance(std::string app_name, 
	  		     std::vector<const char*> extensions, 
			     std::vector<const char*> layers) {
	vk::ApplicationInfo app_info{};
	app_info.pApplicationName = app_name.c_str();
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "No Engine";
	app_info.engineVersion = 0;
	app_info.apiVersion = VK_API_VERSION_1_3;

	uint32_t glfwExtCount = 0;
	vk::InstanceCreateInfo instance_info{};
	instance_info.pApplicationInfo = &app_info;
	instance_info.enabledExtensionCount = extensions.size();
	instance_info.ppEnabledExtensionNames = extensions.data();
	instance_info.enabledLayerCount = layers.size();
	instance_info.ppEnabledLayerNames = layers.data();

	m_instance = vk::createInstance(instance_info);
}

vk::Instance& sln::vkw::Instance::get() {
	return m_instance;
}
