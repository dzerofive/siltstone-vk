#pragma once

#include <glm/mat4x4.hpp>

namespace sln {
struct UBO
{
        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 model;
};

} // namespace sln
