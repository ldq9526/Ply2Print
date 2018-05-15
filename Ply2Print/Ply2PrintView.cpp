
// Ply2PrintView.cpp : CPly2PrintView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CPly2PrintView::OnFileOpen)//打开文件
	ON_COMMAND(ID_VIEW_SMOOTH, &CPly2PrintView::OnVertexNormal)//点法向
	ON_COMMAND(ID_VIEW_WIREFRAME, &CPly2PrintView::OnFacetNormal)//面法向
	ON_COMMAND(ID_32772, &CPly2PrintView::OnFill)//面片填充方式绘制
	ON_COMMAND(ID_32773, &CPly2PrintView::OnLine)//线条填充方式绘制
	ON_COMMAND(ID_32774, &CPly2PrintView::OnPoint)//点方式绘制
	ON_COMMAND(ID_32776, &CPly2PrintView::OnWhiteLight)//白色灯光
	ON_COMMAND(ID_32777, &CPly2PrintView::OnColorLight)//彩色灯光
	ON_COMMAND(ID_32788, &CPly2PrintView::OnPrintModelInfo)//显示模型信息
	ON_COMMAND(ID_32789, &CPly2PrintView::OnRecover)//恢复模型
	ON_COMMAND(ID_FILE_SAVE, &CPly2PrintView::OnFileSave)//保存文件
	ON_COMMAND(ID_EDIT_SET, &CPly2PrintView::OnEditSet)//参数设置
	ON_COMMAND(ID_32781, &CPly2PrintView::OnSelect)//进入拾取模式
	ON_COMMAND(ID_32791, &CPly2PrintView::OnStatement)//操作说明
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CPly2PrintView 构造/析构

CPly2PrintView::CPly2PrintView()
{
	// TODO:  在此处添加构造代码
	//模型对象初始化
	sourceModel = nullptr;//源数据模型设置为空
	backupModel = nullptr;//备份数据模型设置为空
	selectedVertex.clear();//拾取顶点清空
	selectedFacet.clear();//拾取面清空

	//模型绘制模式初始化
	initDrawMode();
	
	//特殊显示控制初始化
	initControl();
	
	//观察点初始化
	near_point[0] = near_point[1] = near_point[2] = 0.0;
	far_point[0] = far_point[1] = far_point[2] = 0.0;
	CrossPoint[0] = CrossPoint[1] = CrossPoint[2] = 0.0;
}

CPly2PrintView::~CPly2PrintView()
{
	if (sourceModel != nullptr)
	{
		sourceModel->ClearModel();//清空模型
		delete sourceModel;
	}
	if (backupModel != nullptr)//清空备份模型
	{
		backupModel->ClearModel();
		delete backupModel;
	}
}

void CPly2PrintView::OnDraw(CDC* pDC)//重绘函数
{
	CGLEnabledView::OnDraw(pDC);
	this->OnDrawGL(pDC);
}

// CPly2PrintView 打印

BOOL CPly2PrintView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPly2PrintView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void CPly2PrintView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}


// CPly2PrintView 诊断

#ifdef _DEBUG
void CPly2PrintView::AssertValid() const
{
	CView::AssertValid();
}

void CPly2PrintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPly2PrintDoc* CPly2PrintView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPly2PrintDoc)));
	return (CPly2PrintDoc*)m_pDocument;
}
#endif //_DEBUG

//内部私有函数

//光照初始化
void CPly2PrintView::InitalLigt()
{
	//设置光源
	GLfloat light_position1[4] = { -52, -16, -50, 0 };
	GLfloat light_position2[4] = { -26, -48, -50, 0 };
	GLfloat light_position3[4] = { 16, -52, -50, 0 };
	GLfloat light_position4[4] = { 52, 16, 50, 0 };
	GLfloat light_position5[4] = { 26, 48, 50, 0 };
	GLfloat light_position6[4] = { -16, 52, 50, 0 };
	//设置光照方向
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

	if (color_type == COLOR_LIGHT)//彩色灯光
	{
		color1[0] = 1.0f; color1[1] = 0.0f; color1[2] = 0.0f; color1[3] = 1.0f;
		color2[0] = 0.0f; color2[1] = 1.0f; color2[2] = 0.0f; color2[3] = 1.0f;
		color3[0] = 0.0f; color3[1] = 0.0f; color3[2] = 1.0f; color3[3] = 1.0f;
		color4[0] = 1.0f; color4[1] = 0.0f; color4[2] = 0.0f; color4[3] = 1.0f;
		color5[0] = 0.0f; color5[1] = 1.0f; color5[2] = 0.0f; color5[3] = 1.0f;
		color6[0] = 0.0f; color6[1] = 0.0f; color6[2] = 1.0f; color6[3] = 1.0f;

		GLfloat ambient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };//环境光

		//材质属性
		GLfloat material_color[4] = { 1.0f, 1.0f, 1.0f, 0.5f };//白色漫反射
		GLfloat material_specular[4] = { 0.5f, 0.5f, 0.5f, 0.5f };//少量镜面反射白光
		GLfloat material_ambient[4] = { 0.0f, 0.0f, 0.0f, 0.0f };//无环境光

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
		//白色灯光
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

//初始化模型绘制信息
void CPly2PrintView::initDrawMode()
{
	style = M_FILL;//默认面片绘制		
	cull = true;//cull face
	zbuffer = true;//启用z缓存
	show_view = true;//默认显示图形
	PNormal = M_FACETNORMAL;//默认为面法向类型
	color_type = WHITE_LIGHT;//默认为白光
}

//初始化显示控制信息
void CPly2PrintView::initControl()
{
	smoothed = false;//默认还没有经历过平滑操作
	select = false;//默认不是拾取模式
	leftbuttondown = false;//默认鼠标没有按下
	showView = false; //默认不显示视线
	showFace = false;//不显示被拾取的面
}

//是否与某个面片相交
int CPly2PrintView::isInFacet(double x, double y, double z)
{
	Vertex * vertex = sourceModel->GetVertex();//获取顶点数组
	Facet * facet = sourceModel->GetFacet();//获取面片数组
	Normal * normal = sourceModel->GetFacetNormal();//获取面片的法向量
	int facetN = sourceModel->GetFacetN();//获取面片数量
	int index = -1;//记录最小的面片的索引,初始化为-1
	double distance = DBL_MAX;//记录交点到近视点的距离,初始化为无穷大

	for (int i = 0; i < facetN; i++)
	{
		double v[3];//记录三角形面片的法向量
		for (int j = 0; j < 3; j++)
			v[j] = normal[i][j];

		double A[3], B[3], C[3];//面片三个顶点的坐标
		for (int j = 0; j < 3; j++)
			A[j] = vertex[facet[i][0]][j];
		for (int j = 0; j < 3; j++)
			B[j] = vertex[facet[i][1]][j];
		for (int j = 0; j < 3; j++)
			C[j] = vertex[facet[i][2]][j];

		double D;//平面方程Ax + By + Cz + D = 0的常数项,其中v = (A,B,C)为法向量
		D = -dot(v, A);//根据平面上某一点与法向量计算常数项 D = -(A * x0 + B * y0 + C * z0)

		double tempU, tempD;//临时变量
		tempU = dot(v, near_point) + D;
		tempD = v[0] * x + v[1] * y + v[2] * z;
		if (fabs(tempD - 0.0) < 1e-5)
			continue;//直线与平面平行或在平面上
		else
		{
			double t = -tempU / tempD;//计算直线参数方程的参数

			double P[3];//记录临时交点
			//计算交点坐标
			P[0] = x * t + near_point[0];
			P[1] = y * t + near_point[1];
			P[2] = z * t + near_point[2];

			if (inTriangle(P, A, B, C))//判断点是否在三角形内部
			{
				double tmpdis = dist(near_point, P);//计算近视点到交点距离
				if (tmpdis < distance)
				{
					CrossPoint[0] = P[0];//保存交点的坐标
					CrossPoint[1] = P[1];
					CrossPoint[2] = P[2];
					distance = tmpdis;
					index = i;
				}
			}
		}
	}
	return index;//返回距离近视点最近的面的索引
}

//获取交点到面片index最近顶点的索引
int CPly2PrintView::getClosest(int index)
{
	Vertex * vertex = sourceModel->GetVertex();//获取顶点数组
	Facet * facet = sourceModel->GetFacet();//获取面片数组
	double l = dist(CrossPoint, vertex[facet[index][0]]);
	int n = facet[index][0];//n记录面片上顶点的索引
	for (int i = 1; i < 3; i++)
	{
		double L = dist(CrossPoint, vertex[facet[index][i]]);
		if (L < l)//如果找到距离更小的点则替换标准
		{
			l = L;
			n = facet[index][i];
		}
	}
	return n;//返回距离最近的顶点的索引
}

//拾取顶点
void CPly2PrintView::selectVertex(int x, int y)
{
	GLdouble    modelview[16];
	GLdouble    projection[16];
	GLint       viewport[4];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	glPushMatrix();//保存当前模式再做一次变换

	glLoadIdentity();
	glTranslated(move_X, move_Y, -25.0 + scale); //平移以能够观察到对象  
	glRotated(angle_X, 1.0, 0.0, 0.0);//绕x轴旋转
	glRotated(angle_Y, 0.0, 1.0, 0.0);//绕y轴旋转
	glRotated(0.0, 0.0, 0.0, 1.0);//不绕z轴旋转

	glPopMatrix();

	// 获取近裁剪面上的交点存入near_point[]数组
	gluUnProject((GLdouble)x, (GLdouble)(viewport[3] - y), 0.0,
		modelview, projection, viewport,
		near_point, near_point + 1, near_point + 2);

	// 获取远裁剪面上的交点存入far_point[]数组
	gluUnProject((GLdouble)x, (GLdouble)(viewport[3] - y), 1.0,
		modelview, projection, viewport,
		far_point, far_point + 1, far_point + 2);

	//获取远近裁剪面交点的向量v[]
	double v[3];
	vec(v, far_point, near_point);//由远指向近
	int facetIndex = isInFacet(v[0], v[1], v[2]);//获取面相交的交点
	if (facetIndex != -1)
	{
		int vertexIndex = getClosest(facetIndex);//获取距离最近的点索引
		selectedVertex.insert(vertexIndex);//加入选点集合
		selectedFacet.insert(facetIndex);//加入面的集合
		GLdouble * point = sourceModel->GetVertex(vertexIndex);
		myText.Format(_T("选到点%d:(%.2f,%.2f,%.2f)"), vertexIndex, point[0], point[1], point[2]);
		select = false;//结束拾取过程
	}
	else
		myText = _T("未选中任何点");
}

//在文档左上角显示信息文字
void CPly2PrintView::drawMyText()
{
	CDC *pDC = GetWindowDC();//获取设备上下文环境
	pDC->DrawText(myText, CRect(6, 6, 160, 420), DT_WORDBREAK); ReleaseDC(pDC);
}

//绘制源模型以及辅助模型
bool CPly2PrintView::DrawModel(bool show, bool cull, int style, bool normalpoint, bool zbuffer)
{
	int vertexN = sourceModel->GetVertexN();//获取顶点数
	Vertex * vertex = sourceModel->GetVertex();//获取顶点数组
	int facetN = sourceModel->GetFacetN();//获取面片数
	Facet * facet = sourceModel->GetFacet();//获取面片数组
	Normal * faceNormal = sourceModel->GetFacetNormal();//获取面法向量
	Normal * vertexNormal = sourceModel->GetVertexNormal();//获取定点法向量

	if (!show)
		return true;//不要求显示则直接退出完成绘制
	if (select && leftbuttondown)
		selectVertex(oldmx, oldmy);//如果
	if (style == M_POINT)	//绘制点图
	{
		glPointSize(1.0);
		glColor3d(0.0, 0.0, 0.0);
		glBegin(GL_POINTS);
		for (int i = 0; i < vertexN; i++)
			glVertex3dv(vertex[i]);
		glEnd();
		glColor3d(1.0, 1.0, 1.0);//恢复成无色
		return true;
	}

	if (cull)//要求消除隐藏面
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	if (zbuffer)//启用zbuffer模式
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	if (style == M_FILL)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//面片填充式
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//线条填充式

	for (int i = 0; i < facetN; i++)
	{
		if (style == M_LINE)
		{
			glLineWidth(0.1f);//线条宽度1.0像素
			glColor3d(0.0, 0.0, 0.0);//黑色绘制线条
		}
		if (normalpoint == M_FACETNORMAL)	//按照面法向画三角面片
		{
			glNormal3dv(faceNormal[i]);
			glBegin(GL_TRIANGLES);
			glVertex3dv(vertex[facet[i][0]]);
			glVertex3dv(vertex[facet[i][1]]);
			glVertex3dv(vertex[facet[i][2]]);
			glEnd();
		}
		else				//点法向画三角面片
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
		glColor3d(1.0, 0.0, 0.0);//红色描点
		glPointSize(5.0);//点大小为5像素
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
		glColor3d(0.0, 1.0, 0.0);//绿色射线
		glLineWidth(5.0);//五个像素点
		glBegin(GL_LINES);
		glVertex3dv(near_point);
		glVertex3dv(far_point);
		glEnd();
	}
	glColor3d(1.0, 1.0, 1.0);//恢复成无色
	return true;
}

//初始化光照等条件并绘制模型
void CPly2PrintView::OnDrawGL(CDC *pDC)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//背景色白色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	InitalLigt();		///初始化光照信息
	glPushMatrix();

	glTranslated(move_X, move_Y, scale);//平移
	glRotated(angle_Y, 0.0, 1.0, 0.0);
	glRotated(angle_X, 1.0, 0.0, 0.0);

	if (sourceModel != nullptr)	//如果打开了模型文件
	{
		cull = false;	//允许观察内部
		DrawModel(show_view, cull, style, PNormal, zbuffer);
	}
	else{
		myText = _T("请打开一个ply2文件");
	}

	glPopMatrix();
}

//自己实现的平滑计算函数
void CPly2PrintView::Smooth(double dt)
{
	int vertexN = sourceModel->GetVertexN();//获取顶点个数
	Vertex * vertex = sourceModel->GetVertex();//获取顶点数组
	bool * isBound = sourceModel->GetBound();//获取边界点信息
	set<int> * degree = sourceModel->GetVertexDegree();//获取顶点邻接表

	for (int i = 0; i < vertexN; i++)
	{
		if (isBound[i] || degree[i].size() == 0)
			continue;//如果是边界点,则坐标不变
		else
		{
			double L[3] = { 0.0, 0.0, 0.0 };//修正向量
			double sub[3];//记录差向量
			for (set<int>::iterator it = degree[i].begin(); it != degree[i].end(); it++)
			{
				vec(sub, vertex[*it], vertex[i]);
				add(L, L, sub);
			}
			double k = 1.0 / degree[i].size();
			times(L, k, L);//修正向量
			if (dt < 1.0)
				times(L, dt, L);
			else
				times(L, 0.1, L);
			add(vertex[i], vertex[i], L);//修正
		}
	}
}

// CPly2PrintView 消息处理程序

//打开文件响应函数
void CPly2PrintView::OnFileOpen()
{
	// TODO:  在此添加命令处理程序代码
	CString filter = _T("PLY2 File (*.PLY2)|*.ply2||");
	CFileDialog fileDlg(TRUE, NULL, NULL, NULL, filter, NULL);
	fileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;
	fileDlg.m_ofn.lpstrTitle = _T("打开ply2文件");//文件选择器，只选择ply2文件

	if (fileDlg.DoModal() == IDOK)//如果点击了确定
	{
		//模型绘制模式初始化
		initDrawMode();

		//显示控制模式初始化
		initControl();
		
		//模型对象清空
		selectedVertex.clear();//拾取顶点清空
		selectedFacet.clear();//拾取面清空
		if (backupModel != nullptr)
		{
			backupModel->ClearModel();
			delete backupModel;
			backupModel = nullptr;
		}
		if (sourceModel != nullptr)//源模型不为空则先清空
		{
			sourceModel->ClearModel();
			delete sourceModel;
		}
		sourceModel = new Model();
		bool flag = sourceModel->ReadFile(fileDlg.GetPathName());//读取模型信息
		if (flag)//读取模型成功
		{
			myText.Format(_T("点数:%d \n面片数:%d"), sourceModel->GetVertexN(), sourceModel->GetFacetN());
		}
		this->OnDraw(this->GetDC());//绘制界面
	}
}

//点法向方式绘制
void CPly2PrintView::OnVertexNormal()
{
	// TODO:  在此添加命令处理程序代码
	if (sourceModel != nullptr)
	{
		PNormal = M_VERTEXNORMAL;//点法向方式绘制
		myText = _T("点法向方式");
	}
	else
		myText = _T("请先打开一个ply2文件");
	this->OnDraw(this->GetDC());	//重绘界面
}

//面法向绘制
void CPly2PrintView::OnFacetNormal()
{
	// TODO:  在此添加命令处理程序代码
	if (sourceModel != nullptr)
	{
		PNormal = M_FACETNORMAL;//面法向方式绘制
		myText = _T("面法向方式");
	}
	else
		myText = _T("请先打开一个ply2文件");
	this->OnDraw(this->GetDC());	//重绘界面
}

//填充点模式
void CPly2PrintView::OnPoint()
{
	// TODO:  在此添加命令处理程序代码
	if (sourceModel != nullptr)
	{
		style = M_POINT;//点绘制模式
		myText = _T("点绘制模式");
	}
	else
		myText = _T("请先打开一个ply2文件");
	this->OnDraw(this->GetDC());	//重绘界面
}

//填充线条模式
void CPly2PrintView::OnLine()
{
	// TODO:  在此添加命令处理程序代码
	if (sourceModel != nullptr)
	{
		style = M_LINE;//线条
		myText = _T("线条填充模式");
	}
	else
		myText = _T("请先打开一个ply2文件");
	this->OnDraw(this->GetDC());	//重绘界面
}

//面片填充模式
void CPly2PrintView::OnFill()
{
	// TODO:  在此添加命令处理程序代码
	if (sourceModel != nullptr)
	{
		style = M_FILL;//面片填充模式
		myText = _T("面片填充模式");
	}
	else
		myText = _T("请先打开一个ply2文件");
	this->OnDraw(this->GetDC());	//重绘界面
}

//白色灯光
void CPly2PrintView::OnWhiteLight()
{
	// TODO:  在此添加命令处理程序代码
	myText = _T("白色灯光");
	color_type = WHITE_LIGHT;//白色灯光
	this->OnDraw(this->GetDC());	//重绘界面
}

//彩色灯光
void CPly2PrintView::OnColorLight()
{
	// TODO:  在此添加命令处理程序代码
	myText = _T("彩色灯光");
	color_type = COLOR_LIGHT;
	this->OnDraw(this->GetDC());	//重绘界面
}

//显示模型点面数以及体积
void CPly2PrintView::OnPrintModelInfo()
{
	// TODO:  在此添加命令处理程序代码
	if (sourceModel != nullptr){
		myText.Format(_T("点数:%d \n面片数:%d\n模型体积:%.2f"),
			sourceModel->GetVertexN(),
			sourceModel->GetFacetN(),
			sourceModel->GetVolume());
	}
	this->OnDraw(this->GetDC());
}

//保存文件
void CPly2PrintView::OnFileSave()
{
	// TODO:  在此添加命令处理程序代码
	if (sourceModel == nullptr){
		myText = _T("请先打开一个模型");
		drawMyText();
		this->InvalidateRect(NULL, FALSE);
		return;
	}
	myText = _T("准备保存模型");
	drawMyText();
	this->InvalidateRect(NULL, FALSE);
	CString buffer;
	CString szFilter = _T("Ply2文件(*.ply2)|*.ply2|所有文件(*.*)|*.*||");
	// 构造保存文件对话框    
	CFileDialog fileDlg(FALSE, _T("ply2"), _T("newModel"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString strFilePath;
	CStdioFile file;

	// 显示保存文件对话框    
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“保存”按钮，则将选择的文件路径显示到编辑框里    
		strFilePath = fileDlg.GetPathName();
		file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		int vertexN = sourceModel->GetVertexN();//获取顶点数
		int facetN = sourceModel->GetFacetN();//获取面片数
		Vertex * vertex = sourceModel->GetVertex();//获取顶点数组
		Facet * facet = sourceModel->GetFacet();//获取面片数
		buffer.Format(_T("%d\n"), vertexN);
		file.WriteString(buffer);  //写入点数
		buffer.Format(_T("%d\n"), facetN);
		file.WriteString(buffer);  //写入面片数
		//写入点的坐标
		for (int i = 0; i < vertexN; ++i){
			buffer.Format(_T("%lf %lf %lf\n"), vertex[i][0], vertex[i][1], vertex[i][2]);
			file.WriteString(buffer);
		}
		//写入面片索引
		for (int i = 0; i < facetN; ++i){
			buffer.Format(_T("3 %d %d %d\n"), facet[i][0], facet[i][1], facet[i][2]);
			file.WriteString(buffer);
		}
		file.Close();
		myText = _T("保存模型成功");
		drawMyText();
		this->InvalidateRect(NULL, FALSE);
	}
}

//设置图形平滑的参数
void CPly2PrintView::OnEditSet()
{
	// TODO:  在此添加命令处理程序代码
	surfaceDialog = new SurfaceDialog();//弹出对话框

	if (surfaceDialog->DoModal() == IDOK)//如果点击了确定按钮
	{
		double step = surfaceDialog->m_step_length;//获取步长
		int times = surfaceDialog->m_times;//获取迭代次数
		int hold_v = surfaceDialog->m_check;//获取是否保持体积

		if (sourceModel != nullptr)
		{
			double original_volume;
			if (hold_v == 1)//如果需要保持体积则获取模型本来的体积
				original_volume = sourceModel->GetVolume();
			if (smoothed == false)
			{
				sourceModel->CopyModel(backupModel);//先保存一份备份模型
				smoothed = true;//以后均恢复成最初的模型
			}
			sourceModel->ComputeDegree();//计算每个点邻接的点与面
			while (times--)
			{
				myText.Format(_T("迭代%d / %d"), surfaceDialog->m_times - times, surfaceDialog->m_times);
				drawMyText();
				Smooth(step);//平滑即微调顶点的坐标
			}
			myText = _T("迭代完成");
			drawMyText();
			if (hold_v == 1)
			{
				double changed_volume = sourceModel->GetVolume();
				myText = _T("正在恢复体积");
				drawMyText();
				sourceModel->Scale(pow((original_volume / changed_volume), 1 / 3.0));
				myText = _T("恢复体积完成");
			}
			else
				myText = _T("完成平滑");
		}
		else
			myText = _T("请先打开一个ply2文件");
		this->OnDraw(this->GetDC());	//重绘界面
	}
}

//恢复之前的模型
void CPly2PrintView::OnRecover()
{
	// TODO:  在此添加命令处理程序代码
	if (backupModel == nullptr)
		myText = _T("未进行过平滑操作");
	else
	{
		if (sourceModel != nullptr)
		{
			sourceModel->ClearModel();
			delete sourceModel;
		}
		sourceModel = new Model;
		myText = _T("正在恢复初始模型");
		drawMyText();
		backupModel->CopyModel(sourceModel);
		myText = _T("恢复完成");
	}
	this->OnDraw(this->GetDC());	//重绘界面
}

//监听拾取模式下的鼠标左键
void CPly2PrintView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	leftbuttondown = true;
	oldmx = point.x;//记录下此时的xy值作为旧值
	oldmy = point.y;
	if (select)
		OnDraw(this->GetDC());
}

//监听拾取模式下的鼠标右键
void CPly2PrintView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (select)
	{
		select = false;
		myText = _T("");
		MessageBox(_T("退出拾取模式"));
		drawMyText();
		this->OnDraw(this->GetDC());	//重绘界面
	}
}

//进入拾取模式
void CPly2PrintView::OnSelect()
{
	// TODO:  在此添加命令处理程序代码
	if (style != M_LINE)
		MessageBox(_T("请在线条模式下拾取!"));
	else
	{
		select = true;
		myText = _T("进入拾取模式");
		drawMyText();
	}
}

//监听拾取模式下的鼠标光标移动
void CPly2PrintView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (select)
		SetCursor(LoadCursor(NULL, IDC_HAND));//鼠标变手型
	else
		SetCursor(LoadCursor(NULL,IDC_ARROW));//恢复成箭头型
	if ((nFlags & MK_LBUTTON) == TRUE) //按下了鼠标左键
	{
		int gapX = point.x - oldmx;
		int gapY = point.y - oldmy;
		oldmx = point.x;//保存当前坐标保证能连续旋转
		oldmy = point.y;
		angle_X -= double(gapY);		//设置上下旋转角度
		angle_Y += double(gapX);		//设置水平旋转角度
		this->OnDraw(this->GetDC());
	}
}

//鼠标左键抬起
void CPly2PrintView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	leftbuttondown = false;
	CGLEnabledView::OnLButtonUp(nFlags, point);
}

//鼠标滚轮控制大小
BOOL CPly2PrintView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (zDelta > 0)//向上滚动
		scale += 0.2;//放大
	else
		scale -= 0.2;//缩小
	this->OnDraw(this->GetDC());//重绘界面
	return CView::OnMouseWheel(nFlags, zDelta, pt);
	return CGLEnabledView::OnMouseWheel(nFlags, zDelta, pt);
}

//特殊按键按下
void CPly2PrintView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	switch (nChar)
	{
	case VK_UP://向上平移
		move_Y += 0.2;
		this->OnDraw(this->GetDC());//重绘界面
		break;
	case VK_DOWN://向下平移
		move_Y -= 0.2;
		this->OnDraw(this->GetDC());//重绘界面
		break;
	case VK_LEFT://向左平移
		move_X -= 0.2;
		this->OnDraw(this->GetDC());//重绘界面
		break;
	case VK_RIGHT://向右平移
		move_X += 0.2;
		this->OnDraw(this->GetDC());//重绘界面
		break;
	case VK_PRIOR://pageup键放大
		scale += 0.2;
		this->OnDraw(this->GetDC());//重绘界面
		break;
	case VK_NEXT://pagedown键缩小
		scale -= 0.2;
		this->OnDraw(this->GetDC());//重绘界面
		break;
	default:
		break;
	}
	CGLEnabledView::OnKeyDown(nChar, nRepCnt, nFlags);
}

//ASCII字符按下
void CPly2PrintView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
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

//显示操作说明
void CPly2PrintView::OnStatement()
{
	// TODO:  在此添加命令处理程序代码
	ShellExecute(NULL, _T("open"), _T("Readme.txt"), NULL, NULL, SW_SHOWNORMAL);
}
