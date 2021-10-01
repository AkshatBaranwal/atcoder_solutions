#define ll long long
#define inf ((ll)1e18 + 1)
#define ii pair<int,int>
#define minheap(T) priority_queue<T, vector<T>, greater<T>>
#define maxheap(T) priority_queue<T>
#define fi first
#define se second
#define vi vector<int>
#define vvi vector<vi>
#define pb push_back
#define ppb pop_back
#define rep(i,a,b) for (i=(a);i<(b);++i)
#define nrep(i,a,b) for (i=(a);i>(b);--i)
#define chk(x) cout << (#x) << " is " << (x) << endl;
#define chk2(x, y) cout << (x) << " " << (y) << endl;
#define chk3(x, y, z) cout << (x) << " " << (y) << " " << (z) << endl;
#define chkp(x) chk2(x.first, x.second);
#define nl cout << endl;

// BINARY EXPONENTIATION
ll bxp(ll a, ll b, ll m) {
    ll r = 1;

    while (b) {
        if (b & 1)
            r = (r * a) % m;

        a = (a * a) % m;
        b /= 2;
    }

    return r;
}

// BINARY MATRIX EXPONENTIATION
vector<vector<ll>> matmul(vector<vector<ll>> &x, vector<vector<ll>> &y) {
    int n = x.size(), m = y[0].size(), o = y.size();
    vector<vector<ll>> z(n, vector<ll>(m, 0));

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            for (int k = 0; k < o; ++k)
                z[i][j] += x[i][k] * y[k][j];

    return z;
}
vector<vector<ll>> bxp_matrix(vector<vector<ll>> a, ll b) {
    int n = a.size();
    vector<vector<ll>> r(n, vector<ll>(n, 0));

    while (n--)
        r[n][n] = 1;

    while (b > 0) {
        if (b & 1)
            r = matmul(r, a);

        a = matmul(a, a);
        b /= 2;
    }

    return r;
}

// MULTIPLY BIG NUMBERS
ll mult(ll a, ll b, ll m) {
    ll r = 0;

    while (b) {
        if (b & 1)
            r = (r + a) % m;

        a = (a + a) % m;
        b /= 2;
    }

    return r;
}

// QUICK SORT
void swap(int *a, int *b) {
    int x = *a;
    *a = *b;
    *b = x;
}
int partition(int *a, int low, int high) {
    int i = low - 1;

    for (int j = low; j < high; ++j)
        if (a[j] < a[high]) ++i, swap(&a[j], &a[i]);

    swap(&a[high], &a[++i]);
    return i;
}
void sort(int *a, int low, int high) {
    if (low < high) {
        int k = partition(a, low, high);
        sort(a, low, k - 1);
        sort(a, k + 1, high);
    }
}

// SUFFIX ARRAY
string str;
vector<vector<int> > c;
vector<int> p, lcs;
void count_sort(int m) {
    int n = p.size(), i;
    vector<int> pos(n, 0), p_new(n);

    for (i = 0; i < n; ++i)
        pos[c[m][i]]++;

    for (i = 1; i < n; ++i)
        pos[i] += pos[i - 1];

    pos.insert(pos.begin(), 0);

    for (auto k : p) {
        p_new[pos[c[m][k]]] = k;
        ++pos[c[m][k]];
    }

    p = p_new;
}
void suffixarray() {
    // c[k][i] -> position of suffix i of length 2^k
    // p[j] -> suffix at position j
    // c[c.size()-1][p[i]] = i
    // p[c[j]] = j
    str += "#";
    int n = str.size(), m = 0, i, k = 0;
    vector<int> c_new(n);
    vector<pair<char, int>> a(n);

    for (i = 0; i < n; ++i)
        a[i] = {str[i], i};

    sort(a.begin(), a.end());

    for (i = 0; i < n; ++i)
        p[i] = a[i].second;

    for (c_new[p[0]] = 0, i = 1; i < n; ++i)
        if (a[i].first == a[i - 1].first)
            c_new[p[i]] = c_new[p[i - 1]];
        else
            c_new[p[i]] = c_new[p[i - 1]] + 1;

    c.push_back(c_new);

    while ((1 << k) < n) {
        for (i = 0; i < n; ++i)
            p[i] = (p[i] - (1 << k) + n) % n;

        count_sort(m);

        for (c_new[p[0]] = 0, i = 1; i < n; ++i)
            if (c[m][p[i - 1]] == c[m][p[i]] &&
                    c[m][(p[i - 1] + (1 << k)) % n] == c[m][(p[i] + (1 << k)) % n])
                c_new[p[i]] = c_new[p[i - 1]];
            else
                c_new[p[i]] = c_new[p[i - 1]] + 1;

        ++k;
        c.push_back(c_new);
        ++m;
    }
}
void lcp() {
    int m = c.size() - 1;
    int i, n = p.size(), k = 0, j;

    for (i = 0; i < n - 1; ++i) {
        j = p[c[m][i] - 1];

        while (str[i + k] == str[j + k]) ++k;

        lcs[c[m][i]] = k;
        k = max(k - 1, 0);
    }
}
int compare(int i, int j, int k, int l) {
    int n = p.size();
    pair<int, int> a = {c[k][i], c[k][(i + l - (1 << k)) % n]};
    pair<int, int> b = {c[k][j], c[k][(j + l - (1 << k)) % n]};
    return a == b ? 0 : a < b ? -1 : 1;
}
bool comp(const pair<int, int> &a,
          const pair<int, int> &b) { // compare any suffix
    int la = a.second - a.first + 1, lb = b.second - b.first + 1;
    int l = min(la, lb);
    int k = int(log2(l));

    if (compare(a.first, b.first, k, l) == -1) return true;

    if (compare(a.first, b.first, k, l) == 1) return false;

    if (la == lb) {
        if (a.first == b.first)
            return a.second < b.second;

        return a.first < b.first;
    }

    return la < lb;
}


// 9 BOX
void print(string &s) {
    for (int i = 0; i < 9; ++i) {
        if (s[i] == '9') cout << "  ";
        else cout << s[i] << " ";

        if (i % 3 == 2) cout << endl;
    }

    cout << endl;
}
int main() {
    int i, n, t;
    string s, p;
    char c;
    queue<string> q;
    map<string, pair<string, int>> mp;
    q.push("123456789");
    mp["123456789"] = {"123456789", 0};

    while (!q.empty()) {
        s = q.front();
        q.pop();
        n = mp[s].second;

        for (i = 0; s[i] != '9'; ++i);

        if (i > 2) {
            p = s;
            c = p[i];
            p[i] = p[i - 3];
            p[i - 3] = c;

            if (mp.find(p) == mp.end()) {
                q.push(p);
                mp[p] = {s, n + 1};
            }
        }

        if (i < 6) {
            p = s;
            c = p[i];
            p[i] = p[i + 3];
            p[i + 3] = c;

            if (mp.find(p) == mp.end()) {
                q.push(p);
                mp[p] = {s, n + 1};
            }
        }

        if (i % 3 != 0) {
            p = s;
            c = p[i];
            p[i] = p[i - 1];
            p[i - 1] = c;

            if (mp.find(p) == mp.end()) {
                q.push(p);
                mp[p] = {s, n + 1};
            }
        }

        if (i % 3 != 2) {
            p = s;
            c = p[i];
            p[i] = p[i + 1];
            p[i + 1] = c;

            if (mp.find(p) == mp.end()) {
                q.push(p);
                mp[p] = {s, n + 1};
            }
        }
    }

    cin >> s;

    if (mp.find(s) == mp.end())
        cout << "Invalid Case" << "\n\n";
    else {
        cout << "\n" << "Minimum Moves: " << mp[s].second << "\n\n";

        while (s != mp[s].first) {
            print(s);

            for (i = 0; s[i] != '9'; ++i);

            cout << "Move " << mp[s].first[i] << "\n\n";
            s = mp[s].first;
        }

        print(s);
    }

    return 0;
}

// SEARCH
bool search(vector<int> &p, string &ptr, string &str) {
    int low = 0, high = p.size() - 1, mid;

    while (low <= high) {
        mid = (high + low) / 2;

        if (ptr < str.substr(p[mid], ptr.size()))
            high = mid - 1;
        else if (ptr > str.substr(p[mid], ptr.size()))
            low = mid + 1;
        else
            return true;
    }

    return false;
}
int lowerbound(vector<int> &p, string &ptr, string &str) {
    int low = 0, high = p.size(), mid;

    while (low < high) {
        mid = (high + low) / 2;

        if (ptr <= str.substr(p[mid], ptr.size()))
            high = mid;
        else
            low = mid + 1;
    }

    return low;
}
int upperbound(vector<int> &p, string &ptr, string &str) {
    int low = 0, high = p.size(), mid;

    while (low < high) {
        mid = (high + low) / 2;

        if (ptr >= str.substr(p[mid], ptr.size()))
            low = mid + 1;
        else
            high = mid;
    }

    return low;
}

int upperBound(int x, vector<int> &a) {
    int l = 0, r = a.size(), m;

    while (l < r) {
        m = l + (r - l) / 2;

        if (a[m] <= x)   // a[l] < x
            l = m + 1;
        else             // a[r] >= x
            r = m;
    }

    return r;
}
int lowerBound(int x, vector<int> &a) {
    int l = 0, r = a.size(), m;

    while (l < r) {
        m = l + (r - l) / 2;

        if (a[m] < x)   // a[l] < x
            l = m + 1;
        else            // a[r] >= x
            r = m;
    }

    return r;
}

// KMP
int solve() {
    string A = "text", B = "pattern";
    int i, j, k, m = A.size(), n = B.size();
    vector<int> x(n);

    for (i = 0, j = 0; i < n; ++i) {
        while (j > 0 && B[i] != B[j])
            j = x[j - 1];

        x[i] = j;

        if (B[i] == B[j])
            ++j;
    }

    for (i = 0, j = 0; i < m && j < n; ++i) {
        while (j > 0 && A[i] != B[j])
            j = x[j - 1];

        if (A[i] == B[j])
            ++j;
    }

    if (j == n) return i - j;

    return -1;
}

// Prettify JSON
string solve() {
    string A =
        "{'id':100,'firstName':'Jack','lastName':'Jones','age':12}";
    int i, n = A.size();
    char c = A[0], p;
    string B = "", tabs = "\t";
    B += c;
    B += '\n';
    B += tabs;

    for (i = 1; i < n; ++i) {
        if (A[i] == ' ') continue;

        p = c;
        c = A[i];

        if (c == '[' || c == '{') {
            if (p != ',' && p != '[' && p != '{') {
                B += "\n";
                B += tabs;
            }

            B += c;
            tabs += "\t";
            B += "\n";
            B += tabs;
        }
        else if (c == ',') {
            B += c;
            B += "\n";
            B += tabs;
        }
        else if (c == ']' || c == '}') {
            tabs = tabs.substr(0, tabs.size() - 1);
            B += "\n";
            B += tabs;
            B += c;
        }
        else
            B += c;
    }

    return B;
}

// SUDOKU
vector<int> check(vector<vector<int>> &k, pair<int, int> v) {
    vector<int> p(10, 0), q;
    int x = v.first, y = v.second, i, j;

    for (i = 0; i < 9; ++i) {
        p[k[x][i]] = 1;
        p[k[i][y]] = 1;
    }

    for (i = x - x % 3; i < x - x % 3 + 3; ++i)
        for (j = y - y % 3; j < y - y % 3 + 3; ++j)
            p[k[i][j]] = 1;

    for (i = 1; i < 10; ++i)
        if (p[i] == 0)
            q.push_back(i);

    return q;
}

void print(vector<vector<int>> &k) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            cout << k[i][j] << " ";

            if (j % 3 == 2)
                cout << " ";
        }

        cout << endl;

        if (i % 3 == 2)
            cout << endl;
    }

    cout << endl;
}

bool sudoku(vector<vector<int>> &k, vector<pair<int, int>> &v, int y) {
    if (y == v.size()) {
        print(k);
        return true;
    }

    bool flag = false;
    vector<int> p = check(k, v[y]);

    for (auto i : p) {
        k[v[y].first][v[y].second] = i;
        flag = sudoku(k, v, y + 1) || flag;

        // comment it to print all solutions
        if (flag) return true;

        k[v[y].first][v[y].second] = 0;
    }

    return flag;
}

auto solve() {
    vector<vector<int>> k = {
        {2, 0, 0,  0, 0, 0,  0, 0, 0},
        {0, 0, 3,  6, 0, 0,  0, 0, 0},
        {0, 7, 0,  0, 9, 0,  2, 0, 0},

        {0, 5, 0,  0, 0, 7,  0, 0, 0},
        {0, 0, 0,  0, 4, 5,  7, 0, 0},
        {0, 0, 0,  1, 0, 0,  0, 3, 0},

        {0, 0, 1,  0, 0, 0,  0, 6, 8},
        {0, 0, 8,  5, 0, 0,  0, 1, 0},
        {0, 9, 0,  0, 0, 0,  4, 0, 0}
    };
    vector<pair<int, int>> v;
    int i, j;

    for (i = 0; i < 9; ++i)
        for (j = 0; j < 9; ++j)
            if (k[i][j] == 0)
                v.push_back({i, j});

    if (!sudoku(k, v, 0))
        cout << "INVALID" << endl;

    return 0;
}

// SEGMENT TREE
void constructTree(vector<int> &segTree, int low, int high, int pos) {
    if (low == high) {
        segTree[pos] = 1;
        return;
    }

    int mid = low + (high - low) / 2;
    constructTree(segTree, low, mid, 2 * pos + 1);
    constructTree(segTree, mid + 1, high, 2 * pos + 2);
    segTree[pos] = segTree[pos * 2 + 1] + segTree[pos * 2 + 2];
}

void search(vector<int> &segTree, vector<int> &ans, pair<int, int> ai, int pos, int low, int high) {
    --segTree[pos];

    if (low == high) {
        ans[low] = ai.first;
        return;
    }

    int mid = low + (high - low) / 2;

    if (segTree[2 * pos + 1] > ai.second)
        search(segTree, ans, ai, pos * 2 + 1, low, mid);
    else {
        ai.second -= segTree[2 * pos + 1];
        search(segTree, ans, ai, pos * 2 + 2, mid + 1, high);
    }
}

// MERGE SORT with INVERSION COUNT
void merge(vector<pair<int, int> > &c, vector<int> &b, int low, int mid, int high) {
    vector<pair<int, int> > d;
    int i = low, j = mid + 1, dd = 0;

    while (i <= mid && j <= high) {
        if (c[i].first <= c[j].first) {
            b[c[i].second] += abs(i - low - dd++);
            d.push_back(c[i++]);
        }
        else {
            b[c[j].second] += abs(j - low - dd++);
            d.push_back(c[j++]);
        }
    }

    while (i <= mid) {
        b[c[i].second] += abs(i - low - dd++);
        d.push_back(c[i++]);
    }

    while (j <= high) {
        b[c[j].second] += abs(j - low - dd++);
        d.push_back(c[j++]);
    }

    for (i = low; i <= high; ++i)
        c[i] = d[i - low];
}

void mergesort(vector<pair<int, int> > &c, vector<int> &b, int low, int high) {
    if (low == high) return;

    int mid = low + (high - low) / 2;
    mergesort(c, b, low, mid);
    mergesort(c, b, mid + 1, high);
    merge(c, b, low, mid, high);
}

// TOPOLOGICAL SORT
void dfs(vector<vector<int> > &adj, vector<int> &visited, vector<int> &tp, int s) {
    visited[s] = true;

    for (auto v : adj[s]) {
        if (visited[s])
            continue;

        dfs(adj, visited, tp, v);
    }

    tp.push_back(s);
}

auto solve() {
    int n, i, e, v1, v2;
    cin >> n >> e;
    vector<int> visited(n + 1, false);
    vector<vector<int> > adj(n + 1);
    vector<int> tp;

    for (i = 0; i < e; ++i) {
        cin >> v1 >> v2;
        adj[v1].push_back(v2);
    }

    for (i = 1; i <= n; ++i)
        if (!visited[i])
            dfs(adj, visited, tp, i);

    for (i = 0; i < n; ++i)
        cout << tp[i] << endl;
}

// DJIKSTRA
void djikstra(int s, vector<vector<pair<int, int> > > &adj, vector<int> &d) {
    int v, dv, to, len, n = adj.size();
    d.resize(n, INT_MAX);
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
    d[s] = 0;
    pq.push({0, s});

    while (!pq.empty()) {
        v = pq.top().second;
        dv = pq.top().first;
        pq.pop();

        if (dv != d[v])
            continue;

        for (auto edge : adj[v]) {
            to = edge.first;
            len = edge.second;

            if (len + dv < d[to]) {
                d[to] = len + dv;
                pq.push({d[to], to});
            }
        }
    }
}

// PRIMS
void prim(int s, vector<vector<pair<int, int> > > &adj, vector<pair<int, int> > &edges) {
    int v, dv, to, w, n = adj.size() - 1;
    edges.resize(n + 1, { -1, INT_MAX});
    vector<int> visited(n + 1, false);
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
    pq.push({0, s});

    while (!pq.empty()) {
        v = pq.top().second;
        visited[v] = true;
        pq.pop();

        for (auto edge : adj[v]) {
            to = edge.first;
            w = edge.second;

            if (visited[to])
                continue;

            if (w < edges[to].second) {
                edges[to] = {v, w};
                pq.push({w, to});
            }
        }
    }
}

// DSU
void makeSet(int v, int d, vector<int> &parent, vector<int> &rank, vector<int> &data) {
    parent[v] = v;
    rank[v] = 0;
    data[v] = d;
}

int find_set(int v, vector<int> &parent) {
    if (v == parent[v])
        return v;

    return parent[v] = find_set(parent[v], parent);
}

void union_sets(int v1, int v2, vector<int> &parent, vector<int> &rank, vector<int> &data) {
    v1 = find_set(v1, parent);
    v2 = find_set(v2, parent);

    if (v1 != v2) {
        if (rank[v1] < rank[v2])
            swap(v1, v2);

        parent[v2] = v1;

        if (rank[v1] == rank[v2])
            ++rank[v1];
    }
}

// KRUSKAL
void makeSet(int v, vector<int> &parent, vector<int> &rank) {
    parent[v] = v;
    rank[v] = 0;
}

int find_set(int v, vector<int> &parent) {
    if (v == parent[v])
        return v;

    return parent[v] = find_set(parent[v], parent);
}

void union_sets(int v1, int v2, vector<int> &parent, vector<int> &rank) {
    v1 = find_set(v1, parent);
    v2 = find_set(v2, parent);

    if (v1 != v2) {
        if (rank[v1] < rank[v2])
            swap(v1, v2);

        parent[v2] = v1;

        if (rank[v1] == rank[v2])
            ++rank[v1];
    }
}

auto solve() {
    int n, e, v1, v2, i, w;
    cin >> n >> e;
    vector<pair<int, pair<int, int> > > edges;
    vector<int> parent(n + 1, -1), rank(n + 1, -1);
    vector<vector<pair<int, int> > > adj(n + 1);
    priority_queue < pair<int, pair<int, int> >, vector < pair<int, pair<int, int> > >,
                   greater<pair<int, pair<int, int> > > > pq;

    for (i = 0; i < e; ++i) {
        cin >> v1 >> v2 >> w;
        adj[v1].push_back({v2, w});
        adj[v2].push_back({v1, w});
        pq.push({w, {min(v1, v2), max(v1, v2)}});
    }

    for (i = 1; i <= n; ++i)
        makeSet(i, parent, rank);

    while (!pq.empty()) {
        w = pq.top().first;
        v1 = pq.top().second.first;
        v2 = pq.top().second.second;
        pq.pop();

        if (find_set(v1, parent) == find_set(v2, parent))
            continue;

        union_sets(v1, v2, parent, rank);
        edges.push_back({w, {v1, v2}});
    }

    for (auto k : edges)
        cout << k.second.first << " " << k.second.second << " " << k.first << endl;
}

// BELMAN FORD
void belmonFord(int s, vector<vector<pair<int, int> > > &adj, vector<int> &distance,
                vector<int> &parent,
                vector<pair<int, pair<int, int> > > &edges) {
    int i, n = adj.size();
    parent.resize(n, -1);
    distance.resize(n, INT_MAX);
    distance[s] = 0;
    parent[s] = s;

    for (i = 0; i < n - 2; ++i) {
        for (auto e : edges) {
            if (distance[e.second.second] > distance[e.second.first] + e.first) {
                distance[e.second.second] = distance[e.second.first] + e.first;
                parent[e.second.second] = e.second.first;
            }
        }
    }
}

// KOSARAJU'S STRONGLY CONNECTED COMPONENTS
void dfs1(int v, vector<vector<int> > &adj, vector<int> &visited, vector<int> &order) {
    visited[v] = true;

    for (auto u : adj[v])
        if (!visited[u])
            dfs1(u, adj, visited, order);

    order.push_back(v);
}

void dfs2(int v, vector<vector<int> > &adj, vector<int> &visited, vector<int> &component) {
    visited[v] = true;
    component.push_back(v);

    for (auto u : adj[v])
        if (!visited[u])
            dfs2(u, adj, visited, component);
}

auto solve() {
    int n, e, v1, v2, i;
    cin >> n >> e;
    vector<int> visited, order, component;
    vector<vector<int> > adj(n + 1), adj_rev(n + 1), components;

    for (i = 0; i < e; ++i) {
        cin >> v1 >> v2;
        adj[v1].push_back(v2);
        adj_rev[v2].push_back(v1);
    }

    visited.assign(n + 1, false);

    for (i = 1; i <= n; ++i)
        if (!visited[i])
            dfs1(i, adj, visited, order);

    visited.assign(n + 1, false);
    reverse(order.begin(), order.end());

    for (auto v : order)
        if (!visited[v]) {
            dfs2(v, adj_rev, visited, component);
            components.push_back(component);
            component.clear();
        }
}

// FLOYD WARSHALL
for (i = 0; i < n; ++i)
    for (j = 0; j < n; ++j)
        for (k = 0; k < n; ++k)
            if (d[i][j] > d[i][k] + d[k][j])
                d[i][j] = d[i][k] + d[k][j];

// CYCLE DETECTION IN DIRECTED GRAPH
bool dfs(int v, vector<vector<int> > &adj, vector<bool> &white, vector<bool> &gray,
         vector<bool> &black) {
    white[v] = false;
    gray[v] = true;

    for (auto u : adj[v]) {
        if (black[u])
            continue;

        if (gray[u])
            return true;

        if (dfs(u, adj, white, gray, black))
            return true;
    }

    gray[v] = false;
    black[v] = true;
    return false;
}

auto solve() {
    int n, e, v1, v2, i;
    cin >> n >> e;
    vector<vector<int> > adj(n + 1);
    vector<bool> white(n + 1, true), gray(n + 1, false), black(n + 1, false);

    for (i = 0; i < e; ++i) {
        cin >> v1 >> v2;
        adj[v1].push_back(v2);
    }

    for (i = 1; i <= n; ++i) {
        if (white[i]) {
            if (dfs(i, adj, white, gray, black))
                break;
        }
    }

    if (i <= n)
        cout << "CYCLE DETECTED";
    else
        cout << "NO CYCLE DETECTED";
}

// CYCLES IN UNDIRECTED GRAPH
bool dfs(int v, int p, vector<vector<int> > &adj, vector<bool> &visited) {
    visited[v] = true;

    for (auto u : adj[v]) {
        if (u == p)
            continue;

        if (visited[u])
            return true;

        if (dfs(u, v, adj, visited))
            return true;
    }

    return false;
}

// ARTICULATION POINTS and BRIDGES
void dfs(int v, int &timer, vector<vector<int> > &adj, vector<bool> &visited,
         vector<int> &tin,
         vector<int> &low, vector<int> &arp, vector<pair<int, int> > &bridge, int p = -1) {
    visited[v] = true;
    tin[v] = low[v] = timer++;
    int children = 0;

    for (auto u : adj[v]) {
        if (u == p)
            continue;

        if (visited[u])
            low[v] = min(low[v], tin[u]);
        else {
            dfs(u, timer, adj, visited, tin, low, arp, bridge, v);
            low[v] = min(low[v], low[u]);

            if (low[u] > tin[v])
                bridge.push_back({min(u, v), max(u, v)});

            if (low[u] >= tin[v] && p != -1)
                arp.push_back(v);

            ++children;
        }
    }

    if (p == -1 && children > 1)
        arp.push_back(v);
}

auto solve() {
    int n, e, v1, v2, i, timer = 0;
    cin >> n >> e;
    vector<vector<int> > adj(n + 1);
    vector<bool> visited(n + 1, false);
    vector<int> tin(n + 1, -1), low(n + 1, -1), arp;
    vector<pair<int, int> > bridge;

    for (i = 0; i < e; ++i) {
        cin >> v1 >> v2;
        adj[v1].push_back(v2);
        adj[v2].push_back(v1);
    }

    for (i = 1; i <= n; ++i)
        if (!visited[i])
            dfs(i, timer, adj, visited, tin, low, arp, bridge, -1);

    for (auto k : arp)
        cout << k << " ";

    cout << endl << endl;

    for (auto k : bridge)
        cout << k.first << " " << k.second << endl;
}

// NEXT PALINDROME
int i, j = -1, k = -1, l = -1, n = A.size(), m = (n - 1) / 2;

for (i = 0; i <= m; ++i) {
    if (A[i] > A[n - 1 - i]) j = i;

    if (A[i] < A[n - 1 - i]) k = i;

    if (A[i] != '9') l = i;
}

if (l == -1 && j == -1) {
    string x = "1";

    for (l = 0; l < n - 1; ++i)
        x += '0';

    x += '1';
    return x;
}

if (j <= k) {
    A[l] = A[l] + 1;

    for (j = l + 1; j <= m; ++j)
        A[j] = '0';

    for (j = 0; j < n / 2; ++j)
        A[n - 1 - j] = A[j];
}
else {
    for (i = 0; i <= j; ++i)
        A[n - 1 - i] = A[i];
}

return A;


// Fenwick Tree
int getParent(int i) {
    return i - (i & -i);
}

int getNext(int i) {
    return i + (i & -i);
}

void update(vector<int> &bit, int val, int index) {
    while (index < bit.size()) {
        bit[index] += val;
        index = getNext(index);
    }
}

void createTree(vector<int> &bit, vector<int> &a) {
    int i, j, n = a.size();

    for (i = 0; i < n; ++i)
        update(bit, a[i], i + 1);
}

void rangeUpdate(vector<int> &bit2, vector<int> &bit3, int l, int r, int x) {
    update(bit2, x, l);
    update(bit2, -x, r + 1);
    update(bit3, x * (l - 1), l);
    update(bit3, -x * r, r + 1);
}

int getSum(vector<int> &bit, int i) {
    int sum = 0;

    while (i > 0) {
        sum += bit[i];
        i = getParent(i);
    }

    return sum;
}

// ALL PERMUTATIONS OF STRING
void find(int f, vector<string> &ans, string &t, int n, string &s) {
    if (f == 0) {
        ans.push_back(t);
        return;
    }

    for (int i = 0; i < n; ++i) {
        if ((1 << i)&f) {
            t.push_back(s[i]);
            find((1 << i)^f, ans, t, n, s);
            t.pop_back();
        }
    }
}

// KALA QUES 1-4 AI

vector<string> search(
    vector<vector<int>> &adj,
    vector<vector<vector<int>>> &dist,
    vector<pair<int, int>> &wtf,
    int m,
    int n,
    int mw
) {
    vector<string> p, p2, ans;
    map<int, int> wfmp, wfmp2;
    int v, u, w, d, w2, d2, md = 1e9;
    int mask, mask2;
    // distance, student_mask, node, total_weight, weight_faculty_map, path
    set<tuple<int, int, int, int, map<int, int>, vector<string>>> st;
    dist[0][0][0] = 0;
    st.insert({0, 0, 0, 0, wfmp, {"0"}});

    while (!st.empty()) {
        auto it = st.begin();
        tie(d, mask, v, w, wfmp, p) = *it;
        st.erase(it);

        for (u = 0; u < m; ++u) {
            if (u == v)
                continue;

            d2 = d + adj[v][u];
            wfmp2 = wfmp;
            p2 = p;
            p2.push_back(to_string(u));

            if (wfmp.find(u) == wfmp.end())
                continue;

            w2 = w - wfmp2[u];
            wfmp2.erase(u);

            if (dist[u][mask][w2] >= d2) {
                if (w2 == 0 && mask == ((1 << n) - 1)) {
                    if (d2 < md) {
                        ans = p2;
                        md = d2;
                    }
                    else if (md == d2)
                        ans = min(ans, p2);
                }
                else
                    st.insert({d2, mask, u, w2, wfmp2, p2});
                dist[u][mask][w2] = d2;
            }
        }

        for (u = m; u < m + n; ++u) {
            w2 = w + wtf[u - m].first;
            d2 = d + adj[v][u];
            mask2 = mask | (1 << (u - m));
            wfmp2 = wfmp;
            wfmp2[wtf[u - m].second] += wtf[u - m].first;
            p2 = p;
            p2.push_back(to_string(u));

            if (mask == mask2 || u == v || w2 > mw || dist[u][mask2][w2] < d2)
                continue;

            dist[u][mask2][w2] = d2;
            st.insert({d2, mask2, u, w2, wfmp2, p2});
        }
    }

    return ans;
}

void solve() {
    vector<string> path;
    int i, j, m, n, mw, mask;

    if (QUESTION_NUMBER > 2)
        cin >> m >> n;
    else {
        cin >> n;
        m = 1;
        --n;
    }

    vector<pair<int, int>> wtf(n, {0, 0});
    vector<vector<int>> adj(m + n, vector<int>(m + n));

    for (i = 0; i < m + n; ++i)
        for (j = 0; j < m + n; ++j)
            cin >> adj[i][j];

    if (QUESTION_NUMBER > 2)
        for (i = 0; i < n; ++i)
            cin >> wtf[i].first >> wtf[i].second;
    else
        for (i = 0; i < n; ++i)
            cin >> wtf[i].first;

    cin >> mw;
    vector<vector<vector<int>>> dist(m + n, vector<vector<int>>(1 << n, vector<int>(mw + 1, 1e9)));
    path = search(adj, dist, wtf, m, n, mw);

    if (QUESTION_NUMBER & 1) {
        j = 0;

        for (i = 1; i < m; ++i)
            if (dist[j][(1 << n) - 1][0] > dist[i][(1 << n) - 1][0])
                j = i;

        cout << dist[j][(1 << n) - 1][0];
    }
    else
        for (i = 0; i < path.size(); ++i)
            cout << path[i] << " ";

    cout << endl;
}