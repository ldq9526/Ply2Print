#include "stdafx.h"
#include "Calculate.h"

//计算向量和p = u + v
void add(double p[3], double u[3], double v[3])
{
	p[0] = u[0] + v[0];
	p[1] = u[1] + v[1];
	p[2] = u[2] + v[2];
}

void vec(double p[3], double u[3], double v[3])//计算两个向量的差p = u - v
{
	p[0] = u[0] - v[0];
	p[1] = u[1] - v[1];
	p[2] = u[2] - v[2];
}

void times(double p[3], double k, double u[3])//计算数乘向量p = ku
{
	p[0] = k * u[0];
	p[1] = k * u[1];
	p[2] = k * u[2];
}

double length(double p[3])//计算向量的长度
{
	return sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
}

double dist(double u[3], double v[3])//计算两点之间的距离
{
	double p[3];
	vec(p, u, v);
	return length(p);
}

double dot(double u[3], double v[3])//计算两个向量的数量积u.v
{
	return (u[0] * v[0] + u[1] * v[1] + u[2] * v[2]);
}

void cross(double p[3], double u[3], double v[3])//计算差积p = u x v
{
	p[0] = u[1] * v[2] - u[2] * v[1];
	p[1] = u[2] * v[0] - u[0] * v[2];
	p[2] = u[0] * v[1] - u[1] * v[0];
}

double area(double u1[3], double u2[3], double u3[3])//计算三角形面积
{
	double p1[3], p2[3];
	double s[3];
	vec(p1, u1, u2);//求差向量
	vec(p2, u1, u3);//求差向量
	cross(s, p1, p2);
	return (0.5 * length(s));
}

bool inTriangle(double P[3], double A[3], double B[3], double C[3])//判断P在不在三角形ABC内部
{
	double AP[3], AB[3], AC[3];//向量AP,AB,AC
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
