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
class Model {
public:
        Model(sln::vkw::Device& device, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) 
        : m_device(device),
          m_vertex_buffer_size(vertices.size()*sizeof(vertices[0])),
          m_index_buffer_size(indices.size()*sizeof(indices[0])),
          m_buffer(device, m_vertex_buffer_size+m_index_buffer_size, 
                          vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eIndexBuffer) {
                m_vertex_count = vertices.size();
                assert(m_vertex_count >= 3 && "Vertex count must >= 3");
                m_index_count = indices.size();
                assert(m_index_count >= 3 && "Index count must be >= 3");

                m_buffer.copy_array_to_buffer(nullptr, 0);
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
                vk::Buffer buffers[] = {m_buffer.buffer()};
                vk::DeviceSize offsets[] = {0};
                command_buffer.bindVertexBuffers(0, 1, buffers, offsets);
                command_buffer.bindIndexBuffer(m_buffer.buffer(), m_vertex_buffer_size, vk::IndexType::eUint32);
        };
        void draw(const vk::CommandBuffer& command_buffer) {
                command_buffer.drawIndexed(m_vertex_count, static_cast<uint32_t>(m_index_count), 1, 0, 0);
        };

private:
        const sln::vkw::Device& m_device;

        vk::DeviceSize m_vertex_buffer_size;
        vk::DeviceSize m_index_buffer_size;

        uint32_t m_vertex_count;
        uint32_t m_index_count;

        sln::vkw::HostBuffer m_buffer;
};
} // namespace sln
