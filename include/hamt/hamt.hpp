#ifndef HAMT_HAMT_HPP
#define HAMT_HAMT_HPP

#include "utility/bitmap.hpp"
#include "utility/pieces.hpp"
#include <bit>
#include <functional>
#include <iostream>
#include <utility>
#include <concepts>
#include <cstddef>

namespace hamt {

// iterator's sentinel
struct sentinel { };

struct node_handler {
	std::byte * pointer{nullptr};

	explicit constexpr operator bool() const noexcept {
		return pointer != nullptr;
	}

	constexpr void release() noexcept {
		// TODO
	}
};

struct node_owner {
	node_handler handler{};

	constexpr ~node_owner() noexcept {
		handler.release();
	}
};

template <typename T> concept is_transparent = requires() {
	typename T::is_transparent;
};

template <typename K, typename T, typename EqualityCompare, typename Hash> concept needle_for = requires(const K & key, const T & record, EqualityCompare ec, Hash hash) {
	requires((std::same_as<K, T> || (is_transparent<Hash> && is_transparent<EqualityCompare>)));
	{ ec(key, record) } -> std::same_as<bool>;
	{ hash(key) } -> std::same_as<decltype(hash(record))>;
	{ hash(record) } -> std::same_as<decltype(hash(key))>;
};

struct transparent_hash {
	using is_transparent = void;

	template <typename T> constexpr auto operator()(const T & item) noexcept(noexcept(std::hash<T>{}(item))) {
		return std::hash<T>{}(item);
	}
};

template <bool Value, typename T> using conditionaly_add_const = std::conditional_t<Value, const T, T>;

template <typename T, typename Hash = transparent_hash, typename EqualityCompare = std::equal_to<void>> struct set {
	template <bool Const> struct basic_iterator {
		using value_type = conditionaly_add_const<Const, T>;
		using reference_type = value_type &;

		value_type * ptr;

		friend constexpr bool operator==(const basic_iterator &, sentinel) {
			return true;
		}

		constexpr basic_iterator & operator++() noexcept {
			return *this;
		}

		constexpr reference_type operator*() noexcept {
			return *ptr;
		}
	};

	using hash_pieces = internal::basic_hash_pieces<uint32_t, 5>;
	using bitmap_view = internal::basic_bitmap_view<uint32_t>;

	using iterator = basic_iterator<false>;
	using const_iterator = basic_iterator<true>;

	[[no_unique_address]] Hash hasher{};
	[[no_unique_address]] EqualityCompare comparator{};

	template <typename... Args> constexpr iterator emplace(Args &&... args) requires std::constructible_from<T, Args...> {
		[[maybe_unused]] const auto obj = T(std::forward<Args>(args)...);
		[[maybe_unused]] auto hash = hash_pieces{hasher(obj)};

		while (hash) {
			const auto bitmap = bitmap_view{0xAAAAAAAAu};
			const auto piece = hash.front();
			const auto [position, exists] = bitmap[piece];
			std::cout << piece << " -> [position: " << position << ", exists: " << (exists ? "yes" : "no") << "]\n";

			if (!exists) {
				std::cout << "early exit.\n";
				break;
			}

			hash = hash.pop();
		}

		return {};
	}

	constexpr iterator find(const hamt::needle_for<T, EqualityCompare, Hash> auto & key) {
		return {};
	}

	constexpr const_iterator find(const hamt::needle_for<T, EqualityCompare, Hash> auto & key) const {
		return {};
	}

	constexpr size_t size() const noexcept {
		return 0u;
	}

	constexpr iterator begin() noexcept {
		return {nullptr};
	}

	constexpr const_iterator begin() const noexcept {
		return {};
	}

	constexpr sentinel end() const noexcept {
		return {};
	}
};

} // namespace hamt

#endif
