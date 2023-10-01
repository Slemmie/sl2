#pragma once

#include "phi.h"

#include <limits>
#include <iostream>
#include <vector>

template <int MOD, bool IS_PRIME = false> class Mint {

public:

	constexpr Mint(int _value = 0) noexcept : m_value(this->m_fix(_value)) { }
	template <typename A> constexpr Mint(const A& _value) noexcept : m_value(this->m_fix(static_cast <long long> (_value))) { }
	constexpr Mint(const Mint& _mint) noexcept : m_value(_mint.m_value) { }

	constexpr explicit operator int () const noexcept {
		return this->m_value;
	}

	constexpr bool operator == (const Mint <MOD, IS_PRIME>& rhs) const noexcept {
		return (int) *this == (int) rhs;
	}

	constexpr bool operator != (const Mint <MOD, IS_PRIME>& rhs) const noexcept {
		return !(*this == rhs);
	}

	constexpr std::strong_ordering operator <=> (const Mint <MOD, IS_PRIME>& rhs) {
		return (int) *this < (int) rhs ? std::strong_ordering::less :
		(int) *this > (int) rhs ? std::strong_ordering::greater : std::strong_ordering::equivalent;
	}

	constexpr Mint <MOD, IS_PRIME> operator - () const noexcept {
		return Mint <MOD, IS_PRIME> (-this->m_value + MOD);
	}

	constexpr Mint <MOD, IS_PRIME>& operator += (const Mint <MOD, IS_PRIME>& rhs) noexcept {
		this->m_value -= (this->m_value += rhs.m_value) < MOD ? 0 : MOD;
		return *this;
	}

	constexpr Mint <MOD, IS_PRIME>& operator -= (const Mint <MOD, IS_PRIME>& rhs) noexcept {
		this->m_value += (this->m_value -= rhs.m_value) < 0 ? MOD : 0;
		return *this;
	}

	constexpr Mint <MOD, IS_PRIME>& operator *= (const Mint <MOD, IS_PRIME>& rhs) noexcept {
		this->m_value = this->m_fix(static_cast <long long> (this->m_value) * (int) rhs);
		return *this;
	}

	constexpr Mint <MOD, IS_PRIME>& operator /= (const Mint <MOD, IS_PRIME>& rhs) noexcept {
		return *this *= inverse(rhs);
	}

	friend constexpr Mint <MOD, IS_PRIME> inverse(const Mint <MOD, IS_PRIME>& den) noexcept {
		return pow(den, m_phi - 1);
	}

	template <typename A> static constexpr Mint <MOD, IS_PRIME> pow(Mint <MOD, IS_PRIME> base, A exponent) noexcept {
		static_assert(std::is_integral_v <A>);
		if (exponent < static_cast <A> (0)) {
			return pow(base, exponent % m_phi + m_phi);
		}
		if (exponent >= m_phi) {
			exponent %= m_phi;
		}
		Mint <MOD, IS_PRIME> result(1);
		while (exponent) {
			if (exponent & static_cast <A> (1)) {
				result *= base;
			}
			exponent >>= static_cast <A> (1);
			base *= base;
		}
		return result;
	}

	constexpr Mint <MOD, IS_PRIME>& operator ++ () noexcept {
		return *this += 1;
	}

	constexpr Mint <MOD, IS_PRIME> operator ++ (int) noexcept {
		Mint <MOD, IS_PRIME> result = *this;
		++*this;
		return result;
	}

	constexpr Mint <MOD, IS_PRIME>& operator -- () noexcept {
		return *this -= 1;
	}

	constexpr Mint <MOD, IS_PRIME> operator -- (int) noexcept {
		Mint <MOD, IS_PRIME> result = *this;
		--*this;
		return result;
	}

	friend constexpr Mint <MOD, IS_PRIME> operator + (Mint <MOD, IS_PRIME> mint, const Mint <MOD, IS_PRIME>& other) noexcept {
		return mint += other;
	}

	friend constexpr Mint <MOD, IS_PRIME> operator - (Mint <MOD, IS_PRIME> mint, const Mint <MOD, IS_PRIME>& other) noexcept {
		return mint -= other;
	}

	friend constexpr Mint <MOD, IS_PRIME> operator * (Mint <MOD, IS_PRIME> mint, const Mint <MOD, IS_PRIME>& other) noexcept {
		return mint *= other;
	}

	friend constexpr Mint <MOD, IS_PRIME> operator / (Mint <MOD, IS_PRIME> mint, const Mint <MOD, IS_PRIME>& other) noexcept {
		return mint /= other;
	}

	static inline Mint <MOD, IS_PRIME> fac(size_t x) noexcept {
		if (x >= (1ULL << (sizeof(size_t) * 8 - 1))) [[unlikely]] {
			return Mint <MOD, IS_PRIME> (0);
		}
		static std::vector <Mint <MOD, IS_PRIME>> result(2, Mint <MOD, IS_PRIME> (1));
		if (x < result.size()) [[likely]] {
			return result[x];
		}
		const size_t next = x + (x >> static_cast <size_t> (4));
		const size_t prev_size = result.size();
		result.resize(next + 1);
		for (size_t i = prev_size; i <= next; i++) {
			result[i] = result[i - 1] * Mint <MOD, IS_PRIME> (i);
		}
		return result[x];
	}

	static inline Mint <MOD, IS_PRIME> inv_fac(size_t x) noexcept {
		if (x >= (1ULL << (sizeof(size_t) * 8 - 1))) [[unlikely]] {
			return Mint <MOD, IS_PRIME> (0);
		}
		static std::vector <Mint <MOD, IS_PRIME>> result(2, Mint <MOD, IS_PRIME> (1));
		if (x < result.size()) [[likely]] {
			return result[x];
		}
		const size_t next = x + (x >> static_cast <size_t> (4));
		const size_t prev_size = result.size();
		result.resize(next + 1);
		result[next] = inverse(fac(next));
		for (size_t i = next - 1; i >= prev_size; i--) {
			result[i] = result[i + 1] * Mint <MOD, IS_PRIME> (i + 1);
		}
		return result[x];
	}

	static inline Mint <MOD, IS_PRIME> choose(size_t n, size_t k) noexcept {
		if (k > n || n >= (1ULL << (sizeof(size_t) * 8 - 1))) [[unlikely]] {
			return Mint <MOD, IS_PRIME> (0);
		}
		return fac(n) * inv_fac(k) * inv_fac(n - k);
	}

	friend constexpr std::ostream& operator << (std::ostream& stream, const Mint <MOD, IS_PRIME>& mint) {
		return stream << (int) mint;
	}

	friend constexpr std::istream& operator >> (std::istream& stream, Mint <MOD, IS_PRIME>& mint) {
		stream >> mint.m_value;
		mint.m_value = mint.m_fix(mint.m_value);
		return stream;
	}

	template <std::istream& STREAM = std::cin> constexpr void readl() {
		long long i64v;
		STREAM >> i64v;
		this->m_value = this->m_fix(i64v);
	}

private:

	int m_value;

	static constexpr const int m_phi = IS_PRIME ? MOD - 1 : phi <int> (MOD);
	static constexpr const __int128_t m_niv = ~1ULL / MOD;

	constexpr int m_fix(int i32v) const noexcept {
		if (i32v < MOD && i32v >= 0) {
			return i32v;
		}
		if (i32v < 0) {
			i32v = (i32v % MOD) + MOD;
		}
		if (i32v >= MOD) {
			if constexpr ((std::numeric_limits <int>::max() >> 2) < MOD) {
				while (i32v >= MOD) {
					i32v -= MOD;
				}
			} else {
				i32v %= MOD;
			}
		}
		return i32v;
	}

	constexpr int m_fix(long long i64v) const noexcept {
		if (0LL <= i64v && i64v < MOD) {
			return i64v;
		}
		i64v -= ((m_niv * i64v) >> 64) * MOD;
		i64v += i64v < 0 ? MOD : 0;
		i64v -= i64v < MOD ? 0 : MOD;
		return i64v;
	}

};

#ifndef no_mi
#ifdef mi9
typedef Mint <998244353, true> mi;
#else
typedef Mint <1000000007, true> mi;
#endif
#endif
