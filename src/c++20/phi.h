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
	if (x > static_cast <A> (1)) {
		return result - result / x;
	}
	return result;
}
