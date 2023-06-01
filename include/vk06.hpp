#pragma once

#include <vulkan/vulkan.hpp>

#include <sln/vkw/print_info.hpp> // Print Vulkan Wrapper info
#include <sln/print_info.hpp> // Print general SLN info

#include <sln/windowing_instance.hpp>
#include <sln/window.hpp>
#include <sln/surface.hpp>
#include <sln/vkw/instance.hpp>
#include <sln/vkw/physical_device.hpp>
#include <sln/vkw/device.hpp>
#include <sln/vkw/swapchain.hpp>
#include <sln/vkw/pipeline.hpp>

#include <sln/type/vertex.hpp>
#include <sln/type/model.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>


namespace sln {
    const std::string app_name = "D05's Vulkan Ventures Gen VI";

    class vk06 {
        public:
        vk06() {
                print_info(pdevice);
                print_info(window);

                std::vector<sln::Vertex> vertices = {
                        {{-1.f, -1.f, 0.f}, {1.f, 1.f, 1.f}},
                        {{-0.8f, -0.8f, 0.f}, {1.f, 0.f, 1.f}},
                        {{-1.f, -0.6f, 0.f}, {0.f, 0.f, 0.f}}
                };
                std::vector<sln::Vertex> vertices2 = {
                        {{0.f, -0.5f, 0.f}, {1.f, 0.f, 0.f}},
                        {{0.5f, 0.5f, 0.f}, {0.f, 1.f, 0.f}},
                        {{-0.5f, 0.5f, 0.f}, {0.f, 0.f, 1.f}},
                        {{0.f, 1.f, 0.f}, {1.f, 1.f, 1.f}},
                        {{-0.5f, 1.f, 0.f}, {0.5f, 0.5f, 0.5f}}
                };
                Model test_model(device, vertices);
                Model test_model2(device, vertices2);

                // Synchronization objects
                vk::Semaphore semaphore_image_available = device->createSemaphore({});
                vk::Semaphore semaphore_render_finished = device->createSemaphore({});
                vk::Fence in_flight = device->createFence({vk::FenceCreateFlagBits::eSignaled});
                // Command pool
                vk::CommandPoolCreateInfo pool_info{};
                pool_info.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
                pool_info.queueFamilyIndex = 0;
                vk::CommandPool command_pool = device->createCommandPool(pool_info);


                // Draw Buffer
                vk::CommandBufferAllocateInfo allocate_info{};
                allocate_info.commandPool = command_pool;
                allocate_info.level = vk::CommandBufferLevel::ePrimary;
                allocate_info.commandBufferCount = 1;

                vk::CommandBuffer command_buffer;
                auto res = device->allocateCommandBuffers(&allocate_info, &command_buffer);
                if(res != vk::Result::eSuccess) throw std::runtime_error("Failed to allocate command buffers");

                
                // Copy to device memory Buffer
                vk::CommandBufferAllocateInfo copy_allocate_info{};
                copy_allocate_info.commandPool = command_pool;
                copy_allocate_info.level = vk::CommandBufferLevel::ePrimary;
                copy_allocate_info.commandBufferCount = 1;
                vk::CommandBuffer copy_command_buffer;
                res = device->allocateCommandBuffers(&copy_allocate_info, &copy_command_buffer);

                vk::CommandBufferBeginInfo copy_begin_info{};
                copy_begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
                copy_command_buffer.begin(copy_begin_info);
                test_model.copy_to_device(copy_command_buffer);
                test_model2.copy_to_device(copy_command_buffer);
                copy_command_buffer.end();
                // Commence copying
                vk::SubmitInfo copy_submit_info;
                copy_submit_info.commandBufferCount = 1;
                copy_submit_info.pCommandBuffers = &copy_command_buffer;
                device.graphic_queue().submit(copy_submit_info, VK_NULL_HANDLE);        
                device.graphic_queue().waitIdle();
                device->freeCommandBuffers(command_pool, 1, &copy_command_buffer);

                while(!window.should_close()) {
                        window.poll_events();
                        

                        auto res = device->waitForFences(1, &in_flight, true, 100000000);
                        vk::resultCheck(res, "Fence");
                        res = device->resetFences(1, &in_flight);
                        vk::resultCheck(res, "Reset fence");
                        
                        uint32_t image_index = 0;
                        res = device->acquireNextImageKHR(swapchain.get(), 100000000, semaphore_image_available, VK_NULL_HANDLE, &image_index);
                        vk::resultCheck(res, "Acquire next image");

                        command_buffer.reset();
                        vk::CommandBufferBeginInfo buffer_begin_info{};
                        command_buffer.begin(buffer_begin_info);

                        // Blue-ish background
                        vk::ClearValue clear_color = vk::ClearValue(vk::ClearColorValue{std::array<float, 4>{0.1f, 0.1f, 0.14f, 1.f}});
                        vk::RenderPassBeginInfo render_pass_begin_info{};
                        render_pass_begin_info.framebuffer = pipeline.framebuffer(image_index); 
                        render_pass_begin_info.renderPass = pipeline.render_pass();
                        render_pass_begin_info.renderArea.offset = vk::Offset2D{0, 0};
                        render_pass_begin_info.renderArea.extent = swapchain.extent();
                        render_pass_begin_info.clearValueCount = 1;
                        render_pass_begin_info.pClearValues = &clear_color;
                        command_buffer.beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);
                        command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.get());

                        vk::Rect2D scissor{};
                        scissor.offset = vk::Offset2D{0, 0};
                        scissor.extent = swapchain.extent();
                        vk::Viewport viewport{};
                        viewport.x = 0.f;
                        viewport.y = 0.f;
                        viewport.width = swapchain.extent().width;
                        viewport.height = swapchain.extent().height;
                        viewport.minDepth = 0.f;
                        viewport.maxDepth = 1.f;
                        command_buffer.setScissor(0, 1, &scissor);
                        command_buffer.setViewport(0, 1, &viewport);

                        test_model.bind(command_buffer);
                        test_model.draw(command_buffer);
                        test_model2.bind(command_buffer);
                        test_model2.draw(command_buffer);

                        command_buffer.endRenderPass();
                        command_buffer.end();

                        vk::SubmitInfo submit_info{};
                        submit_info.waitSemaphoreCount = 1;
                        submit_info.pWaitSemaphores = &semaphore_image_available;
                        vk::PipelineStageFlags wait_stages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
                        submit_info.pWaitDstStageMask = wait_stages;
                        submit_info.commandBufferCount = 1;
                        submit_info.pCommandBuffers = &command_buffer;
                        submit_info.signalSemaphoreCount = 1;
                        submit_info.pSignalSemaphores = &semaphore_render_finished;
                        device.graphic_queue().submit(submit_info, in_flight); 

                        vk::PresentInfoKHR present_info{};
                        present_info.waitSemaphoreCount = 1;
                        present_info.pWaitSemaphores = &semaphore_render_finished;
                        present_info.swapchainCount = 1;
                        present_info.pSwapchains = &swapchain.get();
                        present_info.pImageIndices = &image_index;
                        res = device.graphic_queue().presentKHR(present_info);
                        vk::resultCheck(res, "Present");
                };
                device->waitIdle();
        };


    private: 
        sln::WindowingInstance windowing_instance{};

        std::vector<const char*> enabled_device_extensions{ "VK_KHR_swapchain" };
	std::vector<const char*> enabled_extensions = windowing_instance.get_instance_extensions();
        std::vector<const char*> enabled_layers{ "VK_LAYER_KHRONOS_validation" };

	sln::vkw::Instance instance{app_name, enabled_extensions, enabled_layers}; 
        sln::Window window{app_name, 1280, 720};
        sln::Surface surface{window, instance};
	sln::vkw::PhysicalDevice pdevice{instance, 0};
        sln::vkw::Device device{pdevice, enabled_device_extensions};
        sln::vkw::Swapchain swapchain{device, 
                                      pdevice,
                                      window, 
                                      surface, 
                                      vk::PresentModeKHR::eMailbox};
        sln::vkw::Pipeline pipeline{device, swapchain};


        vk::Buffer m_vertex_buffer;
        vk::DeviceMemory m_vertex_memory;
    };
}
