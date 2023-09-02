#ifndef HAMT_UTILITY_PIECES_HPP
#define HAMT_UTILITY_PIECES_HPP

#include <concepts>
#include <cstdint>

namespace hamt::internal {

template <std::unsigned_integral HashT = uint32_t, unsigned Bits = 5> struct basic_hash_pieces {
	static_assert(Bits <= 8u);
	static_assert(Bits > 0u);

	HashT value;
	int8_t remaining_bits = sizeof(HashT) * 8;

	static constexpr auto mask = HashT{(0b1ull << Bits) - 1ull}; // mask with Bits count of 1s

	explicit constexpr basic_hash_pieces(size_t h) noexcept: value{static_cast<HashT>(h)} { }
	constexpr basic_hash_pieces(HashT v, int8_t rem) noexcept: value{v}, remaining_bits{rem} { }

	constexpr unsigned front() const noexcept {
		return static_cast<unsigned>(value & mask);
	}

	constexpr basic_hash_pieces pop() const noexcept {
		return {(value >> Bits), static_cast<int8_t>(remaining_bits - Bits)};
	}

	constexpr bool empty() const noexcept {
		return remaining_bits <= 0;
	}

	explicit constexpr operator bool() const noexcept {
		return !empty();
	}
};

} // namespace hamt::internal

#endif
