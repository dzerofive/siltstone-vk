#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>


namespace sln {
class Window {
public:
        Window(std::string title, uint32_t width, uint32_t height);

        GLFWwindow* window();
        uint32_t width();
        uint32_t height();
        std::string title();

private:
        GLFWwindow* m_window;

        uint32_t m_width;
        uint32_t m_height;
        std::string m_title;
};
} // namespace sln
