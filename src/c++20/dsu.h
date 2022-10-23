#pragma once

#include <vector>
#include <numeric>
#include <cassert>
#include <iostream>

class Dsu {
	
public:
	
	inline Dsu(const unsigned int _size = 0) noexcept :
	m_parent(_size),
	m_size(_size, 1),
	m_rank(_size, 0)
	{
		std::iota(this->m_parent.begin(), this->m_parent.end(), 0);
	}
	
	inline void grow(const unsigned int new_size) noexcept {
		assert(new_size >= static_cast <unsigned int> (this->m_parent.size()));
		const unsigned int old_size = this->m_parent.size();
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
	
	inline void reset(const unsigned int new_size) noexcept {
		if (new_size != static_cast <unsigned int> (this->m_parent.size())) {
			this->m_parent.resize(new_size);
			this->m_size.resize(new_size);
			this->m_rank.resize(new_size);
		}
		std::iota(this->m_parent.begin(), this->m_parent.end(), 0);
		this->m_size.assign(this->m_size.size(), 1);
		this->m_rank.assign(this->m_rank.size(), 0);
	}
	
	inline void reset() noexcept {
		this->reset(this->m_parent.size());
	}
	
	inline unsigned int size() const noexcept {
		return this->m_parent.size();
	}
	
	inline unsigned int size(const unsigned int vertex) noexcept {
		return this->m_size[this->find(vertex)];
	}
	
	inline unsigned int rank(const unsigned int vertex) noexcept {
		return this->m_rank[this->find(vertex)];
	}
	
	inline unsigned int find(const unsigned int vertex) noexcept {
		if (vertex >= static_cast <unsigned int> (this->m_parent.size())) [[unlikely]] {
			this->grow(vertex);
		}
		return vertex == this->m_parent[vertex] ? vertex : (this->m_parent[vertex] = this->find(this->m_parent[vertex]));
	}
	
	inline unsigned int operator [] (const unsigned int vertex) noexcept {
		return this->find(vertex);
	}
	
	template <bool COMPRESS = true> inline bool unite(unsigned int vertex0, unsigned int vertex1) noexcept;
	
	inline void flush(const unsigned int begin, const unsigned int end) noexcept {
		assert(end <= static_cast <unsigned int> (this->m_parent.size()));
		for (unsigned int vertex = begin; vertex < end; vertex++) {
			this->find(vertex);
		}
	}
	
	inline void flush() noexcept {
		this->flush(0, this->m_parent.size());
	}
	
	friend inline std::ostream& operator << (std::ostream& stream, Dsu& dsu) {
		stream << "{ ";
		for (unsigned int i = 0; i < dsu.size(); i++) {
			stream << dsu[i] << (i + 1 == dsu.size() ? " }" : ", ");
		}
		return stream;
	}
	
private:
	
	std::vector <unsigned int> m_parent;
	std::vector <unsigned int> m_size;
	std::vector <unsigned int> m_rank;
	
};

template <> inline bool Dsu::unite <true> (unsigned int vertex0, unsigned int vertex1) noexcept {
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

template <> inline bool Dsu::unite <false> (unsigned int make_root, unsigned int make_child) noexcept {
	if ((make_root = this->find(make_root)) == (make_child = this->find(make_child))) {
		return false;
	}
	this->m_parent[make_child] = make_root;
	this->m_rank[make_root] = std::max(this->m_rank[make_root], this->m_rank[make_child] + 1);
	this->m_size[make_root] += this->m_size[make_child];
	return true;
}
