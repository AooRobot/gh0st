// RegisterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "client.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"


// CRegisterDlg �Ի���

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


// CRegisterDlg ��Ϣ�������


void CRegisterDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	GetDlgItemText(IDC_EDIT1, m_strusername);
	GetDlgItemText(IDC_EDIT2, m_strpassword1);
	GetDlgItemText(IDC_EDIT3, m_strpassword2);

	if (m_strpassword1 != m_strpassword2)
	{
		SetDlgItemText(IDC_STC_INFO, "��ʾ��Ϣ:�������벻һ��");
		return;
	}
	//����һ��ע�����ͷ�����ͨ��

}
