#include <catch2/catch_test_macros.hpp>
#include <hamt/utility/bitmap.hpp>

TEST_CASE("bitmap: empty") {
	const auto bmp = hamt::internal::basic_bitmap_view<uint32_t>{};

	REQUIRE(bmp.empty());
	REQUIRE_FALSE(bool(bmp));
	REQUIRE(bmp.size() == 0u);

	for (unsigned i = 0; i != 32; ++i) {
		REQUIRE_FALSE(bmp[i].exists);
	}
}

TEST_CASE("bitmap: one record") {
	const auto bmp = hamt::internal::basic_bitmap_view<uint32_t>{0x1u};

	REQUIRE_FALSE(bmp.empty());
	REQUIRE(bool(bmp));
	REQUIRE(bmp.size() == 1u);

	REQUIRE(bmp[0].exists);
	REQUIRE(bmp[0].position == 0u);

	for (unsigned i = 1; i != 32; ++i) {
		REQUIRE_FALSE(bmp[i].exists);
	}
}

TEST_CASE("bitmap: one record somewhere else") {
	const auto bmp = hamt::internal::basic_bitmap_view<uint32_t>{0x1000u};

	REQUIRE_FALSE(bmp.empty());
	REQUIRE(bool(bmp));
	REQUIRE(bmp.size() == 1u);

	for (unsigned i = 0; i != 32; ++i) {
		if (i == 12u) {
			continue; // skip
		}
		REQUIRE_FALSE(bmp[i].exists);
	}

	REQUIRE(bmp[12].exists);
	REQUIRE(bmp[12].position == 0u);
}

TEST_CASE("bitmap: two records") {
	const auto bmp = hamt::internal::basic_bitmap_view<uint32_t>{0x1004u};

	REQUIRE_FALSE(bmp.empty());
	REQUIRE(bool(bmp));
	REQUIRE(bmp.size() == 2u);

	for (unsigned i = 0; i != 32; ++i) {
		if (i == 2u || i == 12u) {
			continue; // skip
		}
		REQUIRE_FALSE(bmp[i].exists);
	}

	REQUIRE(bmp[2].exists);
	REQUIRE(bmp[2].position == 0u);

	REQUIRE(bmp[12].exists);
	REQUIRE(bmp[12].position == 1u);
}

TEST_CASE("bitmap: full") {
	const auto bmp = hamt::internal::basic_bitmap_view<uint32_t>{0xFFFFFFFFu};

	REQUIRE_FALSE(bmp.empty());
	REQUIRE(bool(bmp));
	REQUIRE(bmp.size() == 32u);

	for (unsigned i = 0; i != 32; ++i) {
		REQUIRE(bmp[i].exists);
		REQUIRE(bmp[i].position == i);
	}
}
