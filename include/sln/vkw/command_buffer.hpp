#pragma once
#include <sln/vkw/device.hpp>

// NOTE: Change get() function if it will ever hold more than one command buffer

/**
 * @brief Command Buffer class allocated from Command Pool, used to record
 * commands to to send for execution into the queue. Generally should be
 * created with sln::vkw::CommandPool member function, not manually.
 * @param Device it will operate on
 * @param Pool from which to allocate
 * @param Queue bits like Graphic or Compute
 * @param Is is primary or secondaru buffer?
 */
namespace sln::vkw {
class CommandBuffer {
public:
        CommandBuffer(const sln::vkw::Device& device,
                      const vk::CommandPool& pool,
                      const vk::Queue& queue, 
                      vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary);
        ~CommandBuffer();

        /**
         * @brief Reset command buffer to Initial state
         */
        void reset();

        /**
         * @brief Begin recording of the command buffer
         */
        void begin();

        /**
         * @brief End recording of the command buffer
         */
        void end();

        #warning Figure out what DST Stage Mask is
        /**
         * @brief Submit command buffer to queue specified at initialization
         * @param Semaphores to wait for before submit
         * @param Semaphores to signal after submit
         * @param I don't sure what that is...
         */
        void submit(const vk::ArrayProxyNoTemporaries<vk::Semaphore>& wait_for_semaphores = {},
                    const vk::ArrayProxyNoTemporaries<vk::Semaphore>& signal_semaphores = {},
                    const vk::ArrayProxyNoTemporaries<vk::PipelineStageFlags>& wait_dst_stage_mask = {},
                    const vk::Fence& fence = {});

        //std::vector<vk::CommandBuffer>& get() noexcept;
        //vk::CommandBuffer& get(int index) noexcept;
        vk::CommandBuffer& get() noexcept; // get(0)
        vk::CommandBuffer* operator->() noexcept;

private:
        const sln::vkw::Device& m_device;
        const vk::CommandPool& m_command_pool;
        const vk::Queue& m_queue;
        std::vector<vk::CommandBuffer> m_command_buffers; //actually max 1
};
} // namespace sln
