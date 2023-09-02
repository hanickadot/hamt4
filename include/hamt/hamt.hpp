#ifndef HAMT_HAMT_HPP
#define HAMT_HAMT_HPP

#include <functional>
#include <utility>
#include <concepts>
#include <cstddef>

namespace hamt {

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

	template <typename T> constexpr bool operator()(const T & item) noexcept(noexcept(std::hash<T>{}(item))) {
		return std::hash<T>{}(item);
	}
};

template <typename T, typename Hash = transparent_hash, typename EqualityCompare = std::equal_to<void>> struct set {
	struct sentinel { };
	struct iterator {
		T * ptr;

		friend constexpr bool operator==(const iterator &, sentinel) {
			return true;
		}

		constexpr iterator & operator++() noexcept {
			return *this;
		}

		constexpr const T & operator*() noexcept {
			return *ptr;
		}
	};
	struct const_iterator { };

	template <typename... Args> constexpr iterator emplace(Args &&... args) requires std::constructible_from<T, Args...> {
		[[maybe_unused]] const auto obj = T(std::forward<Args>(args)...);
		[[maybe_unused]] const auto hash = Hash{}(obj);
		return {};
	}

	constexpr iterator find(const hamt::needle_for<T, EqualityCompare, Hash> auto & key) {
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
