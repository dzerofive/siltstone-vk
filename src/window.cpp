#include <sln/window.hpp>

sln::Window::Window(std::string title, uint32_t width, uint32_t height)
        : m_title(title), m_width(width), m_height(height) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(m_window, this);
        glfwSetFramebufferSizeCallback(m_window, framebuffer_resize_callback);
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
void sln::Window::reset_window_resized() {
        m_framebuffer_resized = false;
}
void sln::Window::framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
        auto w = reinterpret_cast<sln::Window*>(glfwGetWindowUserPointer(window));
        w->m_framebuffer_resized = true;
        w->m_width = width;
        w->m_height = height;
}

GLFWwindow* sln::Window::get() { return m_window; }
std::string sln::Window::title() { return m_title; }
uint32_t sln::Window::height() { return m_height; }
uint32_t sln::Window::width() { return m_width; }
bool sln::Window::is_resized() { return m_framebuffer_resized; }
