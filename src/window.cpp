#include <sln/window.hpp>

sln::Window::Window(std::string title, uint32_t width, uint32_t height)
        : m_title(title), m_width(width), m_height(height) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}

bool sln::Window::should_close() {
        return glfwWindowShouldClose(m_window);
}
void sln::Window::poll_events() {
        glfwPollEvents();
}
vk::Extent2D sln::Window::framebuffer_extent() {
        int x, y;
        glfwGetFramebufferSize(m_window, &x, &y);
        return vk::Extent2D{static_cast<uint32_t>(x), static_cast<uint32_t>(y)};
}

GLFWwindow* sln::Window::get() { return m_window; }
std::string sln::Window::title() { return m_title; }
uint32_t sln::Window::height() { return m_height; }
uint32_t sln::Window::width() { return m_width; }
