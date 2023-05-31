#pragma once

#include <glm/vec3.hpp>
#include <vulkan/vulkan.hpp>

#include <array>

namespace sln {
struct Vertex {
        glm::vec3 position;
        glm::vec3 color;

        static vk::VertexInputBindingDescription binding_description() {
                vk::VertexInputBindingDescription binding_desc{};
                binding_desc.binding = 0;
                binding_desc.stride = sizeof(sln::Vertex);
                binding_desc.inputRate = vk::VertexInputRate::eVertex;

                return binding_desc;
        }
        static std::array<vk::VertexInputAttributeDescription, 2> attribute_descriptions() {
                std::array<vk::VertexInputAttributeDescription, 2> attribute_desc{};
                attribute_desc[0].binding = 0;
                attribute_desc[0].location = 0;
                attribute_desc[0].format = vk::Format::eR32G32B32Sfloat;
                attribute_desc[0].offset = 0;

                attribute_desc[1].binding = 0;
                attribute_desc[1].location = 1;
                attribute_desc[1].format = vk::Format::eR32G32B32Sfloat;
                attribute_desc[1].offset = sizeof(sln::Vertex::position);

                return attribute_desc;
        }
};
} // namespace sln
