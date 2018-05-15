#include "stdafx.h"
#include "Model.h"


Model::Model()
{
	vertexN = 0;//顶点数默认为0
	facetN = 0;//面片数默认为0
	vertex = nullptr;//顶点集合为空
	facet = nullptr;//面片集合为空
	vertexNormal = nullptr;//点法向量为空
	facetNormal = nullptr;//面法向量为空
	isBound = nullptr;//边界点判断为空
	vertexDegree = nullptr;//点邻接表为空
	facetDegree = nullptr;//面邻接表为空
}


Model::~Model()
{
	if (nullptr != vertex)
		delete [] vertex;
	if (nullptr != facet)
		delete [] facet;
	if (nullptr != facetNormal)
		delete[] facetNormal;
	if (nullptr != vertexNormal)
		delete[] vertexNormal;
	if (nullptr != isBound)
		delete[] isBound;
	if (vertexDegree != nullptr)
		delete[] vertexDegree;
	if (facetDegree != nullptr)
		delete[] facetDegree;
}

bool Model::ReadFile(CString filepath)//读取文件信息保存模型
{
	CString info, str;
	CStdioFile file;
	if (!file.Open(filepath, CFile::modeRead))//读取文件
		return false;
	else
	{
		file.ReadString(str);//读取一行到str
		swscanf_s(str, _T("%d"), &vertexN);//获取顶点数
		file.ReadString(str);//读取一行到str
		swscanf_s(str, _T("%d"), &facetN);//获取面片数

		vertex = new Vertex[vertexN];//为顶点数组分配内存空间
		isBound = new bool[vertexN];//为边界点标志分配内存空间
		vertexDegree = new set<int>[vertexN];//为点邻接表分配内存空间
		facetDegree = new set<int>[vertexN];//为面邻接表分配内存空间

		facet = new Facet[facetN];//为面片数组分配内存

		// 依次读入点的坐标
		for (int i = 0; i < vertexN; i++)
		{
			file.ReadString(str);
			swscanf_s(str, _T("%lf %lf %lf"), &vertex[i][0], &vertex[i][1], &vertex[i][2]);
		}
		//依次读入面片的信息
		for (int i = 0; i < facetN; i++)
		{
			int temp;
			file.ReadString(str);
			swscanf_s(str, _T("%d %d %d %d"), &temp, &facet[i][0], &facet[i][1], &facet[i][2]);
		}
		ComputeNormal();//计算法向量
		file.Close();//关闭文件
		return true;
	}
}

void Model::CopyModel(Model * & backupModel)//将模型备份一份到backupModel
{
	if (backupModel == nullptr)
		backupModel = new Model();//如果模型为空
	else
		backupModel->ClearModel();//先清空现有的备份模型

	backupModel->vertexN = vertexN;//复制顶点数
	backupModel->facetN = facetN;//复制面片数

	backupModel->vertex = new Vertex[vertexN];//构造顶点数组
	backupModel->vertexNormal = new Normal[vertexN];//构造点法向量数组
	backupModel->isBound = new bool[vertexN];//构造边界值数组
	backupModel->vertexDegree = new set<int>[vertexN];//构造顶点邻接表
	backupModel->facetDegree = new set<int>[vertexN];//构造面邻接表
	backupModel->facet = new Facet[facetN];//构造面片数组
	backupModel->facetNormal = new Normal[facetN];//构造面法向量数组
	
	for (int i = 0; i < vertexN; i++)//复制顶点信息
	{
		//复制顶点
		backupModel->vertex[i][0] = vertex[i][0];
		backupModel->vertex[i][1] = vertex[i][1];
		backupModel->vertex[i][2] = vertex[i][2];
		//复制顶点是否为边界信息
		backupModel->isBound[i] = isBound[i];
	}

	for (int i = 0; i < facetN; i++)//复制面片信息
	{
		//复制面片
		backupModel->facet[i][0] = facet[i][0];
		backupModel->facet[i][1] = facet[i][1];
		backupModel->facet[i][2] = facet[i][2];
	}
	backupModel->ComputeNormal();//重新计算法向量
}

Vertex * Model::GetVertex(void)//获取顶点数组
{
	return vertex;
}

Facet * Model::GetFacet(void)//获取面片数组
{
	return facet;
}

int Model::GetVertexN(void)//获取顶点数
{
	return vertexN;
}

int Model::GetFacetN(void)//获取面片数
{
	return facetN;
}

Normal * Model::GetVertexNormal(void)//获取点法向量的数组
{
	return vertexNormal;
}

Normal * Model::GetFacetNormal(void)//获取面法向量数组
{
	return facetNormal;
}

void Model::computeFaceNormal(void)//计算每个面片的法向量
{
	if (facetNormal != nullptr)
		delete[] facetNormal;
	facetNormal = new Normal[facetN];
	for (int i = 0; i < facetN; i++)
	{
		double u[3], v[3];
		vec(u, vertex[facet[i][0]], vertex[facet[i][1]]);//面片上一个向量
		vec(v, vertex[facet[i][0]], vertex[facet[i][2]]);//另一个不共面向量
		cross(facetNormal[i], u, v);//差积运算求得法向量
		double L = length(facetNormal[i]);//求法向量的长度
		if (fabs(L - 0.0) < 1e-5)//法向量长度为0
			times(facetNormal[i], 0.0, facetNormal[i]);//直接确定为0向量
		else
			times(facetNormal[i], 1.0/L, facetNormal[i]);//单位化
	}
}

void Model::ComputeNormal(void)//计算法向量
{
	computeFaceNormal();//先计算面法向
	computeVertexNormal();//再计算每个点处的法向量
}

/*先计算面片的法向量,再根据面片的法向量求出每个点的法向量*/
void Model::computeVertexNormal(void)//计算每个点处的法向量
{
	if (vertexNormal != nullptr)
		delete[] vertexNormal;
	vertexNormal = new Normal[vertexN];

	for (int i = 0; i < vertexN; i++)
		vertexNormal[i][0] = vertexNormal[i][1] = vertexNormal[i][2] = 0.0;

	for (int i = 0; i < facetN; i++)//每个点处的法向量
	{
		//面片第1个点做向量合成
		vertexNormal[facet[i][0]][0] += facetNormal[i][0];
		vertexNormal[facet[i][0]][1] += facetNormal[i][1];
		vertexNormal[facet[i][0]][2] += facetNormal[i][2];
		//面片第2个点做向量合成
		vertexNormal[facet[i][1]][0] += facetNormal[i][0];
		vertexNormal[facet[i][1]][1] += facetNormal[i][1];
		vertexNormal[facet[i][1]][2] += facetNormal[i][2];
		//面片第3个点做向量合成
		vertexNormal[facet[i][2]][0] += facetNormal[i][0];
		vertexNormal[facet[i][2]][1] += facetNormal[i][1];
		vertexNormal[facet[i][2]][2] += facetNormal[i][2];
	}
	//每个点处的法向量单位化
	for (int i = 0; i < vertexN; i++)
	{
		double L = length(vertexNormal[i]);//计算法向量长度
		if (fabs(L - 0.0) < 1e-5)
			times(vertexNormal[i], 0.0, vertexNormal[i]);//直接确定为0向量
		else
			times(vertexNormal[i], 1.0 / L, vertexNormal[i]);
	}
}

double Model::GetVolume(void)//获取模型体积:计算之前必须先更新法向量
{
	double vol = 0.0;
	double g[3];//记录某个面的中心
	double u[3], v[3];//u,v分别是某个面上不不共线的两个向量
	double n[3];//n是面片的法向量
	ComputeNormal();//重新计算法向量
	for (int i = 0; i < facetN; i++)
	{
		g[0] = (vertex[facet[i][0]][0] + vertex[facet[i][1]][0] + vertex[facet[i][2]][0]) / 3.0;
		g[1] = (vertex[facet[i][0]][1] + vertex[facet[i][1]][1] + vertex[facet[i][2]][1]) / 3.0;
		g[2] = (vertex[facet[i][0]][2] + vertex[facet[i][1]][2] + vertex[facet[i][2]][2]) / 3.0;

		vec(u, vertex[facet[i][1]], vertex[facet[i][0]]);//求u
		vec(v, vertex[facet[i][2]], vertex[facet[i][0]]);//求v
		cross(n, u, v);//求法向量n

		vol += (fabs(dot(g, n))) / 6.0;//混合积a.b.c/6
	}
	return vol;
}

void Model::ClearModel(void)//清除模型的一切信息
{
	if (vertex != nullptr)
	{
		delete[] vertex;
		vertex = nullptr;
	}
	if (facet != nullptr)
	{
		delete[] facet;
		facet = nullptr;
	}
	if (vertexNormal != nullptr)
	{
		delete[] vertexNormal;
		vertexNormal = nullptr;
	}
	if (facetNormal != nullptr)
	{
		delete[] facetNormal;
		facetNormal = nullptr;
	}
	if (isBound != nullptr)
	{
		delete[] isBound;
		isBound = nullptr;
	}
	if (vertexDegree != nullptr)
	{
		delete[] vertexDegree;
		vertexDegree = nullptr;
	}
	if (facetDegree != nullptr)
	{
		delete[] facetDegree;
		facetDegree = nullptr;
	}
}

bool * Model::GetBound(void)//获取边界数组
{
	return isBound;
}

set<int> * Model::GetVertexDegree(void)//获取顶点的邻接表
{
	return vertexDegree;
}

set<int> * Model::GetFacetDegree(void)//获取面片的邻接表
{
	return facetDegree;
}

void Model::Scale(double rate)//成比例缩放
{
	for (int i = 0; i < vertexN; i++)
	{
		vertex[i][0] *= rate;
		vertex[i][1] *= rate;
		vertex[i][2] *= rate;
	}
}

void Model::ComputeDegree(void)//计算邻接点与邻接边
{
	for (int i = 0; i < facetN; i++)
	{
		facetDegree[facet[i][0]].insert(i);//第1个顶点添加邻接面
		facetDegree[facet[i][1]].insert(i);//第2个顶点添加邻接面
		facetDegree[facet[i][2]].insert(i);//第3个顶点添加邻接面
		for (int j = 0; j < 3; j++)//根据面片记录每个点的邻接点
		{
			if (facet[i][j] != facet[i][(j + 1) % 3])
				vertexDegree[facet[i][j]].insert(facet[i][(j + 1) % 3]);
			if (facet[i][j] != facet[i][(j + 2) % 3])
				vertexDegree[facet[i][j]].insert(facet[i][(j + 2) % 3]);
		}
	}
	for (int i = 0; i < vertexN; i++)//判断每个点是否为边界点
	{
		int vN = vertexDegree[i].size();//与点i邻接的点的个数
		int fN = facetDegree[i].size();//与点i邻接的面的个数
		if (vN == fN)//邻接点与邻接面个数相同,不是边界点
			isBound[i] = false;
		else if (fN == 1)//若邻接面只有一个，也不是边界点
			isBound[i] = false;
		else//其余情况则是边界点
			isBound[i] = true;
	}
}

GLdouble * Model::GetVertex(int index)//返回某个点
{
	return vertex[index];
}