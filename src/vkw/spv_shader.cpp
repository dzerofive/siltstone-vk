#include <sln/vkw/spv_shader.hpp>

#include <vector>
#include <fstream>

#include <iostream>

vk::ShaderModule create_shader_module(sln::vkw::Device device, const char* filepath) {
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);
        if(!file.is_open()) throw std::runtime_error("Failed to open shader file");

        size_t file_size = file.tellg();
        std::vector<char> shader_code(file_size);
        file.seekg(0);
        file.read(shader_code.data(), file_size);

        vk::ShaderModuleCreateInfo module_info{};
        module_info.codeSize = shader_code.size();
        module_info.pCode = reinterpret_cast<const uint32_t*>(shader_code.data());
        
        return device->createShaderModule(module_info);
}

sln::vkw::Shader::Shader(sln::vkw::Device device, const char* filepath) {
        m_shader_module = create_shader_module(device, filepath);
}
sln::vkw::Shader::Shader(sln::vkw::Device device, std::string filepath) {
        m_shader_module = create_shader_module(device, filepath.c_str());
}
sln::vkw::Shader::Shader(sln::vkw::Device device, std::filesystem::path filepath) {
        m_shader_module = create_shader_module(device, filepath.c_str());
}

const vk::ShaderModule sln::vkw::Shader::shader_module() const noexcept {
        return m_shader_module;
}
