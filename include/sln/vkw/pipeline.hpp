#pragma once

#include <vulkan/vulkan.hpp>
#include <sln/vkw/device.hpp>
#include <sln/vkw/swapchain.hpp>
#include <sln/vkw/spv_shader.hpp>

namespace sln::vkw {
class Pipeline {
public:
        Pipeline(const sln::vkw::Device& device, const sln::vkw::Swapchain& swapchain);
        const vk::Pipeline& get() const noexcept;
        const vk::Pipeline* operator->() const noexcept;

private:
        inline void create_render_pass(const sln::vkw::Device& device, const sln::vkw::Swapchain& swapchain);

        vk::Pipeline m_pipeline;
        vk::RenderPass m_renderpass;

};
} // namespace sln::vkw
