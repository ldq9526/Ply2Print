#include "stdafx.h"
#include "GLEnabledView.h"


CGLEnabledView::CGLEnabledView()
{
}


CGLEnabledView::~CGLEnabledView()
{
}
BEGIN_MESSAGE_MAP(CGLEnabledView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


int CGLEnabledView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)

		return -1;

	// TODO: Add your specialized creation code here

	m_pDC = new CClientDC(this);

	// 初始化OpenGL

	InitializeOpenGL(m_pDC);

	// 释放OpenGL绘制描述表

	wglMakeCurrent(NULL, NULL);

	return 0;
}


void CGLEnabledView::OnDestroy()
{
	// 获得设备描述表的OpenGL绘制描述表
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	// 释放绘制描述表
	if (m_hRC != NULL) ::wglDeleteContext(m_hRC);

	if (m_hPalette)

		DeleteObject(m_hPalette);//释放调色板

	// 释放Windows设备描述表
	if (m_pDC) delete m_pDC;

	CView::OnDestroy();

	// TODO: Add your message handler code here
}


void CGLEnabledView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if (0 < cx && 0 < cy)
	{
		// 更新客户区，计算窗口的比例
		m_ClientRect.right = cx;
		m_ClientRect.bottom = cy;
		m_dAspectRatio = double(cx) / double(cy);
		// 获取OpenGL绘制描述表
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

		// 设置视口变换
		glViewport(0, 0, cx, cy);
		// 设置透视变换
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(40.0, m_dAspectRatio, 0.1f, 100.0f);	//把深度调大
		glTranslatef(0.0f, 0.0f, -4.0f);
		glMatrixMode(GL_MODELVIEW);

		glPopMatrix();
		// 释放OpenGL绘制描述表

		wglMakeCurrent(NULL, NULL);
		// 更新窗口内容

		Invalidate(TRUE);

	};
}

void CGLEnabledView::drawMyText(){

}

void CGLEnabledView::OnDrawGL(CDC *pDC)
{
	// 绘制三个坐标轴

	glBegin(GL_LINES);
	// 绘制红色的x轴
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.9f, 0.1f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.9f, -0.1f, 0.0f);

	// 绘制蓝色的y轴
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.1f, 0.9f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.1f, 0.9f, 0.0f);

	// 绘制绿色的z轴

	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.1f, 0.9f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, -0.1f, 0.9f);

	glEnd();
}


void CGLEnabledView::OnDraw(CDC* pDC)
{
	// TODO:  在此添加专用代码和/或调用基类
	CDocument* pDoc = GetDocument();

	// TODO: add draw code here

	static BOOL   bBusy = FALSE;				// 准备一个标志信号
	if (bBusy)									//如果忙，则返回
		return;
	bBusy = TRUE;								//置标志为忙，下面开始场景绘制
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC); // 获取设备描述表

	// 清除颜色缓冲区和深度缓冲区
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	OnDrawGL(pDC);								// 调用场景绘制函数

	glFinish();									// 完成图形的绘制
	SwapBuffers(m_pDC->GetSafeHdc());			// 启用双缓冲
	bBusy = FALSE;								// 绘制完毕，置标志为空闲
	wglMakeCurrent(NULL, NULL);					// 释放OpenGL绘制描述表
	//
	drawMyText();	//显示自定义的文字
}


BOOL CGLEnabledView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此添加专用代码和/或调用基类
	////////////////////////////////////////////////////////////////////////////

	//设置窗口风格

	//OpenGL要求

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	//多文档应用程序要求

	cs.lpszClass = AfxRegisterWndClass(CS_OWNDC | CS_HREDRAW | CS_VREDRAW);

	/////////////////////////////////////////////////////////////////////////////      
	return CView::PreCreateWindow(cs);
}


BOOL CGLEnabledView::SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd = {

		sizeof(PIXELFORMATDESCRIPTOR),    // pfd结构的大小
		1,                                // 版本号
		PFD_DRAW_TO_WINDOW |              // 支持在窗口中绘图
		PFD_SUPPORT_OPENGL |              // 支持 OpenGL
		PFD_DOUBLEBUFFER,                 // 双缓存模式
		PFD_TYPE_RGBA,                    // RGBA 颜色模式
		24,                               // 24 位颜色深度
		0, 0, 0, 0, 0, 0,                 // 忽略颜色位
		0,                                // 没有非透明度缓存
		0,                                // 忽略移位位
		0,                                // 无累加缓存
		0, 0, 0, 0,                       // 忽略累加位
		32,                               // 32 位深度缓存    
		0,                                // 无模板缓存
		0,                                // 无辅助缓存
		PFD_MAIN_PLANE,                   // 主层
		0,                                // 保留
		0, 0, 0                           // 忽略层,可见性和损毁掩模
	};

	int pixelformat;

	pixelformat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);	//选择像素格式
	::SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd);		//设置像素格式

	if (pfd.dwFlags & PFD_NEED_PALETTE)

		SetLogicalPalette();										//设置逻辑调色板

	return TRUE;
}


BOOL CGLEnabledView::InitializeOpenGL(CDC* pDC)
{
	m_pDC = pDC;
	SetupPixelFormat();

	//生成绘制描述表
	m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc());

	//置当前绘制描述表
	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	return TRUE;
}


void CGLEnabledView::SetLogicalPalette()
{
	struct
	{
		WORD Version;
		WORD NumberOfEntries;
		PALETTEENTRY aEntries[256];
	} logicalPalette = { 0x300, 256 };

	BYTE reds[] = { 0, 36, 72, 109, 145, 182, 218, 255 };
	BYTE greens[] = { 0, 36, 72, 109, 145, 182, 218, 255 };
	BYTE blues[] = { 0, 85, 170, 255 };

	for (int colorNum = 0; colorNum<256; ++colorNum)
	{
		logicalPalette.aEntries[colorNum].peRed =
			reds[colorNum & 0x07];

		logicalPalette.aEntries[colorNum].peGreen =
			greens[(colorNum >> 0x03) & 0x07];

		logicalPalette.aEntries[colorNum].peBlue =
			blues[(colorNum >> 0x06) & 0x03];

		logicalPalette.aEntries[colorNum].peFlags = 0;
	}
	m_hPalette = CreatePalette((LOGPALETTE*)&logicalPalette);
}