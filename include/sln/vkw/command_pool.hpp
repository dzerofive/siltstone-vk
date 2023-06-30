#pragma once

#include <sln/vkw/device.hpp>
#include <sln/vkw/command_buffer.hpp>

namespace sln::vkw {
/** 
 * @brief Command Pool class from whom command buffers are allocated
 * @param Device to create command pool on
 */
class CommandPool {
public:
        CommandPool(const sln::vkw::Device& device);
        CommandPool(const CommandPool &) = delete;
        CommandPool &operator=(const CommandPool &) = delete;

        const vk::CommandPool& get() const noexcept; 
        sln::vkw::CommandBuffer allocate_command_buffer(
                const vk::Queue& queue,
                vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary
        );

private:
        vk::CommandPool m_command_pool;
        const sln::vkw::Device& m_device;
};
} // namespace sln::vkw
