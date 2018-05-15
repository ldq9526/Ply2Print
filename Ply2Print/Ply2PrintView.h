
// Ply2PrintView.h : CPly2PrintView 类的接口
//

#pragma once

#include"Model.h"
#include "GLEnabledView.h"
#include"SurfaceDialog.h"
class CPly2PrintView : public CGLEnabledView
{
private:
	//需要绘制的对象
	Model * sourceModel, * backupModel;//源模型与备份模型
	SurfaceDialog * surfaceDialog;//关联的对话框
	set<int> selectedVertex;//记录被拾取的点的索引
	set<int> selectedFacet;//记录拾取到的面的索引
	CString myText;//输出在文档左上角的提示信息

	//模型绘制模式
	int style;//0绘制点图,1绘制线条图,2绘制默认的面片图
	bool cull;//隐藏面消除
	bool zbuffer;//是否启用zbuffer
	bool show_view;//是否显示
	bool PNormal;//法向类型

	//模型控制信息
	GLdouble scale = -20.0;//缩放
	int color_type;//灯光类型,彩色与白色灯光
	GLdouble angle_X = 0.0, angle_Y = 0.0;//绕xy轴旋转的角度,有累积效应
	GLdouble move_X = 0.0, move_Y = 0.0;//记录水平移动的距离

	//特殊显示控制
	bool smoothed;//是否平滑过
	bool select;//是否是拾取模式
	bool leftbuttondown;//鼠标左键是否按下
	bool showFace;//是否显示被拾取的面
	bool showView;//是否显示上次的视线

	// 鼠标位置信息
	int oldmy = -1, oldmx = -1;	//记录鼠标在屏幕区的坐标

	//鼠标射线与视椎体近平面远平面的交点,以及和模型的最近的交点
	GLdouble near_point[3];//与近平面交点
	GLdouble far_point[3];//与远平面交点
	GLdouble CrossPoint[3];//与模型的离视点最近的交点

private:
	//私有的内部函数
	int isInFacet(double x, double y, double z);//是否与某个面片相交
	int getClosest(int index);//获取交点到面片index最近顶点的索引
	void selectVertex(int x, int y);//拾取顶点
	void drawMyText();//输出提示文字
	bool DrawModel(bool show, bool cull, int style, bool normalpoint, bool zbuffer);//绘制模型
	void OnDrawGL(CDC *pDC);//在文件上画图
	void InitalLigt();//初始化灯光
	void initDrawMode();//初始化模型绘制信息
	void initControl();//初始化显示控制信息
	void Smooth(double dt);//自己实现的平滑计算函数

protected: // 仅从序列化创建
	CPly2PrintView();//构造函数
	DECLARE_DYNCREATE(CPly2PrintView)

// 特性
public:
	CPly2PrintDoc* GetDocument() const;

//属性
public:
	//模型填充属性
	static const int M_POINT = 0;//绘制点图模式
	static const int M_LINE = 1;//绘制线条图模式
	static const int M_FILL = 2;//绘制面片填充图模式

	//光照法向量属性
	static const bool M_VERTEXNORMAL = false;//点法向模式绘制
	static const bool M_FACETNORMAL = true;//面法向模式绘制

	//光照色彩属性
	static const int WHITE_LIGHT = 0;//白色灯光
	static const int COLOR_LIGHT = 1;//彩色灯光
	
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC* pDC);//重绘函数

// 实现
public:
	virtual ~CPly2PrintView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public://菜单以及事件消息映射
	afx_msg void OnFileOpen();//打开文件
	afx_msg void OnVertexNormal();//点法向绘制
	afx_msg void OnFacetNormal();//默认面法向绘制
	afx_msg void OnFill();//默认方式绘制
	afx_msg void OnLine();//线条填充
	afx_msg void OnPoint();//点绘制
	afx_msg void OnWhiteLight();//白色光源
	afx_msg void OnColorLight();//彩色光源
	afx_msg void OnPrintModelInfo();//显示模型信息
	afx_msg void OnFileSave();//保存文件
	afx_msg void OnEditSet();//设置平滑参数
	afx_msg void OnRecover();//恢复原先模型
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);//拾取状态下的鼠标左键
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);//拾取状态下的鼠标右键
	afx_msg void OnSelect();//进入拾取模式
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnStatement();
};

#ifndef _DEBUG  // Ply2PrintView.cpp 中的调试版本
inline CPly2PrintDoc* CPly2PrintView::GetDocument() const
   { return reinterpret_cast<CPly2PrintDoc*>(m_pDocument); }
#endif

