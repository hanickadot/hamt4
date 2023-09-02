#include <catch2/catch_test_macros.hpp>
#include <hamt/utility/pieces.hpp>

TEST_CASE("hash pieces: empty") {
	constexpr uint32_t base_value = 0ul;
	auto pieces = hamt::internal::basic_hash_pieces<uint32_t, 5>{0ull};

	REQUIRE(pieces.remaining_bits == 32u);
	REQUIRE(pieces.value == base_value);
	REQUIRE(std::as_const(pieces).front() == 0u);
	REQUIRE_FALSE(std::as_const(pieces).empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == 27);
	REQUIRE(pieces.value == (base_value >> 5u));
	REQUIRE(std::as_const(pieces).front() == 0u);
	REQUIRE_FALSE(std::as_const(pieces).empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == 22);
	REQUIRE(pieces.value == (base_value >> 10u));
	REQUIRE(std::as_const(pieces).front() == 0u);
	REQUIRE_FALSE(std::as_const(pieces).empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == 17);
	REQUIRE(pieces.value == (base_value >> 15u));
	REQUIRE(std::as_const(pieces).front() == 0u);
	REQUIRE_FALSE(std::as_const(pieces).empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == 12);
	REQUIRE(pieces.value == (base_value >> 20u));
	REQUIRE(std::as_const(pieces).front() == 0u);
	REQUIRE_FALSE(std::as_const(pieces).empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == 7);
	REQUIRE(pieces.value == (base_value >> 25u));
	REQUIRE(std::as_const(pieces).front() == 0u);
	REQUIRE_FALSE(std::as_const(pieces).empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == 2);
	REQUIRE(std::as_const(pieces).front() == 0u); // cut
	REQUIRE(pieces.value == (base_value >> 30u));
	REQUIRE_FALSE(std::as_const(pieces).empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == -3);
	REQUIRE(std::as_const(pieces).empty());
	REQUIRE_FALSE(pieces);
}

TEST_CASE("hash pieces: basics") {
	constexpr uint32_t base_value = 0xCC520C41ul;
	auto pieces = hamt::internal::basic_hash_pieces<uint32_t, 5>{0x1CC520C41ull};

	REQUIRE(pieces.remaining_bits == 32u);
	REQUIRE(pieces.value == base_value);
	REQUIRE(std::as_const(pieces).front() == 0x1u);
	REQUIRE_FALSE(std::as_const(pieces).empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == 27);
	REQUIRE(pieces.value == (base_value >> 5u));
	REQUIRE(std::as_const(pieces).front() == 0x2u);
	REQUIRE_FALSE(std::as_const(pieces).empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == 22);
	REQUIRE(pieces.value == (base_value >> 10u));
	REQUIRE(std::as_const(pieces).front() == 0x3u);
	REQUIRE_FALSE(std::as_const(pieces).empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == 17);
	REQUIRE(pieces.value == (base_value >> 15u));
	REQUIRE(std::as_const(pieces).front() == 0x4u);
	REQUIRE_FALSE(std::as_const(pieces).empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == 12);
	REQUIRE(pieces.value == (base_value >> 20u));
	REQUIRE(std::as_const(pieces).front() == 0x5u);
	REQUIRE_FALSE(std::as_const(pieces).empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == 7);
	REQUIRE(pieces.value == (base_value >> 25u));
	REQUIRE(std::as_const(pieces).front() == 0x6u);
	REQUIRE_FALSE(std::as_const(pieces).empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == 2);
	REQUIRE(std::as_const(pieces).front() == 0x3u); // cut
	REQUIRE(pieces.value == (base_value >> 30u));
	REQUIRE_FALSE(std::as_const(pieces).empty());
	REQUIRE(pieces);

	pieces = pieces.pop();

	REQUIRE(pieces.remaining_bits == -3);
	REQUIRE(std::as_const(pieces).empty());
	REQUIRE_FALSE(pieces);
}
