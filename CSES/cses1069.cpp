#include <bits/stdc++.h>

using namespace std;
#define ms(x, a) memset(x, a, sizeof x)
typedef long long ll;
const int mod = 1e9 + 7, inf = 0x3f3f3f3f;
const int MN = 0;

int main(){
    cin.tie(0)->sync_with_stdio(0);
    string s; cin >> s;
    int N = s.size();
    s = " " + s;
    int cur = 0, ans = 0;
    for (int i = 1; i <= N; ++i){
        if (s[i] != s[i - 1]) cur = 0;
        cur++;
        ans = max(ans, cur);
    }
    cout << ans << '\n';
}
