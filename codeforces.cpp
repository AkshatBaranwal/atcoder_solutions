#include "bits/stdc++.h"
using namespace std;

#define eps 1e-8
#define F first
#define S second
#define vec vector
#define ll long long
#define ld long double
#define nl cout << '\n';
#define chk(x) cout << (#x) << " is " << (x) << endl;
#define chk2(x, y) cout << (x) << " " << (y) << endl;
#define chk3(x, y, z) cout << (x) << " " << (y) << " " << (z) << endl;
#define chkp(x) chk2(x.F, x.S);

void solve() {
    long long n, m, k, i, x;
    cin >> n >> m >> k;
    vector<long long> a;

    for (i = 0; i < n; ++i) {
        cin >> x;
        a.push_back((m - x % m) % m);
    }

    sort(a.begin(), a.end());

    for (i = 0; i < n; ++i)
        cout << a[i] << " ";

    for (i = 0; i < n; ++i)
        if (a[i] <= k)
            k -= a[i];
        else
            break;

    nl
    cout << i << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    int t = 1, i;
    // cin >> t;

    for (i = 1; i <= t; ++i)
        solve();

    return 0;
}