#include<math.h>

void add(double p[3], double u[3], double v[3]);//计算向量和p = u + v
void vec(double p[3], double u[3], double v[3]);//计算p = u -v
void times(double p[3], double k, double u[3]);//计算p = k.u
double length(double u[3]);//计算向量的长度|u|
double dist(double u[3], double v[3]);//计算u,v两点之间的距离
double dot(double u[3], double v[3]);//计算两个向量的数量积u.v
void cross(double p[3], double u[3], double v[3]);//计算差积p = u x v
double area(double u1[3], double u2[3], double u3[3]);//计算三角形面积
bool inTriangle(double P[3], double A[3], double B[3], double C[3]);//判断P在不在三角形ABC内部
