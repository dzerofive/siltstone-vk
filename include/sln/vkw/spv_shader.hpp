#pragma once

#include <vulkan/vulkan.hpp>
#include <sln/vkw/device.hpp>
#include <filesystem>
#include <string>

namespace sln::vkw {
class Shader {
public:
        Shader(sln::vkw::Device device, const char* filepath);
        Shader(sln::vkw::Device device, std::string filepath);
        Shader(sln::vkw::Device device, std::filesystem::path filepath);
        const vk::ShaderModule shader_module() const noexcept;

private:
        vk::ShaderModule m_shader_module;
};
} // namespace sln::vkw
