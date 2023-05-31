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

                vk::BufferCreateInfo buffer_info;
                buffer_info.size = sizeof(vertices[0])*m_vertex_count;
                buffer_info.usage = vk::BufferUsageFlagBits::eVertexBuffer;
                buffer_info.sharingMode = vk::SharingMode::eExclusive;
                m_vertex_buffer = m_device->createBuffer(buffer_info);

                vk::MemoryAllocateInfo allocate_info;
                uint32_t allocate_size = device->getBufferMemoryRequirements(m_vertex_buffer).size;
                allocate_info.allocationSize = allocate_size;
                allocate_info.memoryTypeIndex = 2;
                m_vertex_memory = device->allocateMemory(allocate_info);

                m_device->bindBufferMemory(m_vertex_buffer, m_vertex_memory, 0);

                Vertex* data = static_cast<Vertex*>(device->mapMemory(m_vertex_memory, 0, allocate_size));
                std::copy_n(vertices.begin(), vertices.size(), data);
                device->unmapMemory(m_vertex_memory);
        };
        ~Model() {
                m_device->freeMemory(m_vertex_memory);
                m_device->destroyBuffer(m_vertex_buffer);
        };

        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;

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
        vk::Buffer m_vertex_buffer;
        vk::DeviceMemory m_vertex_memory;
        uint32_t m_vertex_count;
};
} // namespace sln
