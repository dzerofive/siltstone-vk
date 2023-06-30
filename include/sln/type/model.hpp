#pragma once
#include <vulkan/vulkan.hpp>

#include <sln/type/vertex.hpp>
#include <sln/vkw/device.hpp>
#include <sln/vkw/buffer.hpp>
#include <sln/util/find_memory_type.hpp>

#include <boost/log/trivial.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

namespace sln {

// TODO: Would be nice to refactor

class Model {
public:
        Model(sln::vkw::Device& device, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) 
        : m_device(device),
          m_vertex_buffer_size(vertices.size()*sizeof(vertices[0])),
          m_index_buffer_size(indices.size()*sizeof(indices[0])),
          m_vertex_buffer(device, 
                          m_vertex_buffer_size, 
                          vk::BufferUsageFlagBits::eVertexBuffer,
                          vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent),
          m_index_buffer(device,
                         m_index_buffer_size,
                         vk::BufferUsageFlagBits::eIndexBuffer,
                         vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible)
        {
                m_vertex_count = vertices.size();
                m_index_count = indices.size();

                // Those next two should be moved to HostBuffer class
                // Copy vertex buffer
                auto v_data = static_cast<sln::Vertex*>(m_device->mapMemory(m_vertex_buffer.m_memory, 0, m_vertex_buffer.m_buffer_size));
                std::copy_n(vertices.begin(), vertices.size(), v_data);
                m_device->unmapMemory(m_vertex_buffer.m_memory);
                // Copy index buffer
                auto i_data = static_cast<uint32_t*>(m_device->mapMemory(m_index_buffer.m_memory, 0, m_index_buffer.m_buffer_size));
                std::copy_n(indices.begin(), indices.size(), i_data);
                m_device->unmapMemory(m_index_buffer.m_memory);
        };
        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;

        void copy_to_device(vk::CommandBuffer command_buffer) {
                //vk::BufferCopy copy_region{};
                //copy_region.size = m_vertex_buffer_size;
                //command_buffer.copyBuffer(m_src_vertex_buffer, m_vertex_buffer, 1, &copy_region);
                //copy_region.size = m_index_buffer_size;
                //command_buffer.copyBuffer(m_src_index_buffer, m_index_buffer, 1, &copy_region);
                BOOST_LOG_TRIVIAL(warning) << "(UNIMPLEMENTED) Copying Host buffer to Device buffer";
        } 
        void bind(const vk::CommandBuffer& command_buffer) {
                vk::Buffer buffers[] = {m_vertex_buffer.buffer()};
                vk::DeviceSize offsets[] = {0};
                command_buffer.bindVertexBuffers(0, 1, buffers, offsets);
                command_buffer.bindIndexBuffer(m_index_buffer.buffer(), 0, vk::IndexType::eUint32);
        };
        void draw(const vk::CommandBuffer& command_buffer) {
                command_buffer.drawIndexed(m_index_count, 1, 0, 0, 0);
        };

private:
        const sln::vkw::Device& m_device;

        vk::DeviceSize m_vertex_buffer_size;
        vk::DeviceSize m_index_buffer_size;

        uint32_t m_vertex_count;
        uint32_t m_index_count;

        sln::vkw::Buffer m_vertex_buffer;
        sln::vkw::Buffer m_index_buffer;
};
} // namespace sln
