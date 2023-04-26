#include <vk06.hpp>


std::vector<char> d05::vk06::read_file(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	
	if(!file.is_open())
		throw std::runtime_error("Failed to open file!");

	size_t file_size = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(file_size);
	std::cout << "Loading file " << filename << " of size " << file_size << " bytes\n";

	file.seekg(0);
	file.read(buffer.data(), file_size);

	file.close();
	return buffer;
}


vk::ShaderModule d05::vk06::create_shader_module(const std::vector<char>& code) {
	vk::ShaderModuleCreateInfo shader_info{};
	shader_info.codeSize = code.size();
	shader_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

	return device.createShaderModule(shader_info);
}
