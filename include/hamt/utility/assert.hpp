#ifndef HAMT_UTILITY_ASSERT_HPP
#define HAMT_UTILITY_ASSERT_HPP

// I have my own assert because cassert's assert is messing with code coverage report.

#ifndef NDEBUG
#define HAMT_ASSERT(e) hamt::internal::assert_this(static_cast<bool>(e), #e, __FILE__, __LINE__);
#else
#define HAMT_ASSERT(e) ((void)(0))
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
