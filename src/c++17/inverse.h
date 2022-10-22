#pragma once

template <typename A> inline A inverse(A a, A b) noexcept {
	if (b < static_cast <A> (2)) {
		return static_cast <A> (0);
	}
	A b0 = b;
	A x = static_cast <A> (1), y = static_cast <A> (0);
	while (static_cast <A> (1) < a) {
		if (!(static_cast <A> (0) < b)) {
			return static_cast <A> (0);
		}
		A quo = a / b;
		A t = b;
		b = a % b;
		a = t;
		t = y;
		y = x - quo * y;
		x = t;
	}
	if (x < static_cast <A> (0)) {
		x += b0;
	}
	return x;
}
