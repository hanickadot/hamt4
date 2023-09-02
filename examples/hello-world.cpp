#include <hamt/hamt.hpp>
#include <string>
#include <string_view>

int main() {
	using namespace std::string_view_literals;
	using namespace std::string_literals;

	auto s = hamt::set<std::string>{};
	s.emplace("hello there");
	s.find("hello world"sv);

	for (const auto & record: s) {
	}
}