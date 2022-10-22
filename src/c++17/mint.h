#pragma once

#include <sl2/c++17/phi.h>

#include <limits>
#include <iostream>
#include <vector>

template <int MOD, bool IS_PRIME = false> class Mint {
	
public:
	
	constexpr explicit operator int () const noexcept {
		return this->m_value;
	}
	
	constexpr Mint(const int _value = 0) noexcept : m_value(this->m_fix(_value)) { }
	
	constexpr Mint(const Mint& _mint) noexcept : m_value(_mint.m_value) { }
	
	template <typename A> inline Mint(const A& _value) noexcept {
		if (_value < static_cast <A> (MOD) && _value >= static_cast <A> (0)) {
			this->m_value = (int) _value;
			return;
		}
		static constexpr const __uint128_t niv = ~1ULL / MOD;
		long long llvalue = (long long) _value - ((niv * _value) >> 64LL) * MOD;
		if (llvalue < 0LL) {
			llvalue += MOD;
		}
		if (llvalue >= MOD) {
			llvalue -= MOD;
		}
		this->m_value = static_cast <int> (llvalue);
	}
	
	constexpr bool operator == (const Mint <MOD, IS_PRIME>& other) const noexcept {
		return (int) (*this) == (int) other;
	}
	
	constexpr bool operator != (const Mint <MOD, IS_PRIME>& other) const noexcept {
		return (int) (*this) != (int) other;
	}
	
	constexpr bool operator < (const Mint <MOD, IS_PRIME>& other) const noexcept {
		return (int) (*this) < (int) other;
	}
	
	constexpr Mint <MOD, IS_PRIME> operator - () const noexcept {
		return Mint <MOD, IS_PRIME> (-this->m_value + MOD);
	}
	
	inline Mint <MOD, IS_PRIME>& operator += (const Mint <MOD, IS_PRIME>& other) noexcept {
		if ((this->m_value += (int) other) >= MOD) {
			this->m_value -= MOD;
		}
		return *this;
	}
	
	inline Mint <MOD, IS_PRIME>& operator -= (const Mint <MOD, IS_PRIME>& other) noexcept {
		if ((this->m_value -= (int) other) < 0) {
			this->m_value += MOD;
		}
		return *this;
	}
	
	inline Mint <MOD, IS_PRIME>& operator *= (const Mint <MOD, IS_PRIME>& other) noexcept {
		long long value = static_cast <long long> (this->m_value) * (int) other;
		if (value < MOD && value >= 0LL) {
			this->m_value = value;
			return *this;
		}
		static constexpr const __uint128_t niv = ~1ULL / MOD;
		value -= ((niv * value) >> 64LL) * MOD;
		if (value < 0LL) {
			value += MOD;
		}
		if (value >= MOD) {
			value -= MOD;
		}
		this->m_value = static_cast <int> (value);
		return *this;
	}
	
	inline Mint <MOD, IS_PRIME>& operator /= (const Mint <MOD, IS_PRIME>& other) noexcept {
		(*this) *= inverse(other);
		return *this;
	}
	
	friend inline Mint <MOD, IS_PRIME> inverse(const Mint <MOD, IS_PRIME>& mint) noexcept {
		return pow(mint, m_phi - 1);
	}
	
	template <typename A> static inline Mint <MOD, IS_PRIME> pow(Mint <MOD, IS_PRIME> mint, A exponent) noexcept {
		static_assert (std::is_fundamental <A>::value);
		if (exponent < static_cast <A> (0)) {
			return pow(mint, exponent % m_phi + m_phi);
		}
		if (exponent >= m_phi) {
			exponent %= m_phi;
		}
		Mint <MOD, IS_PRIME> result(1);
		while (exponent) {
			if (exponent & static_cast <A> (1)) {
				result *= mint;
			}
			exponent >>= static_cast <A> (1);
			mint *= mint;
		}
		return result;
	}
	
	inline Mint <MOD, IS_PRIME>& operator ++ () noexcept {
		return *this += 1;
	}
	
	inline Mint <MOD, IS_PRIME>& operator -- () noexcept {
		return *this -= 1;
	}
	
	inline Mint <MOD, IS_PRIME> operator ++ (int) noexcept {
		return *this += 1;
	}
	
	inline Mint <MOD, IS_PRIME> operator -- (int) noexcept {
		return *this -= 1;
	}
	
	friend inline Mint <MOD, IS_PRIME> operator + (Mint <MOD, IS_PRIME> mint, const Mint <MOD, IS_PRIME>& other) noexcept {
		return mint += other;
	}
	
	friend inline Mint <MOD, IS_PRIME> operator - (Mint <MOD, IS_PRIME> mint, const Mint <MOD, IS_PRIME>& other) noexcept {
		return mint -= other;
	}
	
	friend inline Mint <MOD, IS_PRIME> operator * (Mint <MOD, IS_PRIME> mint, const Mint <MOD, IS_PRIME>& other) noexcept {
		return mint *= other;
	}
	
	friend inline Mint <MOD, IS_PRIME> operator / (Mint <MOD, IS_PRIME> mint, const Mint <MOD, IS_PRIME>& other) noexcept {
		return mint /= other;
	}
	
	static inline Mint <MOD, IS_PRIME> factorial(const size_t x) noexcept {
		if (static_cast <long long> (x) < 0LL) {
			return Mint <MOD, IS_PRIME> (0);
		}
		static size_t ready = static_cast <size_t> (1);
		static std::vector <Mint <MOD, IS_PRIME>> result(2, Mint <MOD, IS_PRIME> (1));
		if (x <= ready) {
			return result[x];
		}
		const size_t next = x + (x >> static_cast <size_t> (4));
		result.resize(next + 1);
		for (size_t i = ready + 1; i <= next; i++) {
			result[i] = result[i - 1] * Mint <MOD, IS_PRIME> (i);
		}
		ready = next;
		return result[x];
	}
	
	static inline Mint <MOD, IS_PRIME> inverse_factorial(const size_t x) noexcept {
		if (static_cast <long long> (x) < 0LL) {
			return Mint <MOD, IS_PRIME> (0);
		}
		static size_t ready = static_cast <size_t> (1);
		static std::vector <Mint <MOD, IS_PRIME>> result(2, Mint <MOD, IS_PRIME> (1));
		if (x <= ready) {
			return result[x];
		}
		const size_t next = x + (x >> static_cast <size_t> (4));
		result.resize(next + 1);
		result[next] = inverse(factorial(next));
		for (size_t i = next - 1; i > ready; i--) {
			result[i] = result[i + 1] * Mint <MOD, IS_PRIME> (i + 1);
		}
		ready = next;
		return result[x];
	}
	
	static inline Mint <MOD, IS_PRIME> choose(const size_t n, const size_t k) noexcept {
		if (static_cast <long long> (k) < 0LL || k > n) {
			return Mint <MOD, IS_PRIME> (0);
		}
		return factorial(n) * inverse_factorial(k) * inverse_factorial(n - k);
	}
	
	friend inline std::ostream& operator << (std::ostream& stream, const Mint <MOD, IS_PRIME>& mint) {
		return stream << (int) mint;
	}
	
	friend inline std::istream& operator >> (std::istream& stream, Mint <MOD, IS_PRIME>& mint) {
		stream >> mint.m_value;
		mint.m_value = mint.m_fix(mint.m_value);
		return stream;
	}
	
	template <std::istream& STREAM = std::cin> inline void readl() {
		long long value;
		STREAM >> value;
		if (value < MOD && value >= 0LL) {
			this->m_value = value;
			return;
		}
		static constexpr const __uint128_t niv = ~1ULL / MOD;
		value -= ((niv * value) >> 64LL) * MOD;
		if (value < 0LL) {
			value += MOD;
		}
		if (value >= MOD) {
			value -= MOD;
		}
		this->m_value = static_cast <int> (value);
	}
	
private:
	
	int m_value;
		
	static constexpr int m_phi = IS_PRIME ? MOD - 1 : phi <int> (MOD);
	
	constexpr int m_fix(int value) const noexcept {
		if (value < MOD && value >= 0) {
			return value;
		}
		if (value < 0) {
			value = (value % MOD) + MOD;
		}
		if (value >= MOD) {
			if constexpr ((std::numeric_limits <int>::max() >> 2) < MOD) {
				while (value >= MOD) {
					value -= MOD;
				}
			} else {
				value %= MOD;
			}
		}
		return value;
	}
	
};

#ifndef no_mi
#ifdef mi9
typedef Mint <998244353, true> mi;
#else
typedef Mint <1000000007, true> mi;
#endif
#endif
