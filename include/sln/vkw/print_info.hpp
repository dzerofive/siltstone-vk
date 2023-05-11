#pragma once

#include <sln/log_prefixes.hpp>
#include <iostream>

#include "physical_device.hpp"
#include "swapchain.hpp"

namespace sln {
	void print_info(const sln::vkw::PhysicalDevice& pdevice);
        void print_info(const sln::vkw::Swapchain& swapchain);

}
