
// clientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "client.h"
#include "clientDlg.h"
#include "afxdialogex.h"
#include "login.h"
#include "RegisterDlg.h"
#include "Include/KernelManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CclientDlg �Ի���
CclientDlg::CclientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CclientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_STN_CLICKED(IDC_STC_REGISTER, &CclientDlg::OnStnClickedStcRegister)
	ON_STN_CLICKED(IDC_STC_BACKPWD, &CclientDlg::OnStnClickedStcBackpwd)
	ON_BN_CLICKED(IDC_BUTTON1, &CclientDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


//�̻߳ص�����
DWORD WINAPI ConnectThreadProc(LPVOID lParam)
{

	//�Ժ���ܷܺ�æ
	CclientDlg *pClientDlg = (CclientDlg*)lParam;
	CKernelManager *manager = NULL;
	HANDLE hEvent = NULL;

	//���ӷ�����
	while (true)
	{
		//ÿ100����õ�һ��ɱ���¼�
		//�ȴ�һ�£���ʱ�õģ�
		//�������������ʱ��������sleep������
		//if (bBreakError != NOT_CONNECT & bBreakError != HEARTBEATTIMEOUT_ERROR)

		for (int i = 0; i < 20; i++)  //��δ�����������;��1.ÿ��������һ�η�����  2.ÿ100�������Ƿ���Ҫ���رգ�
		{
			hEvent = OpenEvent(EVENT_ALL_ACCESS, false, pClientDlg->m_strKillEvent);  //��һ���¼�
			//û�д����¼�,һ���򲻿�
			if (hEvent != NULL)
			{
				//�¼����������ܽ���
				pClientDlg->m_socketClient.Disconnect();   //�Ͽ�����

				CloseHandle(hEvent);  //�ر��¼����
				break;
			}
			Sleep(100);
		}

		if (manager)
		{
			delete manager;
			manager = NULL;
		}

		if (hEvent != NULL)
		{
			CloseHandle(hEvent);
			break;
		}

		//���ɱ���¼�û�����þ����ӷ���������Ļ���break
		if (!pClientDlg->m_socketClient.Connect("127.0.0.1", 8888))
		{
			pClientDlg->SetDlgItemText(IDC_STC_ERROR, "���ӷ�����ʧ�ܣ������������");
			DbgPrintA("Debug:����������ʧ��");

			((CButton*)pClientDlg->GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE);
			((CButton*)pClientDlg->GetDlgItem(IDC_STC_REGISTER))->EnableWindow(FALSE);
			((CButton*)pClientDlg->GetDlgItem(IDC_STC_BACKPWD))->EnableWindow(FALSE);
			continue;
		}
		else
		{
			//���ӳɹ����ð�ťΪ����

			//������Ϣ�ص��ࣨ�Զ���������������ķ����
		    manager = new CKernelManager(&pClientDlg->m_socketClient, pClientDlg->m_strKillEvent, "127.0.0.1", 8888);
			pClientDlg->m_socketClient.setManagerCallBack(manager);

			pClientDlg->SetDlgItemText(IDC_STC_ERROR, "���ӷ������ɹ�");
			((CButton*)pClientDlg->GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
			((CButton*)pClientDlg->GetDlgItem(IDC_STC_REGISTER))->EnableWindow(TRUE);
			((CButton*)pClientDlg->GetDlgItem(IDC_STC_BACKPWD))->EnableWindow(TRUE);
		}

		DbgPrintA("Debug:���������ӳɹ�");
		


		//���´������������߳�
		DWORD dwIOCPEvent = 0;
		do
		{
			hEvent = OpenEvent(EVENT_ALL_ACCESS, false, pClientDlg->m_strKillEvent);  //����ر��¼������ˣ�hEvent�Ͳ�ΪNULL��
			dwIOCPEvent = WaitForSingleObject(pClientDlg->m_socketClient.m_hEvent, 100);  //����������ж���ɶ˿ڶ�����ʱ���ж���

		} while (hEvent == NULL && dwIOCPEvent != WAIT_OBJECT_0);

		if (manager)
		{
			delete manager;
			manager = NULL;
		}

		if (hEvent != NULL)
		{
			pClientDlg->m_socketClient.Disconnect();   //�Ͽ��ͷ�����������
			CloseHandle(hEvent);   //�ر��¼����
			break;                 //�����ⲿѭ��
		}
	}
	//�ܵ���������߳���������
	return 0;
}


// CclientDlg ��Ϣ�������
BOOL CclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//******************************
	HANDLE hConnectThread = ::CreateThread(
		NULL,
		0,
		ConnectThreadProc,
		this,                               //������Ϊ�ص�����׼����
		0,
		NULL);
	//*******************************

	CloseHandle(hConnectThread);  //�رվ�������������߳�����������˵��������ûʲô����

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CclientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CclientDlg::OnStnClickedStcRegister()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CRegisterDlg dlg(&m_socketClient);

	dlg.DoModal();
	
}


void CclientDlg::OnStnClickedStcBackpwd()
{
	AfxMessageBox("�һ�����");
}



void CclientDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strUserName, strPassword;

	DWORD dwTickCount = GetTickCount();  //���ϵͳʱ��

	GetDlgItemText(IDC_EDIT1, strUserName);
	GetDlgItemText(IDC_EDIT2, strPassword);


	sendLoginInfo(NULL, &m_socketClient, GetTickCount() - dwTickCount,
		strUserName.GetString(), strPassword.GetString());
}


void CclientDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_socketClient.Disconnect();

	CDialogEx::OnClose();
}
