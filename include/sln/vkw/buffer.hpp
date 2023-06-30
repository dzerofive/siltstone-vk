#pragma once

#include <vulkan/vulkan.hpp>
#include <sln/vkw/device.hpp>
#include <sln/type/vertex.hpp>

namespace sln::vkw {
/**
 * @brief Base buffer class, intended to be inherited by specific buffers,
 * such as Host Memory Buffer (sln::HostBuffer)
 */
class Buffer {
public:
        Buffer(const sln::vkw::Device& device, 
               const vk::DeviceSize size, 
               const vk::BufferUsageFlags usage, 
               const vk::MemoryPropertyFlags memory_property_flags);
        ~Buffer();
        const vk::Buffer get() const noexcept;
        const vk::Buffer* operator->() const noexcept;

        const vk::Buffer buffer() const noexcept;
        const vk::DeviceMemory memory() const noexcept;
        
//protected:
        vk::Buffer m_buffer;
        vk::MemoryRequirements m_memory_req;
        vk::DeviceMemory m_memory;

        const sln::vkw::Device& m_device;
        const vk::DeviceSize m_buffer_size;
        const vk::BufferUsageFlags m_usage_flags;
};

class HostBuffer : public Buffer {
public:
        HostBuffer(const sln::vkw::Device& device, 
                   const vk::DeviceSize size, 
                   const vk::BufferUsageFlags usage);

//private:
        void copy_array_to_buffer(vk::ArrayProxy<sln::Vertex> data, vk::DeviceSize offset);
};
} // namespace sln::vkw
