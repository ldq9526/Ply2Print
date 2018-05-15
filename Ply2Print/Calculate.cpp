#include "stdafx.h"
#include "Calculate.h"

//����������p = u + v
void add(double p[3], double u[3], double v[3])
{
	p[0] = u[0] + v[0];
	p[1] = u[1] + v[1];
	p[2] = u[2] + v[2];
}

void vec(double p[3], double u[3], double v[3])//�������������Ĳ�p = u - v
{
	p[0] = u[0] - v[0];
	p[1] = u[1] - v[1];
	p[2] = u[2] - v[2];
}

void times(double p[3], double k, double u[3])//������������p = ku
{
	p[0] = k * u[0];
	p[1] = k * u[1];
	p[2] = k * u[2];
}

double length(double p[3])//���������ĳ���
{
	return sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
}

double dist(double u[3], double v[3])//��������֮��ľ���
{
	double p[3];
	vec(p, u, v);
	return length(p);
}

double dot(double u[3], double v[3])//��������������������u.v
{
	return (u[0] * v[0] + u[1] * v[1] + u[2] * v[2]);
}

void cross(double p[3], double u[3], double v[3])//������p = u x v
{
	p[0] = u[1] * v[2] - u[2] * v[1];
	p[1] = u[2] * v[0] - u[0] * v[2];
	p[2] = u[0] * v[1] - u[1] * v[0];
}

double area(double u1[3], double u2[3], double u3[3])//�������������
{
	double p1[3], p2[3];
	double s[3];
	vec(p1, u1, u2);//�������
	vec(p2, u1, u3);//�������
	cross(s, p1, p2);
	return (0.5 * length(s));
}

bool inTriangle(double P[3], double A[3], double B[3], double C[3])//�ж�P�ڲ���������ABC�ڲ�
{
	double AP[3], AB[3], AC[3];//����AP,AB,AC
	vec(AP, P, A);
	vec(AB, B, A);
	vec(AC, C, A);
	double x1 = dot(AB, AB);
	double x2 = dot(AC, AB);
	double x3 = dot(AP, AB);
	double x4 = dot(AB, AC);
	double x5 = dot(AC, AC);
	double x6 = dot(AP, AC);
	double u = (x3*x5 - x2*x6) / (x1*x5 - x2*x4);
	double v = (x1*x6 - x3*x4) / (x1*x5 - x2*x4);
	return (u >= 0) && (v >= 0) && (u + v <= 1);
}
