#include <catch2/catch_test_macros.hpp>
#include <hamt/utility/pieces.hpp>

TEST_CASE("hash pieces: basics") {
	constexpr uint32_t base_value = 0xCC520C41ul;
	auto pieces = hamt::internal::basic_hash_pieces<uint32_t, 5>{0x1CC520C41ull};

	REQUIRE(pieces.remaining_bits == 32u);
	REQUIRE(pieces.value == 0xCC520C41ul);
	REQUIRE(pieces.front() == 0x1u);
	REQUIRE_FALSE(pieces.empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == 27);
	REQUIRE(pieces.value == (base_value >> 5u));
	REQUIRE(pieces.front() == 0x2u);
	REQUIRE_FALSE(pieces.empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == 22);
	REQUIRE(pieces.value == (base_value >> 10u));
	REQUIRE(pieces.front() == 0x3u);
	REQUIRE_FALSE(pieces.empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == 17);
	REQUIRE(pieces.value == (base_value >> 15u));
	REQUIRE(pieces.front() == 0x4u);
	REQUIRE_FALSE(pieces.empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == 12);
	REQUIRE(pieces.value == (base_value >> 20u));
	REQUIRE(pieces.front() == 0x5u);
	REQUIRE_FALSE(pieces.empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == 7);
	REQUIRE(pieces.value == (base_value >> 25u));
	REQUIRE(pieces.front() == 0x6u);
	REQUIRE_FALSE(pieces.empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == 2);
	REQUIRE(pieces.front() == 0x3u); // cut
	REQUIRE(pieces.value == (base_value >> 30u));
	REQUIRE_FALSE(pieces.empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == -3);
	REQUIRE(pieces.empty());
	REQUIRE_FALSE(pieces);
}