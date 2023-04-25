#include "vk06.hpp"

// Table of Content
// - Creating instance
// - Creating surface
// - Picking physical device
// - Picking logical device
// - Creating swapchain

void d05::vk06::set_up_devices() {
	glfwInit();

	// Creating instance
	vk::ApplicationInfo app_info{};
	app_info.pApplicationName = app_name.c_str();
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "No Engine";
	app_info.engineVersion = 0;
	app_info.apiVersion = VK_API_VERSION_1_3;

	uint32_t glfwExtCount = 0;
	vk::InstanceCreateInfo instance_info{};
	instance_info.pApplicationInfo = &app_info;
	instance_info.ppEnabledExtensionNames = glfwGetRequiredInstanceExtensions(&glfwExtCount);
	instance_info.enabledExtensionCount = glfwExtCount;
	instance_info.enabledLayerCount = enabled_layers.size();
	instance_info.ppEnabledLayerNames = enabled_layers.data();

	std::cout << "GLFW exts:\n";
	for(int i = 0; i < glfwExtCount; ++i) {
		std::cout << " - " << instance_info.ppEnabledExtensionNames[i] << '\n';
	}

	instance = vk::createInstance(instance_info);
	std::cout << "Created instance successfully!\n";
	

	// Create window
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(1280, 720, app_name.c_str(), nullptr, nullptr);


	// Surface creation
	VkSurfaceKHR temp_surface;
	glfwCreateWindowSurface(instance, window, nullptr, &temp_surface);
	surface = temp_surface;


	// Picking physical device
	std::vector<vk::PhysicalDevice> physical_devices = instance.enumeratePhysicalDevices();
	pdevice = physical_devices[0];
	auto dv = pdevice.getProperties().driverVersion;
	std::cout << "Picked device is " << pdevice.getProperties().deviceName << '\n'
	      << " - Driver version " << VK_VERSION_MAJOR(dv) << '.' 
	      << VK_VERSION_MINOR(dv) << '.' << VK_VERSION_PATCH(dv) << '\n';
	// List memory heaps and types
	auto heaps_count = pdevice.getMemoryProperties().memoryHeapCount;
	auto mem_heaps = pdevice.getMemoryProperties().memoryHeaps;
	auto types_count = pdevice.getMemoryProperties().memoryTypeCount;
	auto mem_types = pdevice.getMemoryProperties().memoryTypes;
	std::cout << " - Has " << heaps_count << " Heaps:\n";
	for(int i = 0; i < heaps_count; i++)
		std::cout << " --- [" << i << "] of size " <<  mem_heaps[i].size << ' ' 
				  << vk::to_string(mem_heaps[i].flags) << '\n';
	std::cout << " - And " << types_count << " Types:\n";
	for(int i = 0; i < types_count; i++)
		std::cout << " --- [" << i << "] for heap index " << mem_types[i].heapIndex << ' ' 
		          << vk::to_string(mem_types[i].propertyFlags) << '\n';
	// List queue families
	std::cout << " - With following Queue Families:\n";
	auto q_family_properties = pdevice.getQueueFamilyProperties();
	for(int i = 0; i < q_family_properties.size(); i++)
		std::cout << " --- [" << i << "] with " << q_family_properties[i].queueCount 
		          << " queue(s) " << vk::to_string(q_family_properties[i].queueFlags) 
			  << " with " << q_family_properties[i].timestampValidBits 
			  << " valid timestamp bits \n";


	// Picking logical device
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
	device_info.enabledExtensionCount = enabled_extensions.size();
	device_info.ppEnabledExtensionNames = enabled_extensions.data();
	device = pdevice.createDevice(device_info);
	
	std::cout << "Both Physical and Logical deivces created successfully!\n";


	vk::SurfaceCapabilitiesKHR capabilities;
	capabilities = pdevice.getSurfaceCapabilitiesKHR(surface);
	std::cout << " - Physical device has minimum of " << capabilities.minImageCount
		  << " images, max of " << capabilities.maxImageCount
		  << "\n - Minimun extent of " << capabilities.minImageExtent.width << 'x' << capabilities.minImageExtent.height
		  << ", max of " << capabilities.maxImageExtent.width << 'x' << capabilities.maxImageExtent.height
		  << "\n - Current extent of " << capabilities.currentExtent.width << 'x' << capabilities.currentExtent.height << '\n';
	int glfw_w, glfw_h = 0;
	glfwGetFramebufferSize(window, &glfw_w, &glfw_h);
	std::cout << " - GLFW framebuffer size " << glfw_w << 'x' << glfw_h << '\n';

	std::vector<vk::SurfaceFormatKHR> formats;
	uint32_t format_count;
	pdevice.getSurfaceFormatsKHR(surface, &format_count, nullptr);
	formats.resize(format_count);
	formats = pdevice.getSurfaceFormatsKHR(surface);
	for(int i = 0; i < format_count; ++i) {
		std::cout << " - Format " << i << " color space " << vk::to_string(formats[i].colorSpace) 
			  << "\n - Format " << vk::to_string(formats[i].format) << '\n';
	}

	std::vector<vk::PresentModeKHR> present_modes;
	uint32_t present_count;
	pdevice.getSurfacePresentModesKHR(surface, &present_count, nullptr);
	present_modes.resize(present_count);
	present_modes = pdevice.getSurfacePresentModesKHR(surface);
	for(int i = 0; i < present_count; ++i) {
		std::cout << " - Present mode " << vk::to_string(present_modes[i]) << '\n';
	}

	// Swapchain creation
	vk::SwapchainCreateInfoKHR swapchain_info{};
	swapchain_info.surface = surface;
	swapchain_info.minImageCount = std::max((uint32_t)2, capabilities.minImageCount);
	swapchain_info.imageFormat = formats[0].format;
	swapchain_info.imageColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
	swapchain_info.imageExtent = capabilities.currentExtent;
	swapchain_info.imageArrayLayers = 1;
	swapchain_info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

	swapchain_info.imageSharingMode = vk::SharingMode::eExclusive;
	swapchain_info.queueFamilyIndexCount = 0; // Unused on RX580
	swapchain_info.pQueueFamilyIndices = nullptr; // ^ same
	swapchain_info.preTransform = capabilities.currentTransform;
	swapchain_info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	swapchain_info.presentMode = vk::PresentModeKHR::eMailbox;
	swapchain_info.clipped = true;
	swapchain_info.oldSwapchain = VK_NULL_HANDLE;

	swapchain = device.createSwapchainKHR(swapchain_info);

	std::cout << "Swapchain created successfully!\n";
}


