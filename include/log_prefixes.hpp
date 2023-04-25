#include <string>

class gtkr_log {
public:
	static constexpr std::string_view GOOD = "\033[90m[ \033[32m✓\033[90m ]\033[0m ";
	static constexpr std::string_view BAD =  "\033[90m[ \033[31m✗\033[90m ]\033[0m ";
	static constexpr std::string_view INFO = "\033[90m[ \033[36mℹ\033[90m ]\033[0m ";
	static constexpr std::string_view WARN = "\033[90m[ \033[33m⚠\033[90m ]\033[0m ";
};
