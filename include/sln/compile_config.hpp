#pragma once
#include <sln/type/debug_level.hpp>

namespace sln::cc {

// Using high debug levels may slow down the application
static constexpr debug_level debug_level = debug_level::extensive;

} // namespace sln
