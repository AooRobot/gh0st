// RegisterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "client.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"


// CRegisterDlg 对话框

IMPLEMENT_DYNAMIC(CRegisterDlg, CDialogEx)

CRegisterDlg::CRegisterDlg(CClientSocket* pClientSocket,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_REGISTER, pParent)
	,m_pClientSocket(pClientSocket)
{

}

CRegisterDlg::~CRegisterDlg()
{
}

void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CRegisterDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CRegisterDlg 消息处理程序


void CRegisterDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	GetDlgItemText(IDC_EDIT1, m_strusername);
	GetDlgItemText(IDC_EDIT2, m_strpassword1);
	GetDlgItemText(IDC_EDIT3, m_strpassword2);

	if (m_strpassword1 != m_strpassword2)
	{
		SetDlgItemText(IDC_STC_INFO, "提示信息:两次密码不一致");
		return;
	}
	//构造一个注册封包和服务器通信

}
