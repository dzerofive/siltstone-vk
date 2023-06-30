#include <sln/vkw/command_buffer.hpp>

// NOTE: One Time Submit (line 27) may not be always suitable, maybe 
// should make it configurable from outside

// Transport settings to CreateInfo
sln::vkw::CommandBuffer::CommandBuffer(const sln::vkw::Device& device, 
                             const vk::CommandPool& pool, 
                             const vk::Queue& queue, 
                             vk::CommandBufferLevel level) 
: m_device(device), m_command_pool(pool), m_queue(queue) {
        vk::CommandBufferAllocateInfo info;
        info.level = level;
        info.commandPool = m_command_pool;
        info.commandBufferCount = 1; // here we say "max 1"
        m_command_buffers = m_device->allocateCommandBuffers(info);
}

// Free buffer
sln::vkw::CommandBuffer::~CommandBuffer() {
        m_device->freeCommandBuffers(m_command_pool, m_command_buffers.size(), m_command_buffers.data()); 
}

// Begin recording
void sln::vkw::CommandBuffer::begin() {
        vk::CommandBufferBeginInfo info;
        info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        m_command_buffers[0].begin(info);
}
// End recording
void sln::vkw::CommandBuffer::end() {
        m_command_buffers[0].end();
}
// Submit to queue
void sln::vkw::CommandBuffer::submit(const vk::ArrayProxyNoTemporaries<vk::Semaphore>& wait_for_semaphores,
                                     const vk::ArrayProxyNoTemporaries<vk::Semaphore>& signal_semaphores,
                                     const vk::ArrayProxyNoTemporaries<vk::PipelineStageFlags>& wait_dst_stage_mask,
                                     const vk::Fence& fence) {
        vk::SubmitInfo info;
        info.setCommandBuffers(m_command_buffers);
        info.setWaitSemaphores(wait_for_semaphores);
        info.setSignalSemaphores(signal_semaphores);
        info.setWaitDstStageMask(wait_dst_stage_mask);
        m_queue.submit(info, fence);
}
// Reset to initial state
void sln::vkw::CommandBuffer::reset() {
        m_command_buffers[0].reset();
}

vk::CommandBuffer& sln::vkw::CommandBuffer::get() noexcept { 
        return m_command_buffers[0];
}
vk::CommandBuffer* sln::vkw::CommandBuffer::operator->() noexcept {
        return &m_command_buffers[0];
}
