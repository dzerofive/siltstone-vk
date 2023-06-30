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
#include <sln/vkw/command_pool.hpp>

#include <sln/type/vertex.hpp>
#include <sln/type/model.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>


namespace sln {
    const std::string app_name = "D05's Vulkan Ventures Gen VI";

    class vk06 {
        public:
        vk06() {
                //print_info(pdevice);
                //print_info(window);
                std::vector<sln::Vertex> vertices = {
                        {{-1.f, -1.f, 0.f}, {1.f, 1.f, 1.f}},
                        {{-0.8f, -0.8f, 0.f}, {1.f, 0.f, 1.f}},
                        {{-1.f, -0.6f, 0.f}, {0.f, 0.f, 0.f}},
                        {{-1.f, -0.6f, -0.5f}, {0.f, 1.f, 0.f}}
                };
                std::vector<uint32_t> indices = {
                        2, 0, 1, 2, 1, 3
                };
                std::vector<sln::Vertex> vertices2 = {
                        {{-0.5f, -0.5f, 0.f}, {1.f, 0.f, 0.f}},
                        {{0.5f, -0.5f, 0.f}, {0.f, 1.f, 0.f}},
                        {{-0.5f, 0.5f, 0.f}, {0.f, 0.f, 1.f}},
                        {{0.5f, 0.5f, 0.f}, {1.f, 1.f, 1.f}},
                        {{-0.5f, -0.5f, 0.f}, {1.f, 0.f, 0.f}},
                };
                std::vector<uint32_t> indices2 = {
                        0, 1, 2, 2, 1, 3, 2, 1, 4 
                };
                Model test_model(device, vertices, indices);
                Model test_model2(device, vertices2, indices2);

                // Synchronization objects
                std::vector<vk::Semaphore> semaphore_image_available = { device->createSemaphore({}) };
                std::vector<vk::Semaphore> semaphore_render_finished = { device->createSemaphore({}) };
                vk::Fence in_flight = device->createFence({vk::FenceCreateFlagBits::eSignaled});
                
                sln::vkw::CommandPool command_pool(device); 
                auto command_buffer = command_pool.allocate_command_buffer(device.graphic_queue());

                while(!window.should_close()) {
                        // Poll GLFW events (exit event)
                        window.poll_events();
                        
                        auto res = device->waitForFences(in_flight, true, 1000000000);
                        vk::resultCheck(res, "Fence");
                        device->resetFences(in_flight);
                        
                        uint32_t image_index = 0;
                        res = device->acquireNextImageKHR(swapchain.get(), 1000000000, semaphore_image_available[0], VK_NULL_HANDLE, &image_index);
                        vk::resultCheck(res, "Acquire next image");

                        std::vector<vk::PipelineStageFlags> wait_stages = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

                        command_buffer.reset();
                        command_buffer.begin();

                        // Blue-ish background
                        vk::ClearValue clear_color = vk::ClearValue(vk::ClearColorValue{std::array<float, 4>{0.1f, 0.1f, 0.14f, 1.f}});
                        vk::RenderPassBeginInfo render_pass_begin_info{};
                        render_pass_begin_info.framebuffer = pipeline.framebuffer(image_index); 
                        render_pass_begin_info.renderPass = pipeline.render_pass();
                        render_pass_begin_info.renderArea.offset = vk::Offset2D{0, 0};
                        render_pass_begin_info.renderArea.extent = swapchain.extent();
                        render_pass_begin_info.clearValueCount = 1;
                        render_pass_begin_info.pClearValues = &clear_color;
                        command_buffer->beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);
                        command_buffer->bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.get());

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
                        command_buffer->setScissor(0, 1, &scissor);
                        command_buffer->setViewport(0, 1, &viewport);

                        test_model.bind(command_buffer.get());
                        test_model.draw(command_buffer.get());
                        test_model2.bind(command_buffer.get());
                        test_model2.draw(command_buffer.get());

                        command_buffer->endRenderPass();
                        command_buffer.end();

                        command_buffer.submit(semaphore_image_available, semaphore_render_finished, wait_stages, in_flight);

                        vk::PresentInfoKHR present_info{};
                        present_info.setWaitSemaphores(semaphore_render_finished);
                        present_info.setSwapchains(swapchain.get());
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
