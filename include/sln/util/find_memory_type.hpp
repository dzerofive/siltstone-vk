#pragma once

#include <vulkan/vulkan.hpp>
#include <sln/vkw/physical_device.hpp>

namespace sln {
        inline uint32_t find_memory_type(const sln::vkw::PhysicalDevice& physical_device, 
                                  const vk::MemoryPropertyFlags& flags_to_find) {
                auto mem_properties = physical_device.get().getMemoryProperties();

                for(int i = 0; i < mem_properties.memoryTypeCount; ++i) {
                        if((mem_properties.memoryTypes[i].propertyFlags & flags_to_find) == flags_to_find) {
                                return i;
                        }
                }

                throw std::runtime_error("Cannot find memory with requested flags");
        };
}
