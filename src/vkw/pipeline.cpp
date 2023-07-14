#include <sln/vkw/pipeline.hpp>
#include <vulkan/vulkan_structs.hpp>

//       -== Outline ==-       //
// - render pass               //
// - fixed stage functions     //
// - pipeline layout           //
// - programmable (shaders)    //
// - getters                   //
// - framebuffers              //

inline void sln::vkw::Pipeline::create_render_pass(const sln::vkw::Device& device, const sln::vkw::Swapchain& swapchain) {
        vk::AttachmentDescription color_attachment{};
        color_attachment.format = swapchain.format().format;
        color_attachment.samples = vk::SampleCountFlagBits::e1;
        color_attachment.loadOp = vk::AttachmentLoadOp::eClear;
        color_attachment.storeOp = vk::AttachmentStoreOp::eStore;
        color_attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        color_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        color_attachment.initialLayout = vk::ImageLayout::eUndefined;
        color_attachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;
        vk::AttachmentReference color_attachment_ref{}; //         ^^^
        color_attachment_ref.attachment = 0; // Index of the color attachment, accessed in shaders
        color_attachment_ref.layout = vk::ImageLayout::eColorAttachmentOptimal;
        vk::SubpassDescription subpass_desc{};
        subpass_desc.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        subpass_desc.colorAttachmentCount = 1;
        subpass_desc.pColorAttachments = &color_attachment_ref;
        
        vk::RenderPassCreateInfo renderpass_info{};
        renderpass_info.attachmentCount = 1;
        renderpass_info.pAttachments = &color_attachment;
        renderpass_info.subpassCount = 1;
        renderpass_info.pSubpasses = &subpass_desc;
        m_render_pass = device->createRenderPass(renderpass_info); 
}

// Constructor
sln::vkw::Pipeline::Pipeline(const sln::vkw::Device& device, const sln::vkw::Swapchain& swapchain, 
                             const vk::ArrayProxyNoTemporaries<vk::DescriptorSetLayout>& descriptor_set_layouts) {
        create_render_pass(device, swapchain);


        // Fixed stages
        std::vector<vk::DynamicState> dynamic_states {
                vk::DynamicState::eViewport, 
                vk::DynamicState::eScissor
        };
        vk::PipelineDynamicStateCreateInfo dynamic_state_info{};
        dynamic_state_info.setDynamicStates(dynamic_states);
        

        auto binding_description = sln::Vertex::binding_description();
        auto attribute_descriptions = sln::Vertex::attribute_descriptions();
        vk::PipelineVertexInputStateCreateInfo vertex_input_info{};
        vertex_input_info.vertexBindingDescriptionCount = 1;
        vertex_input_info.pVertexBindingDescriptions = &binding_description;
        vertex_input_info.vertexAttributeDescriptionCount = attribute_descriptions.size();
        vertex_input_info.pVertexAttributeDescriptions = attribute_descriptions.data();


        vk::PipelineInputAssemblyStateCreateInfo input_assembly_info{};
        input_assembly_info.topology = vk::PrimitiveTopology::eTriangleList;
        input_assembly_info.primitiveRestartEnable = false;

        
        vk::Viewport viewport{};
        viewport.x = 0.f;
        viewport.y = 0.f;
        viewport.height = swapchain.extent().height;
        viewport.width = swapchain.extent().width;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vk::Rect2D scissor{};
        scissor.offset = vk::Offset2D{0, 0};
        scissor.extent = swapchain.extent();
        vk::PipelineViewportStateCreateInfo viewport_state_info{};
        viewport_state_info.viewportCount = 1;
        viewport_state_info.pViewports = &viewport;
        viewport_state_info.scissorCount = 1;
        viewport_state_info.pScissors = &scissor;


        vk::PipelineRasterizationStateCreateInfo rasterization_info{};
        rasterization_info.depthClampEnable = false;
        rasterization_info.rasterizerDiscardEnable = false;
        rasterization_info.polygonMode = vk::PolygonMode::eFill;
        rasterization_info.lineWidth = 1.f;
        rasterization_info.cullMode = vk::CullModeFlagBits::eBack;
        rasterization_info.frontFace = vk::FrontFace::eClockwise;
        rasterization_info.depthClampEnable = false;
        rasterization_info.depthBiasConstantFactor = 0.f;
        rasterization_info.depthBiasSlopeFactor = 0.f;
        rasterization_info.depthBiasClamp = 0.f;


        vk::PipelineMultisampleStateCreateInfo multisample_info{};
        multisample_info.sampleShadingEnable = false;
        multisample_info.rasterizationSamples = vk::SampleCountFlagBits::e1;
        multisample_info.minSampleShading = 1.f;
        multisample_info.pSampleMask = nullptr;
        multisample_info.alphaToCoverageEnable = false;
        multisample_info.alphaToOneEnable = false;


        vk::PipelineDepthStencilStateCreateInfo depth_stencil_info{};
        

        vk::PipelineColorBlendAttachmentState color_blend_attachment{};
        color_blend_attachment.colorWriteMask = 
                vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | 
                vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
        color_blend_attachment.blendEnable = true;
        color_blend_attachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
        color_blend_attachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
        color_blend_attachment.colorBlendOp = vk::BlendOp::eAdd;
        color_blend_attachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
        color_blend_attachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
        color_blend_attachment.alphaBlendOp = vk::BlendOp::eAdd;
        vk::PipelineColorBlendStateCreateInfo color_blend_info{};
        color_blend_info.logicOpEnable = false;
        //color_blend_info.logicOp = vk::LogicOp::eCopy;
        color_blend_info.attachmentCount = 1;
        color_blend_info.pAttachments = &color_blend_attachment;


        // Layout
        vk::PipelineLayoutCreateInfo pipeline_layout_info{};
        pipeline_layout_info.setSetLayouts(descriptor_set_layouts);
        pipeline_layout_info.pushConstantRangeCount = 0;
        pipeline_layout_info.pPushConstantRanges = nullptr;
        vk::PipelineLayout pipeline_layout = device->createPipelineLayout(pipeline_layout_info);


        // Shader stages
        sln::vkw::Shader vertex_shader(device, "shaders/vert.spv");
        vk::PipelineShaderStageCreateInfo vertex_shader_stage{};
        vertex_shader_stage.stage = vk::ShaderStageFlagBits::eVertex;
        vertex_shader_stage.module = vertex_shader.shader_module();
        vertex_shader_stage.pName = "main";

        sln::vkw::Shader fragment_shader(device, "shaders/frag.spv"); 
        vk::PipelineShaderStageCreateInfo fragment_shader_stage{};
        fragment_shader_stage.stage = vk::ShaderStageFlagBits::eFragment;
        fragment_shader_stage.module = fragment_shader.shader_module();
        fragment_shader_stage.pName = "main";

        std::vector<vk::PipelineShaderStageCreateInfo> shader_stages = {
                vertex_shader_stage, 
                fragment_shader_stage
        };


        // Crate pipeline
        vk::GraphicsPipelineCreateInfo pipeline_info{};
        pipeline_info.stageCount = shader_stages.size();
        pipeline_info.pStages = shader_stages.data();
        pipeline_info.pVertexInputState = &vertex_input_info;
        pipeline_info.pInputAssemblyState = &input_assembly_info;
        pipeline_info.pTessellationState = nullptr;
        pipeline_info.pViewportState = &viewport_state_info;
        pipeline_info.pRasterizationState = &rasterization_info;
        pipeline_info.pMultisampleState = &multisample_info;
        pipeline_info.pDepthStencilState = &depth_stencil_info;
        pipeline_info.pColorBlendState = &color_blend_info;
        pipeline_info.pDynamicState = &dynamic_state_info;
        pipeline_info.layout = pipeline_layout;
        pipeline_info.renderPass = m_render_pass;
        pipeline_info.subpass = 0;
        
        m_pipeline = device->createGraphicsPipeline(nullptr, pipeline_info).value;


        // Create framebuffers
        for(auto i : swapchain.image_views()) {
                vk::FramebufferCreateInfo framebuffer_info{};
                framebuffer_info.renderPass = m_render_pass;
                framebuffer_info.attachmentCount = 1;
                framebuffer_info.pAttachments = &i;
                framebuffer_info.width = swapchain.extent().width;
                framebuffer_info.height = swapchain.extent().height;
                framebuffer_info.layers = 1;

                m_framebuffers.push_back(device->createFramebuffer(framebuffer_info, nullptr));
        }
}

// Getters
const vk::Pipeline* sln::vkw::Pipeline::operator->() const noexcept {
        return &m_pipeline;
}
const vk::Pipeline& sln::vkw::Pipeline::get() const noexcept {
        return m_pipeline;
}
const vk::RenderPass sln::vkw::Pipeline::render_pass() const noexcept {
        return m_render_pass;
}
const vk::Framebuffer sln::vkw::Pipeline::framebuffer(size_t index) const noexcept {
        return m_framebuffers[index];
}
const vk::PipelineLayout sln::vkw::Pipeline::pipeline_layout() const noexcept {
        return m_pipeline_layout;
};
