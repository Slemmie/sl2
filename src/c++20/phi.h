#pragma once

template <typename A> constexpr A phi(A x) noexcept {
	A result = x;
	for (A i = static_cast <A> (2); i * i <= x; i++) {
		if (!(x % i)) {
			while (!(x % i)) {
				x /= i;
			}
			result -= result / i;
		}
	}
	return x > static_cast <A> (1) ? result - result / x : result;
}
