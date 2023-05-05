#include <sln/windowing_instance.hpp>

sln::WindowingInstance::WindowingInstance() {
        glfwInit();
}

std::vector<const char*> sln::WindowingInstance::get_instance_extensions() {
        uint32_t ext_count = 0;
        const char** exts = glfwGetRequiredInstanceExtensions(&ext_count);
        std::vector<const char*> vec(exts, exts+ext_count);
        return std::move(vec);
}
