#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include "Jacobi.h"

inline int loc(int n, int i, int j)
{
	return i * n + j;
}

int main()
{
	//freopen("1e4.in", "r", stdin);
	int m, n, u, v;
	scanf("%d %d", &n, &m);
	double* src = new double[n * n];
	double* tzxl = new double[n];
	double* tzz = new double;
	memset(src, 0, sizeof(double) * n * n);

	while (m--)
	{
		scanf("%d %d", &u, &v);
		src[loc(n, u - 1, v - 1)] += 1.0;
		src[loc(n, v - 1, u - 1)] += 1.0;
	}

	JacbiCor(src, n, tzxl, tzz, 5e-2, 10000);
	printf("c = %lf\nv = [ ", tzz[0]);
	for (int i = 0; i < n; i++)
		printf("%.2lf ", tzxl[i]);
	printf("]\n");
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
