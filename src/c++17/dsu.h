#pragma once

#include <vector>
#include <numeric>
#include <cassert>
#include <iostream>

class Dsu {

	using size_type = unsigned int;

public:

	inline Dsu(size_type _size = 0) :
	m_parent(_size),
	m_size(_size, 1),
	m_rank(_size, 0)
	{
		std::iota(this->m_parent.begin(), this->m_parent.end(), 0);
	}

	inline void grow(size_type new_size) {
		if (new_size <= this->size()) {
			return;
		}
		const size_type old_size = this->m_parent.size();
		this->m_parent.resize(new_size);
		std::iota(this->m_parent.begin() + old_size, this->m_parent.end(), old_size);
		this->m_size.resize(new_size, 1);
		this->m_rank.resize(new_size, 0);
	}

	inline void clear() noexcept {
		this->m_parent.clear();
		this->m_size.clear();
		this->m_rank.clear();
	}

	inline void reset(size_type new_size) {
		if (new_size != static_cast <size_type> (this->m_parent.size())) {
			this->m_parent.resize(new_size);
			this->m_size.resize(new_size);
			this->m_rank.resize(new_size);
		}
		std::iota(this->m_parent.begin(), this->m_parent.end(), 0);
		this->m_size.assign(this->m_size.size(), 1);
		this->m_rank.assign(this->m_rank.size(), 0);
	}

	inline void reset() {
		this->reset(this->m_parent.size());
	}

	inline size_type size() const noexcept {
		return this->m_parent.size();
	}

	inline size_type size(size_type vertex) {
		return this->m_size[this->find(vertex)];
	}

	inline size_type rank(size_type vertex) {
		return this->m_rank[this->find(vertex)];
	}

	inline size_type find(size_type vertex) {
		if (vertex >= static_cast <size_type> (this->m_parent.size())) [[unlikely]] {
			this->grow(vertex + 1);
		}
		return vertex == this->m_parent[vertex] ? vertex : (this->m_parent[vertex] = this->find(this->m_parent[vertex]));
	}

	inline size_type operator [] (size_type vertex) {
		return this->find(vertex);
	}

	template <bool COMPRESS = true> inline bool unite(size_type vertex0, size_type vertex1);

	constexpr void flush(size_type begin, size_type end) {
		for (size_type vertex = begin; vertex < end; vertex++) {
			this->find(vertex);
		}
	}

	inline void flush() {
		this->flush(0, this->m_parent.size());
	}

	friend inline std::ostream& operator << (std::ostream& stream, Dsu& dsu) {
		stream << "{ ";
		for (size_type i = 0; i < dsu.size(); i++) {
			stream << dsu[i] << (i + 1 == dsu.size() ? " }" : ", ");
		}
		return stream;
	}

private:

	std::vector <size_type> m_parent;
	std::vector <size_type> m_size;
	std::vector <size_type> m_rank;

};

template <> inline bool Dsu::unite <true> (size_type vertex0, size_type vertex1) {
	if ((vertex0 = this->find(vertex0)) == (vertex1 = this->find(vertex1))) {
		return false;
	}
	if (this->m_rank[vertex0] < this->m_rank[vertex1]) {
		std::swap(vertex0, vertex1);
	}
	this->m_parent[vertex1] = vertex0;
	this->m_rank[vertex0] += this->m_rank[vertex0] == this->m_rank[vertex1];
	this->m_size[vertex0] += this->m_size[vertex1];
	return true;
}

template <> inline bool Dsu::unite <false> (size_type make_root, size_type make_child) {
	if ((make_root = this->find(make_root)) == (make_child = this->find(make_child))) {
		return false;
	}
	this->m_parent[make_child] = make_root;
	this->m_rank[make_root] = std::max(this->m_rank[make_root], this->m_rank[make_child] + 1);
	this->m_size[make_root] += this->m_size[make_child];
	return true;
}

