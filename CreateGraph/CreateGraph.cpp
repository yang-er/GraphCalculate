﻿// CreateGraph.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>
using namespace std;
const int MAXN = 1e6+6;
vector<int> G[MAXN];
vector<int> ID;
int number_tot, N;
mt19937 Rand;

bool random_walk(int depth, int deg)
{
	if (depth == 0 && number_tot < N)
		return true;
	int rands = Rand() % (100 + deg * 10 + depth);
	return rands < 50;
}

void dfs(int current, int depth)
{
	while (number_tot < N)
	{
		if (random_walk(depth, G[current].size()))
		{
			G[current].push_back(++number_tot);
			dfs(number_tot, depth + 1);
		}
		else
		{
			return;
		}
	}
}

int main()
{
	Rand = mt19937(time(NULL));
	cin >> N;
	ID.assign(N + 1, 0);
	for (int i = 1; i <= N; i++)
		ID[i] = i;
	int QAQ = Rand() % ((Rand() + 100) % (N * 10));
	for (int i = 0; i < QAQ; i++)
		random_shuffle(ID.begin()+1, ID.end());
	number_tot = 1;
	dfs(1, 0);
	int M = pow(N, 0.75);

	for (int i = 0; i < M; i++)
	{
		int u = Rand() % N + 1;
		int v = u;
		while (v == u)
			v = Rand() % N + 1;
		G[u].push_back(v);
	}

	for (int u = 1; u <= N; u++)
		for (auto v : G[u])
			cout << ID[u] << " " << ID[v] << endl;
	return 0;
}
