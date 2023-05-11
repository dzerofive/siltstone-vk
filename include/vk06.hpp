#pragma once

#include <vulkan/vulkan.hpp>

#include <sln/vkw/print_info.hpp> // Print Vulkan Wrapper info
#include <sln/print_info.hpp> // Print general SLN info

#include <sln/windowing_instance.hpp>
#include <sln/window.hpp>
#include <sln/surface.hpp>
#include <sln/vkw/instance.hpp>
#include <sln/vkw/physical_device.hpp>
#include <sln/vkw/device.hpp>
#include <sln/vkw/swapchain.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <iostream>


namespace sln {
    const std::string app_name = "D05's Vulkan Ventures Gen VI";

    class vk06 {
        public:
        vk06() {
                print_info(pdevice);
                print_info(window);

                uint32_t q_index{1};
                vk::BufferCreateInfo buff_info{};
                buff_info.size = 3*1024*1024;
                buff_info.usage = vk::BufferUsageFlagBits::eUniformBuffer;
                buff_info.sharingMode = vk::SharingMode::eExclusive;
                buff_info.queueFamilyIndexCount = 1;
                buff_info.pQueueFamilyIndices = &q_index;
                auto buffer = device.get().createBuffer(buff_info);
                std::cout << sln::prefix::INFO << "Test buffer object type - " << vk::to_string(buffer.debugReportObjectType) << '\n';

                while(!window.should_close()) {
                        window.poll_events();
                };
        };


    private: 
        sln::WindowingInstance windowing_instance{};

        std::vector<const char*> enabled_device_extensions{ "VK_KHR_swapchain" };
	std::vector<const char*> enabled_extensions = windowing_instance.get_instance_extensions();
        std::vector<const char*> enabled_layers{ "VK_LAYER_KHRONOS_validation" };

	sln::vkw::Instance instance{app_name, enabled_extensions, enabled_layers}; 
        sln::Window window{app_name, 1280, 720};
        sln::Surface surface{window, instance};
	sln::vkw::PhysicalDevice pdevice{instance, 0};
        sln::vkw::Device device{pdevice, enabled_device_extensions};
        sln::vkw::Swapchain swapchain{device, 
                                      pdevice,
                                      window, 
                                      surface, 
                                      vk::PresentModeKHR::eMailbox};
    };
}
