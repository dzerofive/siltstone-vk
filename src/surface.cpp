#include <sln/surface.hpp>

sln::Surface::Surface(sln::Window window, sln::vkw::Instance vkw_instance) { 
        VkSurfaceKHR surf{};
        VkResult res = glfwCreateWindowSurface(vkw_instance.get(), window.get(), nullptr, &surf);
        m_surface = surf;
}

vk::SurfaceKHR sln::Surface::get() {
        return m_surface;
}


