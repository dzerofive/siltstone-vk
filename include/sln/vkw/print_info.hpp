#pragma once

#include <sln/util/log_prefixes.hpp>
#include <iostream>

#include "physical_device.hpp"
#include "swapchain.hpp"

namespace sln {
	void print_info(const sln::vkw::PhysicalDevice& pdevice);
        void print_info(const sln::vkw::Swapchain& swapchain);

}
