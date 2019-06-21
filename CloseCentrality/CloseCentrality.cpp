#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <queue>
#include <fstream>
#define FOR(x,f,t) for(x=f;x<=t;++x)
#define RFOR(x,f,t) for(x=f;x>=t;--x)
#define oo 2147483647
#define M 20000
#define MEMSET(a,b) memset(a,b,sizeof(a));
/*5
7
1 2
1 4
1 5
2 3
3 5
4 3
4 5 */

/*10
14
6 2
6 2
2 1
2 5
2 10
1 5
5 10
5 1
10 7
10 3
10 8
10 9
8 4
9 4*/
typedef long long ll;
using namespace std;
struct DD {
	int id, dist;
	DD(int i, int d) :id(i), dist(d) {}
	bool operator < (const DD& dd) const {
		//按每点D值排序，用于下面的优先队列
		return dist > dd.dist;
	}
};
struct Edge {
	int f, t, len;
	Edge(int a, int b, int c) :f(a), t(b), len(c) {}
};
struct Dijkstra {
	int size;
	bool taken[M];
	int d[M];
	vector<Edge> edges;
	void init(int size) {
		this->size = size;
		edges.clear();
	}
	void addEdge(int f, int t, int len) {
		edges.push_back(Edge(f, t, len));
	}
	void dijkstra(int s) {
		MEMSET(taken, 0); //清空标记
		MEMSET(d, 127); //将每点d值填充到无穷大
		d[s] = 0; //起始点d值为0
		priority_queue<DD> Q;
		Q.push(DD(s, 0));
		int i;
		while (!Q.empty()) {
			DD mi = Q.top(); //取出剩余点中d值最小的点
			Q.pop();
			int x = mi.id;
			taken[x] = true;
			FOR(i, 0, edges.size() - 1) {
				Edge& e = edges[i];
				if (taken[e.t]) continue;
				if (e.f != x) continue;
				d[e.t] = min(d[e.t], d[x] + e.len);
				Q.push(DD(e.t, d[e.t]));
			}
		}

	}
};

void CloseCentrality(Dijkstra d) {
	int i, j, id;
	float max, sum;
	max = 0;
	id = 0;
	vector<float> average;
	//计算每个点的平均最短路；
	for (i = 1;i <= d.size;i++) {
		sum = 0;
		d.dijkstra(i); //求从第1个点开始到所有点的距离
		FOR(j, 1, d.size) sum += d.d[j];
		average.push_back((d.size - 1) / sum);

	}
	for (i = 0;i <= d.size - 1;i++) {
		if (average[i] >= max) {

			id = i;
			max = average[i];
		}
	}
	cout << "紧密中心点为:" << id + 1 << endl;
	cout << "紧密中心值为" << max << endl;




}

int main() {
	Dijkstra d;
	int i, n, m;
	cin >> n >> m; //图一共有n个点，m条边
	d.init(n); //初始化
	FOR(i, 1, m) {
		int a, b;
		cin >> a >> b;
		d.addEdge(a, b, 1); //添加边
	}
	CloseCentrality(d);
	return 0;
}
