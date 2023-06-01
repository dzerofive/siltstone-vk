#pragma once

#include <vulkan/vulkan.hpp>

#include <sln/type/vertex.hpp>
#include <sln/vkw/device.hpp>

#include <vector>
#include <algorithm>

namespace sln {
class Model {
public:
        Model(sln::vkw::Device& device, const std::vector<Vertex>& vertices) : m_device(device) {
                m_vertex_count = vertices.size();
                assert(m_vertex_count >= 3 && "Vertex count must >= 3");

                vk::BufferCreateInfo src_buffer_info;
                src_buffer_info.size = sizeof(vertices[0])*m_vertex_count;
                src_buffer_info.usage = vk::BufferUsageFlagBits::eTransferSrc;
                src_buffer_info.sharingMode = vk::SharingMode::eExclusive;
                m_src_vertex_buffer = m_device->createBuffer(src_buffer_info);

                vk::MemoryAllocateInfo src_allocate_info;
                uint32_t src_allocate_size = device->getBufferMemoryRequirements(m_src_vertex_buffer).size;
                src_allocate_info.allocationSize = src_allocate_size;
                src_allocate_info.memoryTypeIndex = 2;
                m_src_vertex_memory = device->allocateMemory(src_allocate_info);

                m_device->bindBufferMemory(m_src_vertex_buffer, m_src_vertex_memory, 0);


                Vertex* data = static_cast<Vertex*>(device->mapMemory(m_src_vertex_memory, 0, src_allocate_size));
                std::copy_n(vertices.begin(), vertices.size(), data);
                device->unmapMemory(m_src_vertex_memory);

                vk::BufferCreateInfo dst_buffer_info;
                dst_buffer_info.size = sizeof(vertices[0])*m_vertex_count;
                dst_buffer_info.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer;
                dst_buffer_info.sharingMode = vk::SharingMode::eExclusive;
                m_vertex_buffer = m_device->createBuffer(dst_buffer_info);

                vk::MemoryAllocateInfo dst_allocate_info;
                uint32_t dst_allocate_size = device->getBufferMemoryRequirements(m_vertex_buffer).size;
                m_buffer_size = dst_allocate_size;
                dst_allocate_info.allocationSize = dst_allocate_size;
                dst_allocate_info.memoryTypeIndex = 0;
                m_vertex_memory = device->allocateMemory(dst_allocate_info);

                m_device->bindBufferMemory(m_vertex_buffer, m_vertex_memory, 0);
        };
        ~Model() {
                m_device->freeMemory(m_vertex_memory);
                m_device->destroyBuffer(m_vertex_buffer);
        };

        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;

        void copy_to_device(vk::CommandBuffer command_buffer) {
                vk::BufferCopy copy_region{};
                copy_region.size = m_buffer_size;
                command_buffer.copyBuffer(m_src_vertex_buffer, m_vertex_buffer, 1, &copy_region);
        } 
        void bind(vk::CommandBuffer command_buffer) {
                vk::Buffer buffers[] = {m_vertex_buffer};
                vk::DeviceSize offsets[] = {0};
                command_buffer.bindVertexBuffers(0, 1, buffers, offsets);
        };
        void draw(vk::CommandBuffer command_buffer) {
                command_buffer.draw(m_vertex_count, 1, 0, 0);
        };

private:
        sln::vkw::Device& m_device;
        std::vector<sln::Vertex> m_vertices;

        vk::Buffer m_src_vertex_buffer;
        vk::DeviceMemory m_src_vertex_memory;
        vk::Buffer m_vertex_buffer;
        vk::DeviceMemory m_vertex_memory;

        uint32_t m_buffer_size;
        uint32_t m_vertex_count;
};
} // namespace sln
