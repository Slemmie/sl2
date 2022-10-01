#pragma once

#include <limits>
#include <iostream>
#include <vector>

template <typename A> inline A inverse(A a, const A& b) noexcept {
	if (!(a %= b)) {
		return b == static_cast <A> (1) ? static_cast <A> (0) : static_cast <A> (-1);
	}
	A c = inverse(b, a);
	return c == static_cast <A> (-1) ? static_cast <A> (-1) :
	static_cast <A> (((1LL - static_cast <long long> (b) * c) / a + b) % b);
}

template <int MOD, bool IS_PRIME = false> class Mint {
	
public:
	
	constexpr explicit operator int () const noexcept {
		return this->m_value;
	}
	
	constexpr Mint(const int _value = 0) noexcept : m_value(m_fix(_value)) { }
	
	template <typename A> inline Mint(const A& _value) noexcept {
		const long long llvalue = static_cast <long long> (_value);
		if (-(static_cast <long long> (MOD)) > llvalue || llvalue > static_cast <long long> (MOD)) {
			this->m_value = m_fix(static_cast <int> (llvalue % MOD));
			return;
		}
		this->m_value = m_fix(static_cast <int> (llvalue));
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
		return Mint <MOD, IS_PRIME> (-this->m_value);
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
		this->m_value = static_cast <long long> (this->m_value) * (int) other % MOD;
		return *this;
	}
	
	inline Mint <MOD, IS_PRIME>& operator /= (const Mint <MOD, IS_PRIME>& other) noexcept {
		(*this) *= inverse(other);
		return *this;
	}
	
	friend inline Mint <MOD, IS_PRIME> inverse(const Mint <MOD, IS_PRIME>& mint) noexcept {
		if (IS_PRIME) {
			return pow(mint, MOD - 2);
		}
		static Mint <MOD, IS_PRIME> s_inverse = inverse((int) mint, MOD);
		return s_inverse;
	}
	
	template <typename A> static inline Mint <MOD, IS_PRIME> pow(Mint <MOD, IS_PRIME> mint, A exponent) noexcept {
		static_assert (std::is_fundamental <A>::value);
		if (exponent < static_cast <A> (0)) {
			return inverse(pow(mint, -exponent));
		}
		Mint <MOD, IS_PRIME> result(1);
		while (exponent) {
			if (exponent & 1) {
				result *= mint;
			}
			mint *= mint;
			exponent >>= 1;
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
		return stream >> mint.m_value;
	}
	
private:
	
	int m_value;
	
	constexpr int m_fix(int value) const noexcept {
		if (value < 0) {
			value = (value % MOD) + MOD;
		}
		if (value >= MOD) {
			if constexpr (((std::numeric_limits <int>::max() >> 1) | 1) < MOD) {
				value -= MOD;
			} else {
				value %= MOD;
			}
		}
		return value;
	}
	
};

#ifdef mi9
typedef Mint <998244353, true> mi;
#else
typedef Mint <1000000007, true> mi;
#endif
