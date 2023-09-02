#include <catch2/catch_test_macros.hpp>
#include <hamt/utility/bitmap.hpp>

TEST_CASE("bitmap: empty") {
	const auto bmp = hamt::internal::basic_bitmap_view<uint32_t>{};

	REQUIRE(std::as_const(bmp).empty());
	REQUIRE_FALSE(bool(std::as_const(bmp)));
	REQUIRE(std::as_const(bmp).size() == 0u);

	for (unsigned i = 0; i != 32; ++i) {
		REQUIRE_FALSE(std::as_const(bmp)[i].exists);
	}
}

TEST_CASE("bitmap: one record") {
	const auto bmp = hamt::internal::basic_bitmap_view<uint32_t>{0x1u};

	REQUIRE_FALSE(std::as_const(bmp).empty());
	REQUIRE(bool(std::as_const(bmp)));
	REQUIRE(std::as_const(bmp).size() == 1u);

	REQUIRE(std::as_const(bmp)[0].exists);
	REQUIRE(std::as_const(bmp)[0].position == 0u);

	for (unsigned i = 1; i != 32; ++i) {
		REQUIRE_FALSE(std::as_const(bmp)[i].exists);
	}
}

TEST_CASE("bitmap: one record somewhere else") {
	const auto bmp = hamt::internal::basic_bitmap_view<uint32_t>{0x1000u};

	REQUIRE_FALSE(std::as_const(bmp).empty());
	REQUIRE(bool(std::as_const(bmp)));
	REQUIRE(std::as_const(bmp).size() == 1u);

	for (unsigned i = 0; i != 32; ++i) {
		if (i == 12u) {
			continue; // skip
		}
		REQUIRE_FALSE(std::as_const(bmp)[i].exists);
	}

	REQUIRE(std::as_const(bmp)[12].exists);
	REQUIRE(std::as_const(bmp)[12].position == 0u);
}

TEST_CASE("bitmap: two records") {
	const auto bmp = hamt::internal::basic_bitmap_view<uint32_t>{0x1004u};

	REQUIRE_FALSE(std::as_const(bmp).empty());
	REQUIRE(bool(std::as_const(bmp)));
	REQUIRE(std::as_const(bmp).size() == 2u);

	for (unsigned i = 0; i != 32; ++i) {
		if (i == 2u || i == 12u) {
			continue; // skip
		}
		REQUIRE_FALSE(std::as_const(bmp)[i].exists);
	}

	REQUIRE(std::as_const(bmp)[2].exists);
	REQUIRE(std::as_const(bmp)[2].position == 0u);

	REQUIRE(std::as_const(bmp)[12].exists);
	REQUIRE(std::as_const(bmp)[12].position == 1u);
}

TEST_CASE("bitmap: full") {
	const auto bmp = hamt::internal::basic_bitmap_view<uint32_t>{0xFFFFFFFFu};

	REQUIRE_FALSE(std::as_const(bmp).empty());
	REQUIRE(bool(std::as_const(bmp)));
	REQUIRE(std::as_const(bmp).size() == 32u);

	for (unsigned i = 0; i != 32; ++i) {
		REQUIRE(std::as_const(bmp)[i].exists);
		REQUIRE(std::as_const(bmp)[i].position == i);
	}
}
