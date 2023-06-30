#include <sln/vkw/command_pool.hpp>

sln::vkw::CommandBuffer sln::vkw::CommandPool::allocate_command_buffer(
        const vk::Queue& queue,
        vk::CommandBufferLevel level
) {
        return {m_device, m_command_pool, queue, level};
}


const vk::CommandPool& sln::vkw::CommandPool::get() const noexcept {
        return m_command_pool;
}


sln::vkw::CommandPool::CommandPool(const sln::vkw::Device& device) : m_device(device) {
        vk::CommandPoolCreateInfo pool_info{};
        pool_info.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        pool_info.queueFamilyIndex = device.graphic_family();
        m_command_pool = device->createCommandPool(pool_info);
}
