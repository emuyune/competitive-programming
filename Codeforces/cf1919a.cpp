#include <bits/stdc++.h>

using namespace std;
using ll = long long;

#ifdef _DEBUG
#include "utils/debug.h"
#else
#define debug(...)
#endif

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int T; cin >> T;
	while (T--) [&]() {
		int a, b; cin >> a >> b;
		cout << ((a+b) % 2 == 1 ? "Alice" : "Bob") << '\n';
	}();
}
