#define _CRT_SECURE_NO_WARNINGS
#pragma comment(linker,"/STACK:10240000,20480")
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>
using namespace std;
const int MAXN = 1e6 + 6;
vector<int> G[MAXN];
int N, M;

int duCenter()
{
	int* du = new int[N + 1];
	for (int i = 0; i < N + 1; i++)
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
	int max = 0;
	for (int i = 1; i <= N; i++)
	{
		cout << du[i] << " ";
		if (du[i] > du[max])
			max = i;
	}
	cout << "MAX:" << du[max];
	return max;
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

	cout << "MAX_DU:" << duCenter() << endl;
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
