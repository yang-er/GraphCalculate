#include "Jacobi.h"
#include <cmath>
#include <map>
#include <omp.h>
#include <algorithm>
using namespace std;

bool JacbiCor(double* pMatrix, int nDim, double* pdblVect, double* pdbEigenValue, double dbEps, int nJt)
{
	double* pdblVects = new double[1ll * nDim * nDim];
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

		#pragma omp parallel
		{
			double _dbMax = pMatrix[1];
			int _nRow = 0;
			int _nCol = -1;

			#pragma omp for
			for (int i = 0; i < nDim; i++)			//行
			{
				for (int j = 0; j < nDim; j++)		//列
				{
					if (i == j) continue;

					double d = fabs(pMatrix[i * nDim + j]);

					if (_nCol == -1 || d > _dbMax)
					{
						_dbMax = d;
						_nRow = i;
						_nCol = j;
					}
				}
			}

			#pragma omp critical
			{
				if (_dbMax > dbMax)
				{
					dbMax = _dbMax;
					nRow = _nRow;
					nCol = _nCol;
				}
				else if (_dbMax == dbMax && _nRow < nRow)
				{
					nRow = _nRow;
					nCol = _nCol;
				}
			}
		}

		if (dbMax < dbEps)
		{
			printf("epsilon achieved.\n");
			printf("ncount = %d\n", nCount);
			//精度符合要求 
			break;
		}

		if (nCount == nJt)
		{
			printf("ncount achieved.\n");
			//迭代次数超过限制
			//break;
		}

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

		#pragma omp parallel sections
		{
			#pragma omp section
			for (int i = 0; i < nDim; i++)
			{
				if ((i != nCol) && (i != nRow))
				{
					int u = i * nDim + nRow;	//p  
					int w = i * nDim + nCol;	//q
					double dbMax = pMatrix[u];
					double dbMin = pMatrix[w];
					pMatrix[u] = dbMin * dbSinTheta + dbMax * dbCosTheta;
					pMatrix[w] = dbMin * dbCosTheta - dbMax * dbSinTheta;
				}
			}

			#pragma omp section
			for (int j = 0; j < nDim; j++)
			{
				if ((j != nCol) && (j != nRow))
				{
					int u = nRow * nDim + j;	//p
					int w = nCol * nDim + j;	//q
					double dbMax = pMatrix[u];
					double dbMin = pMatrix[w];
					pMatrix[u] = dbMin * dbSinTheta + dbMax * dbCosTheta;
					pMatrix[w] = dbMin * dbCosTheta - dbMax * dbSinTheta;
				}
			}

			//计算特征向量
			#pragma omp section
			for (int i = 0; i < nDim; i++)
			{
				int u = i * nDim + nRow;		//p   
				int w = i * nDim + nCol;		//q
				double dbMax = pdblVects[u];
				double dbMin = pdblVects[w];
				pdblVects[u] = dbMin * dbSinTheta + dbMax * dbCosTheta;
				pdblVects[w] = dbMin * dbCosTheta - dbMax * dbSinTheta;
			}
		}
	}

	int max_loc = 0; double max_ans = pMatrix[0], tot = 0;
	for (int i = 1; i < nDim; i++)
		if (pMatrix[i * nDim + i] > max_ans)
			max_ans = pMatrix[i * nDim + i], max_loc = i;
	*pdbEigenValue = max_ans;
	for (int i = 0; i < nDim; i++)
	{
		pdblVect[i] = pdblVects[i * nDim + max_loc];
		tot += pdblVect[i];
	}

	if (tot < 0) for (int i = 0; i < nDim; i++) pdblVect[i] *= -1;
	delete[] pdblVects;
	return 1;
}