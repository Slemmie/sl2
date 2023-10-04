#pragma once

#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#ifdef _GLIBCXX_DEBUG
#include <cassert>
#endif

template <typename A> class Segtree {

public:

	template <bool BUILD = false> constexpr Segtree(size_t _size = 0) :
	m_size((1ULL << (65 - __builtin_clzll(_size))) >> 1),
	m_leaf_cnt(_size),
	m_items(this->m_size + _size - !!_size, A { })
	{
		if constexpr (BUILD) {
			this->m_build(0, 0, this->m_size);
		}
	}

	template <typename B> constexpr Segtree(const std::vector <B>& _v) :
	m_size((1ULL << (65 - __builtin_clzll(_v.size()))) >> 1),
	m_leaf_cnt(_v.size()),
	m_items(this->m_size - !!_v.size(), A { })
	{
		this->m_items.reserve(this->m_items.size() + _v.size());
		std::transform(_v.begin(), _v.end(), std::back_inserter(this->m_items), [] (const B& entry) -> A { return { entry }; });
		this->m_build(0, 0, this->m_size);
	}

	template <typename B> constexpr Segtree(size_t _size, const std::vector <B>& _v) :
	Segtree <A> (_v.begin(), _v.begin() + std::min(_size, _v.size()))
	{ }

	constexpr std::vector <A> current() {
		return std::vector <A> (this->m_items.begin() + this->m_size - !!this->m_size, this->m_items.end());
	}

	constexpr size_t size() noexcept {
		return this->m_leaf_cnt;
	}

	template <typename... ARGS> constexpr void update(size_t ind, ARGS&&... args) {
#ifdef _GLIBCXX_DEBUG
		assert(ind < this->m_leaf_cnt);
#endif
		this->m_update(ind, 0, 0, this->m_size, std::forward <ARGS> (args) ...);
	}

	constexpr A query() {
		this->query(0, this->m_leaf_cnt);
	}

	constexpr A query(size_t l, size_t r) {
#ifdef _GLIBCXX_DEBUG
		assert(l < r && r <= this->m_leaf_cnt);
#endif
		return this->m_query(l, r, 0, 0, this->m_size);
	}

	static constexpr const size_t npos = ~static_cast <size_t> (0);

	struct Index_item_pair {
		size_t index;
		A item;
	};

	template <typename F> constexpr Index_item_pair find_first(size_t l, size_t r, F&& check) {
#ifdef _GLIBCXX_DEBUG
		assert(l < r && r <= this->m_leaf_cnt);
#endif
		return this->m_find_first(l, r, 0, 0, this->m_size, std::forward <F> (check));
	}

	template <typename F> constexpr Index_item_pair find_first(F&& check) {
		return this->find_first(0, this->m_leaf_cnt, std::forward <F> (check));
	}

	template <typename F> constexpr Index_item_pair find_last(size_t l, size_t r, F&& check) {
#ifdef _GLIBCXX_DEBUG
		assert(l < r && r <= this->m_leaf_cnt);
#endif
		return this->m_find_last(l, r, 0, 0, this->m_size, std::forward <F> (check));
	}

	template <typename F> constexpr Index_item_pair find_last(F&& check) {
		return this->find_last(0, this->m_leaf_cnt, std::forward <F> (check));
	}

private:

	size_t m_size;
	size_t m_leaf_cnt;
	std::vector <A> m_items;

private:

	constexpr void m_build(size_t now, size_t l, size_t r) {
		if (!(r - l - 1)) {
			return;
		}
		const size_t mid = (l + r) >> 1;
		this->m_build((now << 1) + 1, l, mid);
		if (mid < this->m_leaf_cnt) {
			this->m_build((now << 1) + 2, mid, r);
			this->m_items[now] = this->m_items[(now << 1) + 1] + this->m_items[(now << 1) + 2];
		} else {
			this->m_items[now] = this->m_items[(now << 1) + 1];
		}
	}

	template <typename... ARGS> constexpr void m_update(size_t ind, size_t now, size_t l, size_t r, ARGS&&... args) {
		if (!(r - l - 1)) {
			this->m_items[now].update(std::forward <ARGS> (args) ...);
			return;
		}
		const size_t mid = (l + r) >> 1;
		if (ind < mid) {
			this->m_update(ind, (now << 1) + 1, l, mid, std::forward <ARGS> (args) ...);
			if (mid < this->m_leaf_cnt) {
				this->m_items[now] = this->m_items[(now << 1) + 1] + this->m_items[(now << 1) + 2];
			} else {
				this->m_items[now] = this->m_items[(now << 1) + 1];
			}
		} else if (mid < this->m_leaf_cnt) {
			this->m_update(ind, (now << 1) + 2, mid, r, std::forward <ARGS> (args) ...);
			this->m_items[now] = this->m_items[(now << 1) + 1] + this->m_items[(now << 1) + 2];
		}
	}

	constexpr A m_query(size_t tl, size_t tr, size_t now, size_t l, size_t r) {
		if (l >= tl && r <= tr) {
			return this->m_items[now];
		}
		const size_t mid = (l + r) >> 1;
		if (tl < mid && mid < tr) {
			return this->m_query(tl, tr, (now << 1) + 1, l, mid) + this->m_query(tl, tr, (now << 1) + 2, mid, r);
		}
		if (tr <= mid) {
			return this->m_query(tl, tr, (now << 1) + 1, l, mid);
		}
		return this->m_query(tl, tr, (now << 1) + 2, mid, r);
	}

	template <typename F> constexpr Index_item_pair m_find_first_exists(size_t now, size_t l, size_t r, F&& check) {
		if (!(r - l - 1)) {
			return { l, this->m_items[now] };
		}
		const size_t mid = (l + r) >> 1;
		return check(this->m_items[(now << 1) + 1]) ?
		this->m_find_first_exists((now << 1) + 1, l, mid, std::forward <F> (check)) :
		this->m_find_first_exists((now << 1) + 2, mid, r, std::forward <F> (check));
	}

	template <typename F> constexpr Index_item_pair m_find_first(size_t tl, size_t tr, size_t now, size_t l, size_t r, F&& check) {
		if (l >= tl && r <= tr) {
			if (check(this->m_items[now])) {
				return this->m_find_first_exists(now, l, r, std::forward <F> (check));
			}
			return { npos, A { } };
		}
		const size_t mid = (l + r) >> 1;
		if (l < tr && mid > tl) {
			Index_item_pair candidate = this->m_find_first(tl, tr, (now << 1) + 1, l, mid, std::forward <F> (check));
			if (candidate.index != npos) {
				return candidate;
			}
		}
		if (mid < tr && r > tl) {
			Index_item_pair candidate = this->m_find_first(tl, tr, (now << 1) + 2, mid, r, std::forward <F> (check));
			if (candidate.index != npos) {
				return candidate;
			}
		}
		return { npos, A { } };
	}

	template <typename F> constexpr Index_item_pair m_find_last_exists(size_t now, size_t l, size_t r, F&& check) {
		if (!(r - l - 1)) {
			return { l, this->m_items[now] };
		}
		const size_t mid = (l + r) >> 1;
		return check(this->m_items[(now << 1) + 2]) ?
		this->m_find_last_exists((now << 1) + 2, l, mid, std::forward <F> (check)) :
		this->m_find_last_exists((now << 1) + 1, mid, r, std::forward <F> (check));
	}

	template <typename F> constexpr Index_item_pair m_find_last(size_t tl, size_t tr, size_t now, size_t l, size_t r, F&& check) {
		if (l >= tl && r <= tr) {
			if (check(this->m_items[now])) {
				return this->m_find_last_exists(now, l, r, std::forward <F> (check));
			}
			return { npos, A { } };
		}
		const size_t mid = (l + r) >> 1;
		if (mid < tr && r > tl) {
			Index_item_pair candidate = this->m_find_last(tl, tr, (now << 1) + 2, mid, r, std::forward <F> (check));
			if (candidate.index != npos) {
				return candidate;
			}
		}
		if (l < tr && mid > tl) {
			Index_item_pair candidate = this->m_find_last(tl, tr, (now << 1) + 1, l, mid, std::forward <F> (check));
			if (candidate.index != npos) {
				return candidate;
			}
		}
		return { npos, A { } };
	}

};
