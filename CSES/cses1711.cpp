#include <bits/stdc++.h>

using namespace std;
using ll = long long;

#ifdef _DEBUG
#include "utils/debug.h"
#else
#define debug(...)
#endif

// Source: atcoder library
// https://atcoder.github.io/ac-library/production/document_en/maxflow.html
namespace atcoder {

namespace internal {

template <class T>
struct simple_queue {
	std::vector<T> payload;
	int pos = 0;
	void reserve(int n) { payload.reserve(n); }
	int size() const { return int(payload.size()) - pos; }
	bool empty() const { return pos == int(payload.size()); }
	void push(const T& t) { payload.push_back(t); }
	T& front() { return payload[pos]; }
	void clear() {
		payload.clear();
		pos = 0;
	}
	void pop() { pos++; }
};

}	 // namespace internal

template <class Cap>
struct mf_graph {
 public:
	mf_graph() : _n(0) {}
	explicit mf_graph(int n) : _n(n), g(n) {}

	int add_edge(int from, int to, Cap cap) {
		assert(0 <= from && from < _n);
		assert(0 <= to && to < _n);
		assert(0 <= cap);
		int m = int(pos.size());
		pos.push_back({from, int(g[from].size())});
		int from_id = int(g[from].size());
		int to_id = int(g[to].size());
		if (from == to) to_id++;
		g[from].push_back(_edge{to, to_id, cap});
		g[to].push_back(_edge{from, from_id, 0});
		return m;
	}

	struct edge {
		int from, to;
		Cap cap, flow;
	};

	edge get_edge(int i) {
		int m = int(pos.size());
		assert(0 <= i && i < m);
		auto _e = g[pos[i].first][pos[i].second];
		auto _re = g[_e.to][_e.rev];
		return edge{pos[i].first, _e.to, _e.cap + _re.cap, _re.cap};
	}
	std::vector<edge> edges() {
		int m = int(pos.size());
		std::vector<edge> result;
		for (int i = 0; i < m; i++) {
			result.push_back(get_edge(i));
		}
		return result;
	}
	void change_edge(int i, Cap new_cap, Cap new_flow) {
		int m = int(pos.size());
		assert(0 <= i && i < m);
		assert(0 <= new_flow && new_flow <= new_cap);
		auto& _e = g[pos[i].first][pos[i].second];
		auto& _re = g[_e.to][_e.rev];
		_e.cap = new_cap - new_flow;
		_re.cap = new_flow;
	}

	Cap flow(int s, int t) { return flow(s, t, std::numeric_limits<Cap>::max()); }
	Cap flow(int s, int t, Cap flow_limit) {
		assert(0 <= s && s < _n);
		assert(0 <= t && t < _n);
		assert(s != t);

		std::vector<int> level(_n), iter(_n);
		internal::simple_queue<int> que;

		auto bfs = [&]() {
			std::fill(level.begin(), level.end(), -1);
			level[s] = 0;
			que.clear();
			que.push(s);
			while (!que.empty()) {
				int v = que.front();
				que.pop();
				for (auto e : g[v]) {
					if (e.cap == 0 || level[e.to] >= 0) continue;
					level[e.to] = level[v] + 1;
					if (e.to == t) return;
					que.push(e.to);
				}
			}
		};
		auto dfs = [&](auto self, int v, Cap up) {
			if (v == s) return up;
			Cap res = 0;
			int level_v = level[v];
			for (int& i = iter[v]; i < int(g[v].size()); i++) {
				_edge& e = g[v][i];
				if (level_v <= level[e.to] || g[e.to][e.rev].cap == 0) continue;
				Cap d = self(self, e.to, std::min(up - res, g[e.to][e.rev].cap));
				if (d <= 0) continue;
				g[v][i].cap += d;
				g[e.to][e.rev].cap -= d;
				res += d;
				if (res == up) return res;
			}
			level[v] = _n;
			return res;
		};

		Cap flow = 0;
		while (flow < flow_limit) {
			bfs();
			if (level[t] == -1) break;
			std::fill(iter.begin(), iter.end(), 0);
			Cap f = dfs(dfs, t, flow_limit - flow);
			if (!f) break;
			flow += f;
		}
		return flow;
	}

	std::vector<bool> min_cut(int s) {
		std::vector<bool> visited(_n);
		internal::simple_queue<int> que;
		que.push(s);
		while (!que.empty()) {
			int p = que.front();
			que.pop();
			visited[p] = true;
			for (auto e : g[p]) {
				if (e.cap && !visited[e.to]) {
					visited[e.to] = true;
					que.push(e.to);
				}
			}
		}
		return visited;
	}

 private:
	int _n;
	struct _edge {
		int to, rev;
		Cap cap;
	};
	std::vector<std::pair<int, int>> pos;
	std::vector<std::vector<_edge>> g;
};

}	 // namespace atcoder
using namespace atcoder;

/*
run max-flow and repeatedly run dfs to get paths back
*/

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int N, M; cin >> N >> M;

	mf_graph<int> g(N);
	struct Edge{ int to, id; };
	vector<vector<Edge>> adj(N);
	for (int i = 0; i < M; ++i) {
		int a, b; cin >> a >> b;
		a--, b--;
		adj[a].push_back({b, i});
		g.add_edge(a, b, 1);
	}

	int paths = g.flow(0, N-1);
	vector<int> flow(M);
	for (int i = 0; i < M; ++i) {
		flow[i] = g.get_edge(i).flow;
	}

	vector<int> path;
	auto dfs = [&](auto self, int v) -> void {
		path.push_back(v);
		if (v == N-1) return;
		for (auto [to, id] : adj[v]) {
			if (!flow[id]) continue;
			flow[id]--;
			self(self, to);
			return;
		}
	};
	cout << paths << '\n';
	for (int i = 0; i < paths; ++i) {
		path.clear();
		dfs(dfs, 0);
		cout << path.size() << '\n';
		for (int j = 0; j < path.size(); ++j) {
			cout << path[j]+1 << (j == path.size()-1 ? '\n' : ' ');
		}
	}
}
