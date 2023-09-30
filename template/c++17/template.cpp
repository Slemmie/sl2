// hilo

#if 0
constexpr const bool is_multi_tests = true;
#else
constexpr const bool is_multi_tests = false;
#endif

#include <bits/stdc++.h>

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;

void prep(const int numt);
void test_case(const int curt);

int main() {
	std::ios::sync_with_stdio(0); std::cin.tie(0);
	if constexpr (is_multi_tests) {
		int t;
		std::cin >> t;
		prep(t);
		for (int i = 1; i <= t; i++) {
			test_case(t);
		}
	} else {
		prep(1);
		test_case(0);
	}
}

void prep(const int numt) {
}

void test_case(const int curt) {
}
