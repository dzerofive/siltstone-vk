#pragma once

#include <GLFW/glfw3.h>
#include <vector>

namespace sln {
class WindowingInstance {
public:
        WindowingInstance();
        std::vector<const char*> get_instance_extensions();

};
} // namespace sln
