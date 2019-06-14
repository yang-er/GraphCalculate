#include "Jacobi.h"
#include <cmath>
#include <map>
#include <algorithm>
using namespace std;

bool JacbiCor(double* pMatrix, int nDim, double* pdblVects, double* pdbEigenValues, double dbEps, int nJt)
{
	for (int i = 0; i < nDim; i++)
		for (int j = 0; j < nDim; j++)
			pdblVects[i * nDim + j] = i == j ? 1.0 : 0.0;

	int nCount = 0; //迭代次数
	while (1)
	{
		//在pMatrix的非对角线上找到最大元素
		double dbMax = pMatrix[1];
		int nRow = 0;
		int nCol = 1;
		for (int i = 0; i < nDim; i++)			//行
		{
			for (int j = 0; j < nDim; j++)		//列
			{
				double d = fabs(pMatrix[i * nDim + j]);

				if ((i != j) && (d > dbMax))
				{
					dbMax = d;
					nRow = i;
					nCol = j;
				}
			}
		}

		if (dbMax < dbEps)     //精度符合要求 
			break;

		if (nCount > nJt)       //迭代次数超过限制
			break;

		nCount++;

		double dbApp = pMatrix[nRow * nDim + nRow];
		double dbApq = pMatrix[nRow * nDim + nCol];
		double dbAqq = pMatrix[nCol * nDim + nCol];

		//计算旋转角度
		double dbAngle = 0.5 * atan2(-2 * dbApq, dbAqq - dbApp);
		double dbSinTheta = sin(dbAngle);
		double dbCosTheta = cos(dbAngle);
		double dbSin2Theta = sin(2 * dbAngle);
		double dbCos2Theta = cos(2 * dbAngle);

		pMatrix[nRow * nDim + nRow] = dbApp * dbCosTheta * dbCosTheta +
			dbAqq * dbSinTheta * dbSinTheta + 2 * dbApq * dbCosTheta * dbSinTheta;
		pMatrix[nCol * nDim + nCol] = dbApp * dbSinTheta * dbSinTheta +
			dbAqq * dbCosTheta * dbCosTheta - 2 * dbApq * dbCosTheta * dbSinTheta;
		pMatrix[nRow * nDim + nCol] = 0.5 * (dbAqq - dbApp) * dbSin2Theta + dbApq * dbCos2Theta;
		pMatrix[nCol * nDim + nRow] = pMatrix[nRow * nDim + nCol];

		for (int i = 0; i < nDim; i++)
		{
			if ((i != nCol) && (i != nRow))
			{
				int u = i * nDim + nRow;	//p  
				int w = i * nDim + nCol;	//q
				dbMax = pMatrix[u];
				pMatrix[u] = pMatrix[w] * dbSinTheta + dbMax * dbCosTheta;
				pMatrix[w] = pMatrix[w] * dbCosTheta - dbMax * dbSinTheta;
			}
		}

		for (int j = 0; j < nDim; j++)
		{
			if ((j != nCol) && (j != nRow))
			{
				int u = nRow * nDim + j;	//p
				int w = nCol * nDim + j;	//q
				dbMax = pMatrix[u];
				pMatrix[u] = pMatrix[w] * dbSinTheta + dbMax * dbCosTheta;
				pMatrix[w] = pMatrix[w] * dbCosTheta - dbMax * dbSinTheta;
			}
		}

		//计算特征向量
		for (int i = 0; i < nDim; i++)
		{
			int u = i * nDim + nRow;		//p   
			int w = i * nDim + nCol;		//q
			dbMax = pdblVects[u];
			pdblVects[u] = pdblVects[w] * dbSinTheta + dbMax * dbCosTheta;
			pdblVects[w] = pdblVects[w] * dbCosTheta - dbMax * dbSinTheta;
		}

	}

	//对特征值进行排序以及重新排列特征向量,特征值即pMatrix主对角线上的元素
	map<double, int> mapEigen;
	for (int i = 0; i < nDim; i++)
	{
		pdbEigenValues[i] = pMatrix[i * nDim + i];
		mapEigen[pdbEigenValues[i]] = i;
	}

	double* pdbTmpVec = new double[nDim * nDim];
	auto iter = mapEigen.rbegin();
	for (int j = 0; iter != mapEigen.rend(), j < nDim; ++iter, ++j)
	{
		for (int i = 0; i < nDim; i++)
		{
			pdbTmpVec[i * nDim + j] = pdblVects[i * nDim + iter->second];
		}

		//特征值重新排列
		pdbEigenValues[j] = iter->first;
	}

	//设定正负号
	for (int i = 0; i < nDim; i++)
	{
		double dSumVec = 0;
		for (int j = 0; j < nDim; j++)
			dSumVec += pdbTmpVec[j * nDim + i];
		if (dSumVec< 0)
		{
			for (int j = 0; j < nDim; j++)
				pdbTmpVec[j * nDim + i] *= -1;
		}
	}

	memcpy(pdblVects, pdbTmpVec, sizeof(double) * nDim * nDim);
	delete[] pdbTmpVec;
	return 1;
}