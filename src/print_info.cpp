#include <sln/print_info.hpp>

using namespace sln::prefix;

void sln::print_info(sln::Window window) {
        std::cout << INFO << "Created window " << window.width() << "×" << window.height() 
                  << " with name \"" << window.title() << "\"\n";
}
