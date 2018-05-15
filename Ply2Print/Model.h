#pragma once

#include<GL/glut.h>
#include<set>
#include"Calculate.h"//��������������㺯��
using namespace std;

typedef GLdouble Vertex[3];//GLdouble[3]����������Ϊ��������
typedef int Facet[3];//��int[3]������Ϊ��Ƭ
typedef double Normal[3];//��double[3]������Ϊ������

class Model
{
private:
	int vertexN;//������
	int facetN;//��Ƭ��
	Vertex * vertex;//����ļ���
	Facet * facet;//��Ƭ�ļ���
	Normal * vertexNormal;//�㷨����
	Normal * facetNormal;//�淨����
	bool * isBound;//���ÿ�����Ƿ�Ϊ�߽�������
	set < int > * vertexDegree;//��¼��ÿ�������ڽӵĵ�����
	set < int > * facetDegree;//��¼��ÿ�������ڽӵ�������

	void computeFaceNormal(void);//�����淨����
	void computeVertexNormal(void);//����㷨����
public:
	Model();
	void CopyModel(Model * &);//��ģ�Ϳ���
	void ComputeNormal(void);//���㷨����
	void ClearModel(void);//���ģ�͵�һ����Ϣ
	void ComputeDegree(void);//�����ڽӵ����ڽӱ�
	bool ReadFile(CString);//���ļ��ж�ȡģ�͵���Ϣ
	int GetVertexN(void);//��ȡ������
	int GetFacetN(void);//��ȡ��Ƭ��
	Vertex * GetVertex(void);//��ȡ��������
	Facet * GetFacet(void);//��ȡ��Ƭ����
	Normal * GetVertexNormal(void);//��ȡ�㷨����������
	Normal * GetFacetNormal(void);//��ȡ�淨��������
	GLdouble * GetVertex(int index);//����ĳ����
	double GetVolume(void);//��ȡģ�����
	bool * GetBound(void);//��ȡ�߽�����
	set<int> * GetVertexDegree(void);//��ȡ������ڽӱ�
	set<int> * GetFacetDegree(void);//��ȡ��Ƭ���ڽӱ�
	void Scale(double);//�ɱ�������
	virtual ~Model();
};

