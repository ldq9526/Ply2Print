#pragma once
#include "afxwin.h"
#include<GL/glut.h>


class CGLEnabledView :
	public CView
{
public:
	CGLEnabledView();
	~CGLEnabledView();
private:
	CDC* m_pDC;									// Windows�豸������
	HGLRC m_hRC;                                // OpenGL��Ⱦ������
	CRect m_ClientRect;							// �ͻ����Ĵ�С
	double m_dAspectRatio;						// ���ڵı���
	HPALETTE    m_hPalette;						//��ɫ��

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnDrawGL(CDC * pDC);
	virtual void drawMyText();
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	BOOL SetupPixelFormat();
	BOOL InitializeOpenGL(CDC* pDC);
	void SetLogicalPalette();
};

