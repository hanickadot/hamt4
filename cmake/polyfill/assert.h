#ifndef HAMT_UTILITY_ASSERT_HPP
#define HAMT_UTILITY_ASSERT_HPP

// I have my own assert because cassert's assert is messing with code coverage report.

#undef assert

#ifndef NDEBUG
#define assert(e) hamt::internal::assert_this(static_cast<bool>(e), #e, __FILE__, __LINE__);
#else
#define assert(e) ((void)(0))
#endif

#include <cstdio>
#include <cstdlib>

namespace hamt::internal {

constexpr void assert_this(bool value, const char * expression, const char * file, unsigned line) noexcept {
	if (!value) {
		printf("%s:%u: failed assertion '%s'\n", file, line, expression);
		std::abort();
	}
}

} // namespace hamt::internal

#endif
