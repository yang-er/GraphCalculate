#pragma once
/**
��ʵ�Գƾ��������ֵ�������������ſ˱ȷ�
�����Ÿ��(Jacobi)������ʵ�Գƾ����ȫ������ֵ����������
@param pMatrix ����Ϊn*n�����飬���ʵ�Գƾ���
@param nDim ����Ľ���
@param pdblVects ����Ϊn*n�����飬������������(���д洢)
@param dbEps ����Ҫ��
@param nJt ���ͱ�������������������
@param pdbEigenValues ����ֵ����
*/
extern bool JacbiCor(double* pMatrix, int nDim, double* pdblVects, double* pdbEigenValues, double dbEps, int nJt);
