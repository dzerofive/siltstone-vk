#pragma once

#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>


namespace sln::vkw {
class Instance {
public:
	Instance(std::string app_name, 
		 std::vector<const char*> extensions, 
		 std::vector<const char*> layers);
	vk::Instance& get();

private:	
        vk::Instance m_instance;

};
} // namespace d05
