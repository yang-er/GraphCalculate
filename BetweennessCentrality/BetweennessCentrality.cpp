#define _CRT_SECURE_NO_WARNINGS
using namespace std;
#include <cstdio>
#include <cstring>
#include <queue>
#include <iostream>
#include <stack>
int n, m, u, v;
#define _(u,v) (u-1)*n+v-1

struct tuple3 { int u, dis, uu; tuple3(int a, int b, int c) { u = a, dis = b, uu = c; } };

void betweenness(int* graph)
{
	int INF = 0x3f3f3f3f;
	queue<int> bfs;//用来存储bfs序
	stack<int> sfb;//用来存储反bfs序
	int* MinWayNum = new int[n * n];
	int* Shortest = new int[n * n];
	int* vis = new int[n];
	double* Betweenness = new double[n];
	double* PaFb = new double[n * n];//Pass a From b 记录从a出发的最短路经过b的betweenness
	int* PreNode = new int[n * n];//记录各个节点是其他哪个节点的前驱
	memset(vis, 0, n * sizeof(int));
	memset(Shortest, 0x3f3f3f3f, n * n * sizeof(int));
	memset(PaFb, 0, n * n * sizeof(double));
	memset(PreNode, 0, n * n * sizeof(int));
	memset(Betweenness, 0, n * sizeof(double));
	memset(MinWayNum, 0, n * n * sizeof(int));

	for (int i = 1; i <= n; i++)
	{
		queue<tuple3> Q;
		Q.emplace(i, 0, -1);

		while (!Q.empty())
		{
			tuple3 e = Q.front();
			Q.pop();
			if (Shortest[_(i, e.u)] < e.dis) continue;
			Shortest[_(i, e.u)] = e.dis;
			MinWayNum[_(i, e.u)]++;

			for (int v = 1; v <= n; v++)
			{
				for (int k = 0; k < graph[_(e.u, v)]; k++)
				{
					Q.emplace(v, e.dis + 1, e.u);
				}
			}
		}
	}
	for (int i = 1; i <= n; i++) {//计算从a出发经过b的介数中心性
		queue<pair<int, int>> Q;
		Q.emplace(i, 0);
		bfs.push(i);
		sfb.push(i);
		vis[i-1] = 1;
		while (!Q.empty())
		{
			int u = Q.front().first;
			int dis = Q.front().second;
			Q.pop();
			if (Shortest[_(i, u)] < dis) continue;

			for (int v = 1; v <= n; v++)
			{
				for (int k = 0; k < graph[_(u, v)]; k++)
				{
					Q.emplace(v, dis + 1);
					PreNode[_(u, v)] = 1;
				}
				if (graph[_(u, v)] > 0 && !vis[v - 1]) {
					sfb.push(v);
					vis[v - 1] = 1;
				}
			}
		}
		memset(vis, 0, n * sizeof(int));
		//cout << "栈的大小为" << sfb.size() << endl;
		while (!sfb.empty())
		{
			queue<int> pre;//记录当前节点的前驱节点
			bool flag = false;//用来标记当前节点是否为其他节点的前驱
			int u = sfb.top();
			sfb.pop();
			for (int v = 1; v <= n; v++) {
				if (PreNode[_(u,v)] && v!=i) {
					flag = true;
					pre.push(v);
				}
			}
			if (flag) {
				while (!pre.empty()) {
					int j = pre.front();
					pre.pop();
					PaFb[_(i,u)] +=
						((double)MinWayNum[_(i, u)] / MinWayNum[_(i,j)]) *
						(1 + PaFb[_(i,j)])*graph[_(u,j)];
				}
			}
		}
	}
	for (int i = 0; i < n; i++) {
		PaFb[i * n + i] = 0;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			Betweenness[i] += PaFb[j * n + i];
		}
	}
	double max = 0;
	int node = 0;
	for (int i = 0; i < n; i++) {
		cout << i + 1;
		cout << "  ";
		cout << Betweenness[i]<< endl;
		if (Betweenness[i] > max) {
			max = Betweenness[i];
			node = i + 1;
		}
	}
	cout << "节数中心性最大的是节点" << node << "最大的阶数中心性为" << max << endl;
}

int main()
{
	scanf("%d %d", &n, &m);
	int* graph = new int[n * n];
	memset(graph, 0, n * n *sizeof(int));
	while (m--)
	{
		scanf("%d %d", &u, &v);
		graph[_(u, v)] += 1;
	}

	betweenness(graph);
	return 0;
}
