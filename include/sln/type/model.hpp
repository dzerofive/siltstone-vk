#pragma once

#include <vulkan/vulkan.hpp>
#include <sln/type/vertex.hpp>
#include <vector>

namespace sln {
class Model {
public:
        Model() {

        };
        ~Model() {};

        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;

        void bind(vk::CommandBuffer command_buffer);
        void draw(vk::CommandBuffer command_buffer);

private:
        std::vector<sln::Vertex> m_vertices;
        vk::Buffer m_vertex_buffer;
        vk::DeviceMemory m_vertex_memory;
        uint32_t m_vertex_count;
};
} // namespace sln
