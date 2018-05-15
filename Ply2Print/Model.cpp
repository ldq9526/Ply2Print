#include "stdafx.h"
#include "Model.h"


Model::Model()
{
	vertexN = 0;//������Ĭ��Ϊ0
	facetN = 0;//��Ƭ��Ĭ��Ϊ0
	vertex = nullptr;//���㼯��Ϊ��
	facet = nullptr;//��Ƭ����Ϊ��
	vertexNormal = nullptr;//�㷨����Ϊ��
	facetNormal = nullptr;//�淨����Ϊ��
	isBound = nullptr;//�߽���ж�Ϊ��
	vertexDegree = nullptr;//���ڽӱ�Ϊ��
	facetDegree = nullptr;//���ڽӱ�Ϊ��
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

bool Model::ReadFile(CString filepath)//��ȡ�ļ���Ϣ����ģ��
{
	CString info, str;
	CStdioFile file;
	if (!file.Open(filepath, CFile::modeRead))//��ȡ�ļ�
		return false;
	else
	{
		file.ReadString(str);//��ȡһ�е�str
		swscanf_s(str, _T("%d"), &vertexN);//��ȡ������
		file.ReadString(str);//��ȡһ�е�str
		swscanf_s(str, _T("%d"), &facetN);//��ȡ��Ƭ��

		vertex = new Vertex[vertexN];//Ϊ������������ڴ�ռ�
		isBound = new bool[vertexN];//Ϊ�߽���־�����ڴ�ռ�
		vertexDegree = new set<int>[vertexN];//Ϊ���ڽӱ�����ڴ�ռ�
		facetDegree = new set<int>[vertexN];//Ϊ���ڽӱ�����ڴ�ռ�

		facet = new Facet[facetN];//Ϊ��Ƭ��������ڴ�

		// ���ζ���������
		for (int i = 0; i < vertexN; i++)
		{
			file.ReadString(str);
			swscanf_s(str, _T("%lf %lf %lf"), &vertex[i][0], &vertex[i][1], &vertex[i][2]);
		}
		//���ζ�����Ƭ����Ϣ
		for (int i = 0; i < facetN; i++)
		{
			int temp;
			file.ReadString(str);
			swscanf_s(str, _T("%d %d %d %d"), &temp, &facet[i][0], &facet[i][1], &facet[i][2]);
		}
		ComputeNormal();//���㷨����
		file.Close();//�ر��ļ�
		return true;
	}
}

void Model::CopyModel(Model * & backupModel)//��ģ�ͱ���һ�ݵ�backupModel
{
	if (backupModel == nullptr)
		backupModel = new Model();//���ģ��Ϊ��
	else
		backupModel->ClearModel();//��������еı���ģ��

	backupModel->vertexN = vertexN;//���ƶ�����
	backupModel->facetN = facetN;//������Ƭ��

	backupModel->vertex = new Vertex[vertexN];//���춥������
	backupModel->vertexNormal = new Normal[vertexN];//����㷨��������
	backupModel->isBound = new bool[vertexN];//����߽�ֵ����
	backupModel->vertexDegree = new set<int>[vertexN];//���춥���ڽӱ�
	backupModel->facetDegree = new set<int>[vertexN];//�������ڽӱ�
	backupModel->facet = new Facet[facetN];//������Ƭ����
	backupModel->facetNormal = new Normal[facetN];//�����淨��������
	
	for (int i = 0; i < vertexN; i++)//���ƶ�����Ϣ
	{
		//���ƶ���
		backupModel->vertex[i][0] = vertex[i][0];
		backupModel->vertex[i][1] = vertex[i][1];
		backupModel->vertex[i][2] = vertex[i][2];
		//���ƶ����Ƿ�Ϊ�߽���Ϣ
		backupModel->isBound[i] = isBound[i];
	}

	for (int i = 0; i < facetN; i++)//������Ƭ��Ϣ
	{
		//������Ƭ
		backupModel->facet[i][0] = facet[i][0];
		backupModel->facet[i][1] = facet[i][1];
		backupModel->facet[i][2] = facet[i][2];
	}
	backupModel->ComputeNormal();//���¼��㷨����
}

Vertex * Model::GetVertex(void)//��ȡ��������
{
	return vertex;
}

Facet * Model::GetFacet(void)//��ȡ��Ƭ����
{
	return facet;
}

int Model::GetVertexN(void)//��ȡ������
{
	return vertexN;
}

int Model::GetFacetN(void)//��ȡ��Ƭ��
{
	return facetN;
}

Normal * Model::GetVertexNormal(void)//��ȡ�㷨����������
{
	return vertexNormal;
}

Normal * Model::GetFacetNormal(void)//��ȡ�淨��������
{
	return facetNormal;
}

void Model::computeFaceNormal(void)//����ÿ����Ƭ�ķ�����
{
	if (facetNormal != nullptr)
		delete[] facetNormal;
	facetNormal = new Normal[facetN];
	for (int i = 0; i < facetN; i++)
	{
		double u[3], v[3];
		vec(u, vertex[facet[i][0]], vertex[facet[i][1]]);//��Ƭ��һ������
		vec(v, vertex[facet[i][0]], vertex[facet[i][2]]);//��һ������������
		cross(facetNormal[i], u, v);//���������÷�����
		double L = length(facetNormal[i]);//�������ĳ���
		if (fabs(L - 0.0) < 1e-5)//����������Ϊ0
			times(facetNormal[i], 0.0, facetNormal[i]);//ֱ��ȷ��Ϊ0����
		else
			times(facetNormal[i], 1.0/L, facetNormal[i]);//��λ��
	}
}

void Model::ComputeNormal(void)//���㷨����
{
	computeFaceNormal();//�ȼ����淨��
	computeVertexNormal();//�ټ���ÿ���㴦�ķ�����
}

/*�ȼ�����Ƭ�ķ�����,�ٸ�����Ƭ�ķ��������ÿ����ķ�����*/
void Model::computeVertexNormal(void)//����ÿ���㴦�ķ�����
{
	if (vertexNormal != nullptr)
		delete[] vertexNormal;
	vertexNormal = new Normal[vertexN];

	for (int i = 0; i < vertexN; i++)
		vertexNormal[i][0] = vertexNormal[i][1] = vertexNormal[i][2] = 0.0;

	for (int i = 0; i < facetN; i++)//ÿ���㴦�ķ�����
	{
		//��Ƭ��1�����������ϳ�
		vertexNormal[facet[i][0]][0] += facetNormal[i][0];
		vertexNormal[facet[i][0]][1] += facetNormal[i][1];
		vertexNormal[facet[i][0]][2] += facetNormal[i][2];
		//��Ƭ��2�����������ϳ�
		vertexNormal[facet[i][1]][0] += facetNormal[i][0];
		vertexNormal[facet[i][1]][1] += facetNormal[i][1];
		vertexNormal[facet[i][1]][2] += facetNormal[i][2];
		//��Ƭ��3�����������ϳ�
		vertexNormal[facet[i][2]][0] += facetNormal[i][0];
		vertexNormal[facet[i][2]][1] += facetNormal[i][1];
		vertexNormal[facet[i][2]][2] += facetNormal[i][2];
	}
	//ÿ���㴦�ķ�������λ��
	for (int i = 0; i < vertexN; i++)
	{
		double L = length(vertexNormal[i]);//���㷨��������
		if (fabs(L - 0.0) < 1e-5)
			times(vertexNormal[i], 0.0, vertexNormal[i]);//ֱ��ȷ��Ϊ0����
		else
			times(vertexNormal[i], 1.0 / L, vertexNormal[i]);
	}
}

double Model::GetVolume(void)//��ȡģ�����:����֮ǰ�����ȸ��·�����
{
	double vol = 0.0;
	double g[3];//��¼ĳ���������
	double u[3], v[3];//u,v�ֱ���ĳ�����ϲ������ߵ���������
	double n[3];//n����Ƭ�ķ�����
	ComputeNormal();//���¼��㷨����
	for (int i = 0; i < facetN; i++)
	{
		g[0] = (vertex[facet[i][0]][0] + vertex[facet[i][1]][0] + vertex[facet[i][2]][0]) / 3.0;
		g[1] = (vertex[facet[i][0]][1] + vertex[facet[i][1]][1] + vertex[facet[i][2]][1]) / 3.0;
		g[2] = (vertex[facet[i][0]][2] + vertex[facet[i][1]][2] + vertex[facet[i][2]][2]) / 3.0;

		vec(u, vertex[facet[i][1]], vertex[facet[i][0]]);//��u
		vec(v, vertex[facet[i][2]], vertex[facet[i][0]]);//��v
		cross(n, u, v);//������n

		vol += (fabs(dot(g, n))) / 6.0;//��ϻ�a.b.c/6
	}
	return vol;
}

void Model::ClearModel(void)//���ģ�͵�һ����Ϣ
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

bool * Model::GetBound(void)//��ȡ�߽�����
{
	return isBound;
}

set<int> * Model::GetVertexDegree(void)//��ȡ������ڽӱ�
{
	return vertexDegree;
}

set<int> * Model::GetFacetDegree(void)//��ȡ��Ƭ���ڽӱ�
{
	return facetDegree;
}

void Model::Scale(double rate)//�ɱ�������
{
	for (int i = 0; i < vertexN; i++)
	{
		vertex[i][0] *= rate;
		vertex[i][1] *= rate;
		vertex[i][2] *= rate;
	}
}

void Model::ComputeDegree(void)//�����ڽӵ����ڽӱ�
{
	for (int i = 0; i < facetN; i++)
	{
		facetDegree[facet[i][0]].insert(i);//��1����������ڽ���
		facetDegree[facet[i][1]].insert(i);//��2����������ڽ���
		facetDegree[facet[i][2]].insert(i);//��3����������ڽ���
		for (int j = 0; j < 3; j++)//������Ƭ��¼ÿ������ڽӵ�
		{
			if (facet[i][j] != facet[i][(j + 1) % 3])
				vertexDegree[facet[i][j]].insert(facet[i][(j + 1) % 3]);
			if (facet[i][j] != facet[i][(j + 2) % 3])
				vertexDegree[facet[i][j]].insert(facet[i][(j + 2) % 3]);
		}
	}
	for (int i = 0; i < vertexN; i++)//�ж�ÿ�����Ƿ�Ϊ�߽��
	{
		int vN = vertexDegree[i].size();//���i�ڽӵĵ�ĸ���
		int fN = facetDegree[i].size();//���i�ڽӵ���ĸ���
		if (vN == fN)//�ڽӵ����ڽ��������ͬ,���Ǳ߽��
			isBound[i] = false;
		else if (fN == 1)//���ڽ���ֻ��һ����Ҳ���Ǳ߽��
			isBound[i] = false;
		else//����������Ǳ߽��
			isBound[i] = true;
	}
}

GLdouble * Model::GetVertex(int index)//����ĳ����
{
	return vertex[index];
}