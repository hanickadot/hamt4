#ifndef HAMT_UTILITY_BITMAP_HPP
#define HAMT_UTILITY_BITMAP_HPP

#include <bit>
#include <cassert>
#include <concepts>
#include <cstdint>

namespace hamt::internal {

template <unsigned Bits> struct bits_to_bitmap;

template <> struct bits_to_bitmap<3> {
	using type = uint8_t;
};

template <> struct bits_to_bitmap<4> {
	using type = uint16_t;
};

template <> struct bits_to_bitmap<5> {
	using type = uint32_t;
};

template <> struct bits_to_bitmap<6> {
	using type = uint64_t;
};

template <unsigned Bits> using bits_to_bitmap_t = typename bits_to_bitmap<Bits>::value;

struct record_info {
	unsigned position;
	bool exists;
};

template <std::unsigned_integral T = uint32_t> struct basic_bitmap_view {
	using value_type = T;

	value_type value{};

	static constexpr unsigned bits_available = sizeof(value_type) * 8u;

	constexpr basic_bitmap_view() noexcept = default;
	constexpr basic_bitmap_view(std::same_as<value_type> auto v) noexcept: value{v} { }

	constexpr auto operator[](unsigned index) const noexcept -> record_info {
		assert(index < bits_available);

		const auto bit = T{1u << index};
		const auto mask = bit - 1u;
		return {.position = static_cast<unsigned>(std::popcount(value & mask)), .exists = static_cast<bool>(value & bit)};
	}

	constexpr size_t size() const noexcept {
		return static_cast<size_t>(std::popcount(value));
	}

	constexpr bool empty() const noexcept {
		return value == value_type{};
	}

	explicit constexpr operator bool() const noexcept {
		return !empty();
	}
};

template <unsigned Bits> using bitmap_view_for_pieces = basic_bitmap_view<bits_to_bitmap_t<Bits>>;

} // namespace hamt::internal

#endif
