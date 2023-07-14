#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <string>


namespace sln {
class Window {
public:
        Window(std::string title, uint32_t width, uint32_t height);

        bool should_close();
        void poll_events();

        GLFWwindow* get();
        uint32_t width();
        uint32_t height();
        std::string title();
        vk::Extent2D framebuffer_extent();

        bool is_resized();
        void reset_window_resized();
private:
        static void framebuffer_resize_callback(GLFWwindow* window, int width, int height);

        GLFWwindow* m_window;
        bool m_framebuffer_resized = false;
        uint32_t m_width;
        uint32_t m_height;
        std::string m_title;
};
} // namespace sln
