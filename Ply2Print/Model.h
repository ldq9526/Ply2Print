#pragma once

#include<GL/glut.h>
#include<set>
#include"Calculate.h"//向量及矩阵的运算函数
using namespace std;

typedef GLdouble Vertex[3];//GLdouble[3]类型重命名为顶点类型
typedef int Facet[3];//将int[3]重命名为面片
typedef double Normal[3];//将double[3]重命名为法向量

class Model
{
private:
	int vertexN;//顶点数
	int facetN;//面片数
	Vertex * vertex;//顶点的集合
	Facet * facet;//面片的集合
	Normal * vertexNormal;//点法向量
	Normal * facetNormal;//面法向量
	bool * isBound;//标记每个点是否为边界点的数组
	set < int > * vertexDegree;//记录与每个点相邻接的点索引
	set < int > * facetDegree;//记录与每个点相邻接的面索引

	void computeFaceNormal(void);//计算面法向量
	void computeVertexNormal(void);//计算点法向量
public:
	Model();
	void CopyModel(Model * &);//将模型拷贝
	void ComputeNormal(void);//计算法向量
	void ClearModel(void);//清除模型的一切信息
	void ComputeDegree(void);//计算邻接点与邻接边
	bool ReadFile(CString);//从文件中读取模型的信息
	int GetVertexN(void);//获取顶点数
	int GetFacetN(void);//获取面片数
	Vertex * GetVertex(void);//获取顶点数组
	Facet * GetFacet(void);//获取面片数组
	Normal * GetVertexNormal(void);//获取点法向量的数组
	Normal * GetFacetNormal(void);//获取面法向量数组
	GLdouble * GetVertex(int index);//返回某个点
	double GetVolume(void);//获取模型体积
	bool * GetBound(void);//获取边界数组
	set<int> * GetVertexDegree(void);//获取顶点的邻接表
	set<int> * GetFacetDegree(void);//获取面片的邻接表
	void Scale(double);//成比例缩放
	virtual ~Model();
};

