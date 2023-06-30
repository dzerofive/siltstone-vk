#include <sln/vkw/buffer.hpp>

#include <sln/util/find_memory_type.hpp>

#include <boost/log/trivial.hpp>

#include <iostream>
#include <typeinfo>

sln::vkw::Buffer::Buffer(const sln::vkw::Device& device, 
                         const vk::DeviceSize size, 
                         const vk::BufferUsageFlags usage, 
                         const vk::MemoryPropertyFlags memory_property_flags) 
        : m_device(device), m_buffer_size(size), m_usage_flags(usage) {
        // Create buffer
        vk::BufferCreateInfo create_info;
        create_info.size = m_buffer_size;
        create_info.usage = m_usage_flags;
        create_info.sharingMode = vk::SharingMode::eExclusive;
        m_buffer = m_device->createBuffer(create_info);
        // Get memory requirements size, as it may differ from array size
        m_memory_req = m_device->getBufferMemoryRequirements(m_buffer).size;

        // Allocate memory
        vk::MemoryAllocateInfo allocate_info;
        allocate_info.allocationSize = m_memory_req.size;
        allocate_info.memoryTypeIndex = find_memory_type(m_device.parent_device(), memory_property_flags);
        m_memory = m_device->allocateMemory(allocate_info);
        // Bind memory and buffer
        m_device->bindBufferMemory(m_buffer, m_memory, 0);

        BOOST_LOG_TRIVIAL(trace) << "Created buffer of size " << m_memory_req.size << " bytes";
}
sln::vkw::Buffer::~Buffer() {
        m_device->destroyBuffer(m_buffer);
        BOOST_LOG_TRIVIAL(trace) << "Destroyed buffer";
}

// Getters
const vk::DeviceMemory sln::vkw::Buffer::memory() const noexcept { return m_memory; }
const vk::Buffer sln::vkw::Buffer::buffer() const noexcept { return m_buffer; }

sln::vkw::HostBuffer::HostBuffer(const sln::vkw::Device& device,
                                 const vk::DeviceSize size,
                                 const vk::BufferUsageFlags usage)
        : Buffer(device, size, usage, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent) {
        BOOST_LOG_TRIVIAL(trace) << "Buffer memory type: Host";
}
// Needs to be separate from constructor because offset may be used
// to bind several arrays
void sln::vkw::HostBuffer::copy_array_to_buffer(const vk::ArrayProxy<sln::Vertex> data, const vk::DeviceSize offset) {
        void* data_ptr;
        auto result = m_device.get().mapMemory(m_memory, offset, m_buffer_size, vk::MemoryMapFlags(), &data_ptr);
        if(result != vk::Result::eSuccess) throw std::runtime_error("Failed to map memory for Host Buffer!");
        //std::copy_n()
        m_device.get().unmapMemory(m_memory);

        BOOST_LOG_TRIVIAL(trace) << "Copied " << m_buffer_size << " bytes of type " << typeid(sln::Vertex).name();
}
