
// Ply2PrintView.h : CPly2PrintView ��Ľӿ�
//

#pragma once

#include"Model.h"
#include "GLEnabledView.h"
#include"SurfaceDialog.h"
class CPly2PrintView : public CGLEnabledView
{
private:
	//��Ҫ���ƵĶ���
	Model * sourceModel, * backupModel;//Դģ���뱸��ģ��
	SurfaceDialog * surfaceDialog;//�����ĶԻ���
	set<int> selectedVertex;//��¼��ʰȡ�ĵ������
	set<int> selectedFacet;//��¼ʰȡ�����������
	CString myText;//������ĵ����Ͻǵ���ʾ��Ϣ

	//ģ�ͻ���ģʽ
	int style;//0���Ƶ�ͼ,1��������ͼ,2����Ĭ�ϵ���Ƭͼ
	bool cull;//����������
	bool zbuffer;//�Ƿ�����zbuffer
	bool show_view;//�Ƿ���ʾ
	bool PNormal;//��������

	//ģ�Ϳ�����Ϣ
	GLdouble scale = -20.0;//����
	int color_type;//�ƹ�����,��ɫ���ɫ�ƹ�
	GLdouble angle_X = 0.0, angle_Y = 0.0;//��xy����ת�ĽǶ�,���ۻ�ЧӦ
	GLdouble move_X = 0.0, move_Y = 0.0;//��¼ˮƽ�ƶ��ľ���

	//������ʾ����
	bool smoothed;//�Ƿ�ƽ����
	bool select;//�Ƿ���ʰȡģʽ
	bool leftbuttondown;//�������Ƿ���
	bool showFace;//�Ƿ���ʾ��ʰȡ����
	bool showView;//�Ƿ���ʾ�ϴε�����

	// ���λ����Ϣ
	int oldmy = -1, oldmx = -1;	//��¼�������Ļ��������

	//�����������׵���ƽ��Զƽ��Ľ���,�Լ���ģ�͵�����Ľ���
	GLdouble near_point[3];//���ƽ�潻��
	GLdouble far_point[3];//��Զƽ�潻��
	GLdouble CrossPoint[3];//��ģ�͵����ӵ�����Ľ���

private:
	//˽�е��ڲ�����
	int isInFacet(double x, double y, double z);//�Ƿ���ĳ����Ƭ�ཻ
	int getClosest(int index);//��ȡ���㵽��Ƭindex������������
	void selectVertex(int x, int y);//ʰȡ����
	void drawMyText();//�����ʾ����
	bool DrawModel(bool show, bool cull, int style, bool normalpoint, bool zbuffer);//����ģ��
	void OnDrawGL(CDC *pDC);//���ļ��ϻ�ͼ
	void InitalLigt();//��ʼ���ƹ�
	void initDrawMode();//��ʼ��ģ�ͻ�����Ϣ
	void initControl();//��ʼ����ʾ������Ϣ
	void Smooth(double dt);//�Լ�ʵ�ֵ�ƽ�����㺯��

protected: // �������л�����
	CPly2PrintView();//���캯��
	DECLARE_DYNCREATE(CPly2PrintView)

// ����
public:
	CPly2PrintDoc* GetDocument() const;

//����
public:
	//ģ���������
	static const int M_POINT = 0;//���Ƶ�ͼģʽ
	static const int M_LINE = 1;//��������ͼģʽ
	static const int M_FILL = 2;//������Ƭ���ͼģʽ

	//���շ���������
	static const bool M_VERTEXNORMAL = false;//�㷨��ģʽ����
	static const bool M_FACETNORMAL = true;//�淨��ģʽ����

	//����ɫ������
	static const int WHITE_LIGHT = 0;//��ɫ�ƹ�
	static const int COLOR_LIGHT = 1;//��ɫ�ƹ�
	
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC* pDC);//�ػ溯��

// ʵ��
public:
	virtual ~CPly2PrintView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public://�˵��Լ��¼���Ϣӳ��
	afx_msg void OnFileOpen();//���ļ�
	afx_msg void OnVertexNormal();//�㷨�����
	afx_msg void OnFacetNormal();//Ĭ���淨�����
	afx_msg void OnFill();//Ĭ�Ϸ�ʽ����
	afx_msg void OnLine();//�������
	afx_msg void OnPoint();//�����
	afx_msg void OnWhiteLight();//��ɫ��Դ
	afx_msg void OnColorLight();//��ɫ��Դ
	afx_msg void OnPrintModelInfo();//��ʾģ����Ϣ
	afx_msg void OnFileSave();//�����ļ�
	afx_msg void OnEditSet();//����ƽ������
	afx_msg void OnRecover();//�ָ�ԭ��ģ��
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);//ʰȡ״̬�µ�������
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);//ʰȡ״̬�µ�����Ҽ�
	afx_msg void OnSelect();//����ʰȡģʽ
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnStatement();
};

#ifndef _DEBUG  // Ply2PrintView.cpp �еĵ��԰汾
inline CPly2PrintDoc* CPly2PrintView::GetDocument() const
   { return reinterpret_cast<CPly2PrintDoc*>(m_pDocument); }
#endif

