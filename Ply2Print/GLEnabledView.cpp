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

	// ��ʼ��OpenGL

	InitializeOpenGL(m_pDC);

	// �ͷ�OpenGL����������

	wglMakeCurrent(NULL, NULL);

	return 0;
}


void CGLEnabledView::OnDestroy()
{
	// ����豸��������OpenGL����������
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	// �ͷŻ���������
	if (m_hRC != NULL) ::wglDeleteContext(m_hRC);

	if (m_hPalette)

		DeleteObject(m_hPalette);//�ͷŵ�ɫ��

	// �ͷ�Windows�豸������
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
		// ���¿ͻ��������㴰�ڵı���
		m_ClientRect.right = cx;
		m_ClientRect.bottom = cy;
		m_dAspectRatio = double(cx) / double(cy);
		// ��ȡOpenGL����������
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

		// �����ӿڱ任
		glViewport(0, 0, cx, cy);
		// ����͸�ӱ任
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(40.0, m_dAspectRatio, 0.1f, 100.0f);	//����ȵ���
		glTranslatef(0.0f, 0.0f, -4.0f);
		glMatrixMode(GL_MODELVIEW);

		glPopMatrix();
		// �ͷ�OpenGL����������

		wglMakeCurrent(NULL, NULL);
		// ���´�������

		Invalidate(TRUE);

	};
}

void CGLEnabledView::drawMyText(){

}

void CGLEnabledView::OnDrawGL(CDC *pDC)
{
	// ��������������

	glBegin(GL_LINES);
	// ���ƺ�ɫ��x��
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.9f, 0.1f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.9f, -0.1f, 0.0f);

	// ������ɫ��y��
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.1f, 0.9f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.1f, 0.9f, 0.0f);

	// ������ɫ��z��

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
	// TODO:  �ڴ�����ר�ô����/����û���
	CDocument* pDoc = GetDocument();

	// TODO: add draw code here

	static BOOL   bBusy = FALSE;				// ׼��һ����־�ź�
	if (bBusy)									//���æ���򷵻�
		return;
	bBusy = TRUE;								//�ñ�־Ϊæ�����濪ʼ��������
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC); // ��ȡ�豸������

	// �����ɫ����������Ȼ�����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	OnDrawGL(pDC);								// ���ó������ƺ���

	glFinish();									// ���ͼ�εĻ���
	SwapBuffers(m_pDC->GetSafeHdc());			// ����˫����
	bBusy = FALSE;								// ������ϣ��ñ�־Ϊ����
	wglMakeCurrent(NULL, NULL);					// �ͷ�OpenGL����������
	//
	drawMyText();	//��ʾ�Զ��������
}


BOOL CGLEnabledView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ�����ר�ô����/����û���
	////////////////////////////////////////////////////////////////////////////

	//���ô��ڷ��

	//OpenGLҪ��

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	//���ĵ�Ӧ�ó���Ҫ��

	cs.lpszClass = AfxRegisterWndClass(CS_OWNDC | CS_HREDRAW | CS_VREDRAW);

	/////////////////////////////////////////////////////////////////////////////      
	return CView::PreCreateWindow(cs);
}


BOOL CGLEnabledView::SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd = {

		sizeof(PIXELFORMATDESCRIPTOR),    // pfd�ṹ�Ĵ�С
		1,                                // �汾��
		PFD_DRAW_TO_WINDOW |              // ֧���ڴ����л�ͼ
		PFD_SUPPORT_OPENGL |              // ֧�� OpenGL
		PFD_DOUBLEBUFFER,                 // ˫����ģʽ
		PFD_TYPE_RGBA,                    // RGBA ��ɫģʽ
		24,                               // 24 λ��ɫ���
		0, 0, 0, 0, 0, 0,                 // ������ɫλ
		0,                                // û�з�͸���Ȼ���
		0,                                // ������λλ
		0,                                // ���ۼӻ���
		0, 0, 0, 0,                       // �����ۼ�λ
		32,                               // 32 λ��Ȼ���    
		0,                                // ��ģ�建��
		0,                                // �޸�������
		PFD_MAIN_PLANE,                   // ����
		0,                                // ����
		0, 0, 0                           // ���Բ�,�ɼ��Ժ������ģ
	};

	int pixelformat;

	pixelformat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);	//ѡ�����ظ�ʽ
	::SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd);		//�������ظ�ʽ

	if (pfd.dwFlags & PFD_NEED_PALETTE)

		SetLogicalPalette();										//�����߼���ɫ��

	return TRUE;
}


BOOL CGLEnabledView::InitializeOpenGL(CDC* pDC)
{
	m_pDC = pDC;
	SetupPixelFormat();

	//���ɻ���������
	m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc());

	//�õ�ǰ����������
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