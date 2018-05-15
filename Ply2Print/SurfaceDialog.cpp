// SurfaceDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Ply2Print.h"
#include "SurfaceDialog.h"
#include "afxdialogex.h"


// SurfaceDialog �Ի���

IMPLEMENT_DYNAMIC(SurfaceDialog, CDialogEx)

SurfaceDialog::SurfaceDialog(CWnd* pParent /*=NULL*/)
: CDialogEx(SurfaceDialog::IDD, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

SurfaceDialog::~SurfaceDialog()
{
}

void SurfaceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, btn_check);
	DDX_Control(pDX, IDC_EDIT1, edit_times);
	DDX_Control(pDX, IDC_EDIT2, edit_step);
}


BEGIN_MESSAGE_MAP(SurfaceDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &SurfaceDialog::OnBnClickedOk)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// SurfaceDialog ��Ϣ�������


void SurfaceDialog::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
	CString info;
	// ��ȡѡ��״̬
	m_check = this->btn_check.GetCheck();
	this->edit_times.GetWindowTextW(info);
	swscanf_s(info, _T("%d"), &m_times);
	this->edit_step.GetWindowTextW(info);
	swscanf_s(info, _T("%lf"), &m_step_length);
}

void SurfaceDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// ���Ի���Ĭ��ֵ
	btn_check.SetCheck(1);
	edit_step.SetWindowTextW(_T("0.1"));
	edit_times.SetWindowTextW(_T("3"));
}