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
	CDC* m_pDC;									// Windows设备描述表
	HGLRC m_hRC;                                // OpenGL渲染描述表
	CRect m_ClientRect;							// 客户区的大小
	double m_dAspectRatio;						// 窗口的比例
	HPALETTE    m_hPalette;						//调色板

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

