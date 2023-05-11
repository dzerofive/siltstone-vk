#include <sln/vkw/print_info.hpp>

using namespace sln::prefix;

void sln::print_info(const sln::vkw::PhysicalDevice& pdevice) {
        auto i = pdevice.get().getProperties(); 
        std::cout << INFO << "Found " << pdevice.get_count() << " device(s)\n";
        std::cout << INFO << "Selected device is " << i.deviceName << ", driver version " 
                << VK_VERSION_MAJOR(i.driverVersion) << '.' 
                << VK_VERSION_MINOR(i.driverVersion) << '.' 
                << VK_VERSION_PATCH(i.driverVersion) << '\n';
        
        auto mp = pdevice.get().getMemoryProperties();
        auto heap_count = mp.memoryHeapCount;
        auto mem_heaps = mp.memoryHeaps;
        std::cout << INFO << "Has " << heap_count << " memory heaps: ";
        for(int i = 0; i < heap_count; ++i) {
                std::cout << mem_heaps[i].size/(1024*1024) << "M ";
        }
        std::cout << '\n';
        auto mem_type_count = mp.memoryTypeCount;
        auto mem_types = mp.memoryTypes;
        std::cout << INFO << "Has " << mem_type_count << " memory types:\n";
        for(int i = 0; i < mem_type_count; ++i) {
                std::cout << ADDIT << "Heap index " << mem_types[i].heapIndex 
                          << ", flags " << vk::to_string(mem_types[i].propertyFlags) 
                          << '\n';
        }

        auto qp = pdevice.get().getQueueFamilyProperties();
        std::cout << INFO << "Has " << qp.size() << " queue families:\n";
        for(int i = 0; i < qp.size(); ++i) {
                std::cout << ADDIT << "Index " << i << ": " << qp[i].queueCount 
                          << " queue(s) with flag(s) " << vk::to_string(qp[i].queueFlags) 
                          << '\n';
        }

        auto limits = i.limits;
        std::cout << INFO << "Discrete queue priorities " << limits.discreteQueuePriorities << '\n';
}

void sln::print_info(const sln::vkw::Swapchain& swapchain) {

}
