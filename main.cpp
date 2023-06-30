#include <sln/type/debug_level.hpp>
#include <iostream>
#include <string>
#include <vk06.hpp>

#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

int main(int argc, char *argv[]) {
        boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::trace);

	sln::vk06 app;

	return 0;
}
