#pragma once

#include <vulkan/vulkan.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>


namespace d05 {
    const std::string app_name = "D05's Vulkan Ventures Gen VI";

    class vk06 {
    public:
        vk06();


    private:
        std::vector<const char*> enabled_extensions{ "VK_KHR_swapchain" };
        std::vector<const char*> enabled_layers{ "VK_LAYER_KHRONOS_validation" };

	GLFWwindow* window;

        vk::Instance instance;
	vk::SurfaceKHR surface;
        vk::PhysicalDevice pdevice;
        vk::Device device;
	vk::SwapchainKHR swapchain;

	// Functions
	void set_up_devices();
	void run_loop();
    };
}
