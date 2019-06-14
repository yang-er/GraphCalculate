#pragma once
/**
求实对称矩阵的特征值及特征向量的雅克比法
利用雅格比(Jacobi)方法求实对称矩阵的全部特征值及特征向量
@param pMatrix 长度为n*n的数组，存放实对称矩阵
@param nDim 矩阵的阶数
@param pdblVects 长度为n*n的数组，返回特征向量(按列存储)
@param dbEps 精度要求
@param nJt 整型变量，控制最大迭代次数
@param pdbEigenValues 特征值数组
*/
extern bool JacbiCor(double* pMatrix, int nDim, double* pdblVects, double* pdbEigenValues, double dbEps, int nJt);
