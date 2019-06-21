#define _CRT_SECURE_NO_WARNINGS
#pragma comment(linker,"/STACK:10240000,20480")
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>
using namespace std;
const int MAXN = 1e6+6;
vector<int> G[MAXN];
int N, M;
int* du;
int MAX_POINT;
void duCenter()
{
	du = new int[MAXN];
	for (int i = 0; i < MAXN; i++)
	{
		du[i] = 0;
	}
	for (int u = 1; u <= N; u++)
	{
		for (auto v : G[u])
		{
			du[v]++;
			du[u]++;
		}
	}
	//sort(du, du + N, [](int a, int b) { return a > b; });
	MAX_POINT = 0;
	for (int i = 1; i <= N; i++)
	{
		cout << du[i] << " ";
		if (du[i] > du[MAX_POINT])
			MAX_POINT = i;
	}
}
void addPoint()
{
	cout << "请输入要加入的点";
	int point;
	cin >> point;

}
void addEdge()
{
	cout << "请输入要加入的边 ?->?"<<endl;
	int p1,p2;
	cin >> p1 >> p2;
	if (p1 == p2)
	{
		cout << "不能存在自环";
		addEdge();
	}
	else
	{
		bool  f = false;
		G[p1].push_back(p2);
		if (++(du[p1]) > du[MAX_POINT])
		{
			f = true;
			MAX_POINT = p1;
		}
		if (++(du[p2]) > du[MAX_POINT])
		{
			f = true;
			MAX_POINT =  p2;
		}
		if (f)
		{
			cout<<"度中心性已经改变，MAX_DU:" << du[MAX_POINT] << "POINT:"<< MAX_POINT << endl;
		}
		else
		{
			cout << "度中心性未改变，MAX_DU:" << du[MAX_POINT] << "POINT:" << MAX_POINT << endl;
		}
	}
}
int main()
{
	scanf("%d %d", &N, &M);
	int u, v;

	while (M--)
	{
		scanf("%d %d", &u, &v);
		G[u].push_back(v);
	}
	duCenter();
	cout << "MAX_DU:" << du[MAX_POINT] << "POINT:" << MAX_POINT << endl;
	int chose;
	cout << "1-加边";
	while (cin >> chose)
	{
		if (chose) addEdge();
		else break;
		cout << "1-加边";
	}
	return 0;
}