
// Ply2PrintView.cpp : CPly2PrintView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Ply2Print.h"
#endif

#include "Ply2PrintDoc.h"
#include "Ply2PrintView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPly2PrintView

IMPLEMENT_DYNCREATE(CPly2PrintView, CGLEnabledView)

BEGIN_MESSAGE_MAP(CPly2PrintView, CGLEnabledView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CPly2PrintView::OnFileOpen)//���ļ�
	ON_COMMAND(ID_VIEW_SMOOTH, &CPly2PrintView::OnVertexNormal)//�㷨��
	ON_COMMAND(ID_VIEW_WIREFRAME, &CPly2PrintView::OnFacetNormal)//�淨��
	ON_COMMAND(ID_32772, &CPly2PrintView::OnFill)//��Ƭ��䷽ʽ����
	ON_COMMAND(ID_32773, &CPly2PrintView::OnLine)//������䷽ʽ����
	ON_COMMAND(ID_32774, &CPly2PrintView::OnPoint)//�㷽ʽ����
	ON_COMMAND(ID_32776, &CPly2PrintView::OnWhiteLight)//��ɫ�ƹ�
	ON_COMMAND(ID_32777, &CPly2PrintView::OnColorLight)//��ɫ�ƹ�
	ON_COMMAND(ID_32788, &CPly2PrintView::OnPrintModelInfo)//��ʾģ����Ϣ
	ON_COMMAND(ID_32789, &CPly2PrintView::OnRecover)//�ָ�ģ��
	ON_COMMAND(ID_FILE_SAVE, &CPly2PrintView::OnFileSave)//�����ļ�
	ON_COMMAND(ID_EDIT_SET, &CPly2PrintView::OnEditSet)//��������
	ON_COMMAND(ID_32781, &CPly2PrintView::OnSelect)//����ʰȡģʽ
	ON_COMMAND(ID_32791, &CPly2PrintView::OnStatement)//����˵��
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CPly2PrintView ����/����

CPly2PrintView::CPly2PrintView()
{
	// TODO:  �ڴ˴���ӹ������
	//ģ�Ͷ����ʼ��
	sourceModel = nullptr;//Դ����ģ������Ϊ��
	backupModel = nullptr;//��������ģ������Ϊ��
	selectedVertex.clear();//ʰȡ�������
	selectedFacet.clear();//ʰȡ�����

	//ģ�ͻ���ģʽ��ʼ��
	initDrawMode();
	
	//������ʾ���Ƴ�ʼ��
	initControl();
	
	//�۲���ʼ��
	near_point[0] = near_point[1] = near_point[2] = 0.0;
	far_point[0] = far_point[1] = far_point[2] = 0.0;
	CrossPoint[0] = CrossPoint[1] = CrossPoint[2] = 0.0;
}

CPly2PrintView::~CPly2PrintView()
{
	if (sourceModel != nullptr)
	{
		sourceModel->ClearModel();//���ģ��
		delete sourceModel;
	}
	if (backupModel != nullptr)//��ձ���ģ��
	{
		backupModel->ClearModel();
		delete backupModel;
	}
}

void CPly2PrintView::OnDraw(CDC* pDC)//�ػ溯��
{
	CGLEnabledView::OnDraw(pDC);
	this->OnDrawGL(pDC);
}

// CPly2PrintView ��ӡ

BOOL CPly2PrintView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CPly2PrintView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CPly2PrintView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}


// CPly2PrintView ���

#ifdef _DEBUG
void CPly2PrintView::AssertValid() const
{
	CView::AssertValid();
}

void CPly2PrintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPly2PrintDoc* CPly2PrintView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPly2PrintDoc)));
	return (CPly2PrintDoc*)m_pDocument;
}
#endif //_DEBUG

//�ڲ�˽�к���

//���ճ�ʼ��
void CPly2PrintView::InitalLigt()
{
	//���ù�Դ
	GLfloat light_position1[4] = { -52, -16, -50, 0 };
	GLfloat light_position2[4] = { -26, -48, -50, 0 };
	GLfloat light_position3[4] = { 16, -52, -50, 0 };
	GLfloat light_position4[4] = { 52, 16, 50, 0 };
	GLfloat light_position5[4] = { 26, 48, 50, 0 };
	GLfloat light_position6[4] = { -16, 52, 50, 0 };
	//���ù��շ���
	GLfloat direction1[3] = { 52, 16, 50 };
	GLfloat direction2[3] = { 26, 48, 50 };
	GLfloat direction3[3] = { -16, 52, 50 };
	GLfloat direction4[3] = { -52, -16, -50 };
	GLfloat direction5[3] = { -26, -48, -50 };
	GLfloat direction6[3] = { 16, -52, -50 };

	GLfloat color1[4], color2[4], color3[4], color4[4], color5[4], color6[4];

	glClearColor(1, 1, 1, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	if (color_type == COLOR_LIGHT)//��ɫ�ƹ�
	{
		color1[0] = 1.0f; color1[1] = 0.0f; color1[2] = 0.0f; color1[3] = 1.0f;
		color2[0] = 0.0f; color2[1] = 1.0f; color2[2] = 0.0f; color2[3] = 1.0f;
		color3[0] = 0.0f; color3[1] = 0.0f; color3[2] = 1.0f; color3[3] = 1.0f;
		color4[0] = 1.0f; color4[1] = 0.0f; color4[2] = 0.0f; color4[3] = 1.0f;
		color5[0] = 0.0f; color5[1] = 1.0f; color5[2] = 0.0f; color5[3] = 1.0f;
		color6[0] = 0.0f; color6[1] = 0.0f; color6[2] = 1.0f; color6[3] = 1.0f;

		GLfloat ambient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };//������

		//��������
		GLfloat material_color[4] = { 1.0f, 1.0f, 1.0f, 0.5f };//��ɫ������
		GLfloat material_specular[4] = { 0.5f, 0.5f, 0.5f, 0.5f };//�������淴��׹�
		GLfloat material_ambient[4] = { 0.0f, 0.0f, 0.0f, 0.0f };//�޻�����

		glLightfv(GL_LIGHT3, GL_POSITION, light_position4);
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, direction4);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, color4);
		glLightfv(GL_LIGHT3, GL_SPECULAR, color4);

		glLightfv(GL_LIGHT4, GL_POSITION, light_position5);
		glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, direction5);
		glLightfv(GL_LIGHT4, GL_DIFFUSE, color5);
		glLightfv(GL_LIGHT4, GL_SPECULAR, color5);

		glLightfv(GL_LIGHT5, GL_POSITION, light_position6);
		glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, direction6);
		glLightfv(GL_LIGHT5, GL_DIFFUSE, color6);
		glLightfv(GL_LIGHT5, GL_SPECULAR, color6);

		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);

		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT3);
		glEnable(GL_LIGHT4);
		glEnable(GL_LIGHT5);

		glDisable(GL_COLOR_MATERIAL);
		return;
	}
	else{
		//��ɫ�ƹ�
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4);
		glDisable(GL_LIGHT5);
		glDisable(GL_LIGHTING);
		GLfloat m_LightPostion[4] = { 0.0f, 10.0f, 10.0f, 1.0f };

		GLfloat ambientLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
		GLfloat diffuseLight[] = { 0.5, 0.5f, 0.5f, 1.0f };
		GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };

		glEnable(GL_LIGHTING);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
		glLightfv(GL_LIGHT0, GL_POSITION, m_LightPostion);
		glEnable(GL_LIGHT0);

		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	}
}

//��ʼ��ģ�ͻ�����Ϣ
void CPly2PrintView::initDrawMode()
{
	style = M_FILL;//Ĭ����Ƭ����		
	cull = true;//cull face
	zbuffer = true;//����z����
	show_view = true;//Ĭ����ʾͼ��
	PNormal = M_FACETNORMAL;//Ĭ��Ϊ�淨������
	color_type = WHITE_LIGHT;//Ĭ��Ϊ�׹�
}

//��ʼ����ʾ������Ϣ
void CPly2PrintView::initControl()
{
	smoothed = false;//Ĭ�ϻ�û�о�����ƽ������
	select = false;//Ĭ�ϲ���ʰȡģʽ
	leftbuttondown = false;//Ĭ�����û�а���
	showView = false; //Ĭ�ϲ���ʾ����
	showFace = false;//����ʾ��ʰȡ����
}

//�Ƿ���ĳ����Ƭ�ཻ
int CPly2PrintView::isInFacet(double x, double y, double z)
{
	Vertex * vertex = sourceModel->GetVertex();//��ȡ��������
	Facet * facet = sourceModel->GetFacet();//��ȡ��Ƭ����
	Normal * normal = sourceModel->GetFacetNormal();//��ȡ��Ƭ�ķ�����
	int facetN = sourceModel->GetFacetN();//��ȡ��Ƭ����
	int index = -1;//��¼��С����Ƭ������,��ʼ��Ϊ-1
	double distance = DBL_MAX;//��¼���㵽���ӵ�ľ���,��ʼ��Ϊ�����

	for (int i = 0; i < facetN; i++)
	{
		double v[3];//��¼��������Ƭ�ķ�����
		for (int j = 0; j < 3; j++)
			v[j] = normal[i][j];

		double A[3], B[3], C[3];//��Ƭ�������������
		for (int j = 0; j < 3; j++)
			A[j] = vertex[facet[i][0]][j];
		for (int j = 0; j < 3; j++)
			B[j] = vertex[facet[i][1]][j];
		for (int j = 0; j < 3; j++)
			C[j] = vertex[facet[i][2]][j];

		double D;//ƽ�淽��Ax + By + Cz + D = 0�ĳ�����,����v = (A,B,C)Ϊ������
		D = -dot(v, A);//����ƽ����ĳһ���뷨�������㳣���� D = -(A * x0 + B * y0 + C * z0)

		double tempU, tempD;//��ʱ����
		tempU = dot(v, near_point) + D;
		tempD = v[0] * x + v[1] * y + v[2] * z;
		if (fabs(tempD - 0.0) < 1e-5)
			continue;//ֱ����ƽ��ƽ�л���ƽ����
		else
		{
			double t = -tempU / tempD;//����ֱ�߲������̵Ĳ���

			double P[3];//��¼��ʱ����
			//���㽻������
			P[0] = x * t + near_point[0];
			P[1] = y * t + near_point[1];
			P[2] = z * t + near_point[2];

			if (inTriangle(P, A, B, C))//�жϵ��Ƿ����������ڲ�
			{
				double tmpdis = dist(near_point, P);//������ӵ㵽�������
				if (tmpdis < distance)
				{
					CrossPoint[0] = P[0];//���潻�������
					CrossPoint[1] = P[1];
					CrossPoint[2] = P[2];
					distance = tmpdis;
					index = i;
				}
			}
		}
	}
	return index;//���ؾ�����ӵ�������������
}

//��ȡ���㵽��Ƭindex������������
int CPly2PrintView::getClosest(int index)
{
	Vertex * vertex = sourceModel->GetVertex();//��ȡ��������
	Facet * facet = sourceModel->GetFacet();//��ȡ��Ƭ����
	double l = dist(CrossPoint, vertex[facet[index][0]]);
	int n = facet[index][0];//n��¼��Ƭ�϶��������
	for (int i = 1; i < 3; i++)
	{
		double L = dist(CrossPoint, vertex[facet[index][i]]);
		if (L < l)//����ҵ������С�ĵ����滻��׼
		{
			l = L;
			n = facet[index][i];
		}
	}
	return n;//���ؾ�������Ķ��������
}

//ʰȡ����
void CPly2PrintView::selectVertex(int x, int y)
{
	GLdouble    modelview[16];
	GLdouble    projection[16];
	GLint       viewport[4];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	glPushMatrix();//���浱ǰģʽ����һ�α任

	glLoadIdentity();
	glTranslated(move_X, move_Y, -25.0 + scale); //ƽ�����ܹ��۲쵽����  
	glRotated(angle_X, 1.0, 0.0, 0.0);//��x����ת
	glRotated(angle_Y, 0.0, 1.0, 0.0);//��y����ת
	glRotated(0.0, 0.0, 0.0, 1.0);//����z����ת

	glPopMatrix();

	// ��ȡ���ü����ϵĽ������near_point[]����
	gluUnProject((GLdouble)x, (GLdouble)(viewport[3] - y), 0.0,
		modelview, projection, viewport,
		near_point, near_point + 1, near_point + 2);

	// ��ȡԶ�ü����ϵĽ������far_point[]����
	gluUnProject((GLdouble)x, (GLdouble)(viewport[3] - y), 1.0,
		modelview, projection, viewport,
		far_point, far_point + 1, far_point + 2);

	//��ȡԶ���ü��潻�������v[]
	double v[3];
	vec(v, far_point, near_point);//��Զָ���
	int facetIndex = isInFacet(v[0], v[1], v[2]);//��ȡ���ཻ�Ľ���
	if (facetIndex != -1)
	{
		int vertexIndex = getClosest(facetIndex);//��ȡ��������ĵ�����
		selectedVertex.insert(vertexIndex);//����ѡ�㼯��
		selectedFacet.insert(facetIndex);//������ļ���
		GLdouble * point = sourceModel->GetVertex(vertexIndex);
		myText.Format(_T("ѡ����%d:(%.2f,%.2f,%.2f)"), vertexIndex, point[0], point[1], point[2]);
		select = false;//����ʰȡ����
	}
	else
		myText = _T("δѡ���κε�");
}

//���ĵ����Ͻ���ʾ��Ϣ����
void CPly2PrintView::drawMyText()
{
	CDC *pDC = GetWindowDC();//��ȡ�豸�����Ļ���
	pDC->DrawText(myText, CRect(6, 6, 160, 420), DT_WORDBREAK); ReleaseDC(pDC);
}

//����Դģ���Լ�����ģ��
bool CPly2PrintView::DrawModel(bool show, bool cull, int style, bool normalpoint, bool zbuffer)
{
	int vertexN = sourceModel->GetVertexN();//��ȡ������
	Vertex * vertex = sourceModel->GetVertex();//��ȡ��������
	int facetN = sourceModel->GetFacetN();//��ȡ��Ƭ��
	Facet * facet = sourceModel->GetFacet();//��ȡ��Ƭ����
	Normal * faceNormal = sourceModel->GetFacetNormal();//��ȡ�淨����
	Normal * vertexNormal = sourceModel->GetVertexNormal();//��ȡ���㷨����

	if (!show)
		return true;//��Ҫ����ʾ��ֱ���˳���ɻ���
	if (select && leftbuttondown)
		selectVertex(oldmx, oldmy);//���
	if (style == M_POINT)	//���Ƶ�ͼ
	{
		glPointSize(1.0);
		glColor3d(0.0, 0.0, 0.0);
		glBegin(GL_POINTS);
		for (int i = 0; i < vertexN; i++)
			glVertex3dv(vertex[i]);
		glEnd();
		glColor3d(1.0, 1.0, 1.0);//�ָ�����ɫ
		return true;
	}

	if (cull)//Ҫ������������
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	if (zbuffer)//����zbufferģʽ
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	if (style == M_FILL)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//��Ƭ���ʽ
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�������ʽ

	for (int i = 0; i < facetN; i++)
	{
		if (style == M_LINE)
		{
			glLineWidth(0.1f);//�������1.0����
			glColor3d(0.0, 0.0, 0.0);//��ɫ��������
		}
		if (normalpoint == M_FACETNORMAL)	//�����淨��������Ƭ
		{
			glNormal3dv(faceNormal[i]);
			glBegin(GL_TRIANGLES);
			glVertex3dv(vertex[facet[i][0]]);
			glVertex3dv(vertex[facet[i][1]]);
			glVertex3dv(vertex[facet[i][2]]);
			glEnd();
		}
		else				//�㷨��������Ƭ
		{
			glBegin(GL_TRIANGLES);
			glNormal3dv(vertexNormal[facet[i][0]]);
			glVertex3dv(vertex[facet[i][0]]);
			glNormal3dv(vertexNormal[facet[i][1]]);
			glVertex3dv(vertex[facet[i][1]]);
			glNormal3dv(vertexNormal[facet[i][2]]);
			glVertex3dv(vertex[facet[i][2]]);
			glEnd();
		}
	}
	if (style == M_LINE && selectedVertex.size() > 0)
	{
		glColor3d(1.0, 0.0, 0.0);//��ɫ���
		glPointSize(5.0);//���СΪ5����
		glBegin(GL_POINTS);
		for (set<int>::iterator it = selectedVertex.begin(); it != selectedVertex.end(); it++)
			glVertex3dv(vertex[*it]);
		glEnd();
	}
	if (style == M_LINE && showFace &&selectedFacet.size() > 0)
	{
		glColor3d(1.0, 0.0, 0.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_TRIANGLES);
		for (set<int>::iterator it = selectedFacet.begin(); it != selectedFacet.end(); it++)
		{
			glVertex3dv(vertex[facet[*it][0]]);
			glVertex3dv(vertex[facet[*it][1]]);
			glVertex3dv(vertex[facet[*it][2]]);
		}
		glEnd();
	}
	if (showView)
	{
		glColor3d(0.0, 1.0, 0.0);//��ɫ����
		glLineWidth(5.0);//������ص�
		glBegin(GL_LINES);
		glVertex3dv(near_point);
		glVertex3dv(far_point);
		glEnd();
	}
	glColor3d(1.0, 1.0, 1.0);//�ָ�����ɫ
	return true;
}

//��ʼ�����յ�����������ģ��
void CPly2PrintView::OnDrawGL(CDC *pDC)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//����ɫ��ɫ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	InitalLigt();		///��ʼ��������Ϣ
	glPushMatrix();

	glTranslated(move_X, move_Y, scale);//ƽ��
	glRotated(angle_Y, 0.0, 1.0, 0.0);
	glRotated(angle_X, 1.0, 0.0, 0.0);

	if (sourceModel != nullptr)	//�������ģ���ļ�
	{
		cull = false;	//����۲��ڲ�
		DrawModel(show_view, cull, style, PNormal, zbuffer);
	}
	else{
		myText = _T("���һ��ply2�ļ�");
	}

	glPopMatrix();
}

//�Լ�ʵ�ֵ�ƽ�����㺯��
void CPly2PrintView::Smooth(double dt)
{
	int vertexN = sourceModel->GetVertexN();//��ȡ�������
	Vertex * vertex = sourceModel->GetVertex();//��ȡ��������
	bool * isBound = sourceModel->GetBound();//��ȡ�߽����Ϣ
	set<int> * degree = sourceModel->GetVertexDegree();//��ȡ�����ڽӱ�

	for (int i = 0; i < vertexN; i++)
	{
		if (isBound[i] || degree[i].size() == 0)
			continue;//����Ǳ߽��,�����겻��
		else
		{
			double L[3] = { 0.0, 0.0, 0.0 };//��������
			double sub[3];//��¼������
			for (set<int>::iterator it = degree[i].begin(); it != degree[i].end(); it++)
			{
				vec(sub, vertex[*it], vertex[i]);
				add(L, L, sub);
			}
			double k = 1.0 / degree[i].size();
			times(L, k, L);//��������
			if (dt < 1.0)
				times(L, dt, L);
			else
				times(L, 0.1, L);
			add(vertex[i], vertex[i], L);//����
		}
	}
}

// CPly2PrintView ��Ϣ�������

//���ļ���Ӧ����
void CPly2PrintView::OnFileOpen()
{
	// TODO:  �ڴ���������������
	CString filter = _T("PLY2 File (*.PLY2)|*.ply2||");
	CFileDialog fileDlg(TRUE, NULL, NULL, NULL, filter, NULL);
	fileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;
	fileDlg.m_ofn.lpstrTitle = _T("��ply2�ļ�");//�ļ�ѡ������ֻѡ��ply2�ļ�

	if (fileDlg.DoModal() == IDOK)//��������ȷ��
	{
		//ģ�ͻ���ģʽ��ʼ��
		initDrawMode();

		//��ʾ����ģʽ��ʼ��
		initControl();
		
		//ģ�Ͷ������
		selectedVertex.clear();//ʰȡ�������
		selectedFacet.clear();//ʰȡ�����
		if (backupModel != nullptr)
		{
			backupModel->ClearModel();
			delete backupModel;
			backupModel = nullptr;
		}
		if (sourceModel != nullptr)//Դģ�Ͳ�Ϊ���������
		{
			sourceModel->ClearModel();
			delete sourceModel;
		}
		sourceModel = new Model();
		bool flag = sourceModel->ReadFile(fileDlg.GetPathName());//��ȡģ����Ϣ
		if (flag)//��ȡģ�ͳɹ�
		{
			myText.Format(_T("����:%d \n��Ƭ��:%d"), sourceModel->GetVertexN(), sourceModel->GetFacetN());
		}
		this->OnDraw(this->GetDC());//���ƽ���
	}
}

//�㷨��ʽ����
void CPly2PrintView::OnVertexNormal()
{
	// TODO:  �ڴ���������������
	if (sourceModel != nullptr)
	{
		PNormal = M_VERTEXNORMAL;//�㷨��ʽ����
		myText = _T("�㷨��ʽ");
	}
	else
		myText = _T("���ȴ�һ��ply2�ļ�");
	this->OnDraw(this->GetDC());	//�ػ����
}

//�淨�����
void CPly2PrintView::OnFacetNormal()
{
	// TODO:  �ڴ���������������
	if (sourceModel != nullptr)
	{
		PNormal = M_FACETNORMAL;//�淨��ʽ����
		myText = _T("�淨��ʽ");
	}
	else
		myText = _T("���ȴ�һ��ply2�ļ�");
	this->OnDraw(this->GetDC());	//�ػ����
}

//����ģʽ
void CPly2PrintView::OnPoint()
{
	// TODO:  �ڴ���������������
	if (sourceModel != nullptr)
	{
		style = M_POINT;//�����ģʽ
		myText = _T("�����ģʽ");
	}
	else
		myText = _T("���ȴ�һ��ply2�ļ�");
	this->OnDraw(this->GetDC());	//�ػ����
}

//�������ģʽ
void CPly2PrintView::OnLine()
{
	// TODO:  �ڴ���������������
	if (sourceModel != nullptr)
	{
		style = M_LINE;//����
		myText = _T("�������ģʽ");
	}
	else
		myText = _T("���ȴ�һ��ply2�ļ�");
	this->OnDraw(this->GetDC());	//�ػ����
}

//��Ƭ���ģʽ
void CPly2PrintView::OnFill()
{
	// TODO:  �ڴ���������������
	if (sourceModel != nullptr)
	{
		style = M_FILL;//��Ƭ���ģʽ
		myText = _T("��Ƭ���ģʽ");
	}
	else
		myText = _T("���ȴ�һ��ply2�ļ�");
	this->OnDraw(this->GetDC());	//�ػ����
}

//��ɫ�ƹ�
void CPly2PrintView::OnWhiteLight()
{
	// TODO:  �ڴ���������������
	myText = _T("��ɫ�ƹ�");
	color_type = WHITE_LIGHT;//��ɫ�ƹ�
	this->OnDraw(this->GetDC());	//�ػ����
}

//��ɫ�ƹ�
void CPly2PrintView::OnColorLight()
{
	// TODO:  �ڴ���������������
	myText = _T("��ɫ�ƹ�");
	color_type = COLOR_LIGHT;
	this->OnDraw(this->GetDC());	//�ػ����
}

//��ʾģ�͵������Լ����
void CPly2PrintView::OnPrintModelInfo()
{
	// TODO:  �ڴ���������������
	if (sourceModel != nullptr){
		myText.Format(_T("����:%d \n��Ƭ��:%d\nģ�����:%.2f"),
			sourceModel->GetVertexN(),
			sourceModel->GetFacetN(),
			sourceModel->GetVolume());
	}
	this->OnDraw(this->GetDC());
}

//�����ļ�
void CPly2PrintView::OnFileSave()
{
	// TODO:  �ڴ���������������
	if (sourceModel == nullptr){
		myText = _T("���ȴ�һ��ģ��");
		drawMyText();
		this->InvalidateRect(NULL, FALSE);
		return;
	}
	myText = _T("׼������ģ��");
	drawMyText();
	this->InvalidateRect(NULL, FALSE);
	CString buffer;
	CString szFilter = _T("Ply2�ļ�(*.ply2)|*.ply2|�����ļ�(*.*)|*.*||");
	// ���챣���ļ��Ի���    
	CFileDialog fileDlg(FALSE, _T("ply2"), _T("newModel"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString strFilePath;
	CStdioFile file;

	// ��ʾ�����ļ��Ի���    
	if (IDOK == fileDlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ����桱��ť����ѡ����ļ�·����ʾ���༭����    
		strFilePath = fileDlg.GetPathName();
		file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		int vertexN = sourceModel->GetVertexN();//��ȡ������
		int facetN = sourceModel->GetFacetN();//��ȡ��Ƭ��
		Vertex * vertex = sourceModel->GetVertex();//��ȡ��������
		Facet * facet = sourceModel->GetFacet();//��ȡ��Ƭ��
		buffer.Format(_T("%d\n"), vertexN);
		file.WriteString(buffer);  //д�����
		buffer.Format(_T("%d\n"), facetN);
		file.WriteString(buffer);  //д����Ƭ��
		//д��������
		for (int i = 0; i < vertexN; ++i){
			buffer.Format(_T("%lf %lf %lf\n"), vertex[i][0], vertex[i][1], vertex[i][2]);
			file.WriteString(buffer);
		}
		//д����Ƭ����
		for (int i = 0; i < facetN; ++i){
			buffer.Format(_T("3 %d %d %d\n"), facet[i][0], facet[i][1], facet[i][2]);
			file.WriteString(buffer);
		}
		file.Close();
		myText = _T("����ģ�ͳɹ�");
		drawMyText();
		this->InvalidateRect(NULL, FALSE);
	}
}

//����ͼ��ƽ���Ĳ���
void CPly2PrintView::OnEditSet()
{
	// TODO:  �ڴ���������������
	surfaceDialog = new SurfaceDialog();//�����Ի���

	if (surfaceDialog->DoModal() == IDOK)//��������ȷ����ť
	{
		double step = surfaceDialog->m_step_length;//��ȡ����
		int times = surfaceDialog->m_times;//��ȡ��������
		int hold_v = surfaceDialog->m_check;//��ȡ�Ƿ񱣳����

		if (sourceModel != nullptr)
		{
			double original_volume;
			if (hold_v == 1)//�����Ҫ����������ȡģ�ͱ��������
				original_volume = sourceModel->GetVolume();
			if (smoothed == false)
			{
				sourceModel->CopyModel(backupModel);//�ȱ���һ�ݱ���ģ��
				smoothed = true;//�Ժ���ָ��������ģ��
			}
			sourceModel->ComputeDegree();//����ÿ�����ڽӵĵ�����
			while (times--)
			{
				myText.Format(_T("����%d / %d"), surfaceDialog->m_times - times, surfaceDialog->m_times);
				drawMyText();
				Smooth(step);//ƽ����΢�����������
			}
			myText = _T("�������");
			drawMyText();
			if (hold_v == 1)
			{
				double changed_volume = sourceModel->GetVolume();
				myText = _T("���ڻָ����");
				drawMyText();
				sourceModel->Scale(pow((original_volume / changed_volume), 1 / 3.0));
				myText = _T("�ָ�������");
			}
			else
				myText = _T("���ƽ��");
		}
		else
			myText = _T("���ȴ�һ��ply2�ļ�");
		this->OnDraw(this->GetDC());	//�ػ����
	}
}

//�ָ�֮ǰ��ģ��
void CPly2PrintView::OnRecover()
{
	// TODO:  �ڴ���������������
	if (backupModel == nullptr)
		myText = _T("δ���й�ƽ������");
	else
	{
		if (sourceModel != nullptr)
		{
			sourceModel->ClearModel();
			delete sourceModel;
		}
		sourceModel = new Model;
		myText = _T("���ڻָ���ʼģ��");
		drawMyText();
		backupModel->CopyModel(sourceModel);
		myText = _T("�ָ����");
	}
	this->OnDraw(this->GetDC());	//�ػ����
}

//����ʰȡģʽ�µ�������
void CPly2PrintView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	leftbuttondown = true;
	oldmx = point.x;//��¼�´�ʱ��xyֵ��Ϊ��ֵ
	oldmy = point.y;
	if (select)
		OnDraw(this->GetDC());
}

//����ʰȡģʽ�µ�����Ҽ�
void CPly2PrintView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (select)
	{
		select = false;
		myText = _T("");
		MessageBox(_T("�˳�ʰȡģʽ"));
		drawMyText();
		this->OnDraw(this->GetDC());	//�ػ����
	}
}

//����ʰȡģʽ
void CPly2PrintView::OnSelect()
{
	// TODO:  �ڴ���������������
	if (style != M_LINE)
		MessageBox(_T("��������ģʽ��ʰȡ!"));
	else
	{
		select = true;
		myText = _T("����ʰȡģʽ");
		drawMyText();
	}
}

//����ʰȡģʽ�µ�������ƶ�
void CPly2PrintView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (select)
		SetCursor(LoadCursor(NULL, IDC_HAND));//��������
	else
		SetCursor(LoadCursor(NULL,IDC_ARROW));//�ָ��ɼ�ͷ��
	if ((nFlags & MK_LBUTTON) == TRUE) //������������
	{
		int gapX = point.x - oldmx;
		int gapY = point.y - oldmy;
		oldmx = point.x;//���浱ǰ���걣֤��������ת
		oldmy = point.y;
		angle_X -= double(gapY);		//����������ת�Ƕ�
		angle_Y += double(gapX);		//����ˮƽ��ת�Ƕ�
		this->OnDraw(this->GetDC());
	}
}

//������̧��
void CPly2PrintView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	leftbuttondown = false;
	CGLEnabledView::OnLButtonUp(nFlags, point);
}

//�����ֿ��ƴ�С
BOOL CPly2PrintView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (zDelta > 0)//���Ϲ���
		scale += 0.2;//�Ŵ�
	else
		scale -= 0.2;//��С
	this->OnDraw(this->GetDC());//�ػ����
	return CView::OnMouseWheel(nFlags, zDelta, pt);
	return CGLEnabledView::OnMouseWheel(nFlags, zDelta, pt);
}

//���ⰴ������
void CPly2PrintView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nChar)
	{
	case VK_UP://����ƽ��
		move_Y += 0.2;
		this->OnDraw(this->GetDC());//�ػ����
		break;
	case VK_DOWN://����ƽ��
		move_Y -= 0.2;
		this->OnDraw(this->GetDC());//�ػ����
		break;
	case VK_LEFT://����ƽ��
		move_X -= 0.2;
		this->OnDraw(this->GetDC());//�ػ����
		break;
	case VK_RIGHT://����ƽ��
		move_X += 0.2;
		this->OnDraw(this->GetDC());//�ػ����
		break;
	case VK_PRIOR://pageup���Ŵ�
		scale += 0.2;
		this->OnDraw(this->GetDC());//�ػ����
		break;
	case VK_NEXT://pagedown����С
		scale -= 0.2;
		this->OnDraw(this->GetDC());//�ػ����
		break;
	default:
		break;
	}
	CGLEnabledView::OnKeyDown(nChar, nRepCnt, nFlags);
}

//ASCII�ַ�����
void CPly2PrintView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nChar)
	{
	case _T('f'):
	case _T('F'):
		showFace = !showFace;
		this->OnDraw(this->GetDC());
		break;
	case _T('d'):
	case _T('D'):
		showView = !showView;
		this->OnDraw(this->GetDC());
		break;
	default:
	break;
	}
	CGLEnabledView::OnChar(nChar, nRepCnt, nFlags);
}

//��ʾ����˵��
void CPly2PrintView::OnStatement()
{
	// TODO:  �ڴ���������������
	ShellExecute(NULL, _T("open"), _T("Readme.txt"), NULL, NULL, SW_SHOWNORMAL);
}
