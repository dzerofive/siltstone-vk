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
        std::cout << ADDIT << "Has " << heap_count << " memory heaps: ";
        for(int i = 0; i < heap_count; ++i) {
                std::cout << mem_heaps[i].size/(1024*1024) << "M ";
        }
        std::cout << '\n';
}
