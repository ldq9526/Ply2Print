#include<math.h>

void add(double p[3], double u[3], double v[3]);//����������p = u + v
void vec(double p[3], double u[3], double v[3]);//����p = u -v
void times(double p[3], double k, double u[3]);//����p = k.u
double length(double u[3]);//���������ĳ���|u|
double dist(double u[3], double v[3]);//����u,v����֮��ľ���
double dot(double u[3], double v[3]);//��������������������u.v
void cross(double p[3], double u[3], double v[3]);//������p = u x v
double area(double u1[3], double u2[3], double u3[3]);//�������������
bool inTriangle(double P[3], double A[3], double B[3], double C[3]);//�ж�P�ڲ���������ABC�ڲ�
