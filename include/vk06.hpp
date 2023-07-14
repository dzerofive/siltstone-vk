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
#include <sln/type/uniform_buffer_object.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

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
                


                vk::DescriptorSetLayoutBinding dset_layout_binding;
                dset_layout_binding.descriptorType = vk::DescriptorType::eUniformBuffer;
                dset_layout_binding.descriptorCount = swapchain.image_views().size();
                dset_layout_binding.binding = 0;
                dset_layout_binding.stageFlags = vk::ShaderStageFlagBits::eVertex;
                std::vector<vk::DescriptorSetLayoutBinding> dset_layout_bindings;
                dset_layout_bindings.push_back(dset_layout_binding);
                
                vk::DescriptorSetLayoutCreateInfo dset_layout_info;
                dset_layout_info.setBindings(dset_layout_bindings);
                
                std::vector<vk::DescriptorSetLayout> dset_layouts;
                //for(const auto& i : swapchain.image_views()) {
                        auto l = device->createDescriptorSetLayout(dset_layout_info);
                        dset_layouts.push_back(l);
                //}


                sln::UBO ubo;
                sln::vkw::Buffer ubo_buffer(device, 
                                            sizeof(ubo), 
                                            vk::BufferUsageFlagBits::eUniformBuffer, 
                                            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
                auto ubo_ptr = static_cast<sln::UBO*>(device->mapMemory(ubo_buffer.memory(), 0, sizeof(ubo)));

                std::cout << "Swapchain image view count: " << swapchain.image_views().size() << '\n';
                vk::DescriptorPoolSize d_pool_size;
                d_pool_size.descriptorCount = swapchain.image_views().size();
                d_pool_size.type = vk::DescriptorType::eUniformBuffer; 

                vk::DescriptorPoolCreateInfo d_pool_info;
                d_pool_info.maxSets = swapchain.image_views().size();
                d_pool_info.setPoolSizes(d_pool_size);
                auto descriptor_pool = device->createDescriptorPool(d_pool_info);

                vk::DescriptorSetAllocateInfo dset_allocate_info;
                dset_allocate_info.descriptorPool = descriptor_pool;
                dset_allocate_info.descriptorSetCount = swapchain.image_views().size();
                dset_allocate_info.setSetLayouts(dset_layouts);
                auto descriptor_sets = device->allocateDescriptorSets(dset_allocate_info);
                
                vk::DescriptorBufferInfo uniform_buffer_descriptor;
                uniform_buffer_descriptor.buffer = ubo_buffer.get();
                uniform_buffer_descriptor.offset = 0;
                uniform_buffer_descriptor.range = sizeof(ubo);

                vk::WriteDescriptorSet dset_write_info;
                dset_write_info.dstSet = descriptor_sets[0];
                dset_write_info.dstBinding = 0;
                dset_write_info.dstArrayElement = 0;
                dset_write_info.descriptorCount = 1;
                dset_write_info.descriptorType = vk::DescriptorType::eUniformBuffer;
                dset_write_info.pBufferInfo = &uniform_buffer_descriptor;
                device->updateDescriptorSets(dset_write_info, nullptr);


                #warning That should be moved back to member objects, and Descriptor Set Layouts to separate object initialized before that
                sln::vkw::Pipeline pipeline{device, swapchain, dset_layouts[0]};


                Model test_model(device, vertices, indices);
                Model test_model2(device, vertices2, indices2);

                // Synchronization objects
                std::vector<vk::Semaphore> semaphore_image_available = { device->createSemaphore({}) };
                std::vector<vk::Semaphore> semaphore_render_finished = { device->createSemaphore({}) };

                vk::Fence in_flight = device->createFence({vk::FenceCreateFlagBits::eSignaled});
                
                sln::vkw::CommandPool command_pool(device); 
                auto command_buffer = command_pool.allocate_command_buffer(device.graphic_queue());

                while(!window.should_close()) {
                        // ---------------------
                        //     Input Loop
                        // ---------------------
                        // Poll GLFW events (exit event)
                        window.poll_events();
                         
                        glm::vec3 camera = { 1.f, 0.f, 1.f };
                        glm::vec3 target = {0.f, 0.f, 0.f };
                        glm::vec3 up = { 0.f, 0.f, 1.f };
                        ubo_ptr->view = glm::lookAt(camera, target, up);
                        glm::mat4 projection = glm::perspective(glm::radians(45.f), 
                                                                (float)swapchain.extent().height / (float)swapchain.extent().width, 
                                                                0.1f, 10.f);
                        projection[1][1] *= -1;
                        ubo_ptr->projection = projection;
                        ubo_ptr->model = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));


                        // ---------------------
                        //     Render Loop
                        // ---------------------
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

                        command_buffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, 
                                                           pipeline.pipeline_layout(), 
                                                           0, 
                                                           descriptor_sets[0], 
                                                           nullptr);


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
                device->unmapMemory(ubo_buffer.memory());
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


        vk::Buffer m_vertex_buffer;
        vk::DeviceMemory m_vertex_memory;
    };
}
