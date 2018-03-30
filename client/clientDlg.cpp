
// clientDlg.cpp : 实现文件
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


// CclientDlg 对话框
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


//线程回调函数
DWORD WINAPI ConnectThreadProc(LPVOID lParam)
{

	//以后可能很繁忙
	CclientDlg *pClientDlg = (CclientDlg*)lParam;
	CKernelManager *manager = NULL;
	HANDLE hEvent = NULL;

	//连接服务器
	while (true)
	{
		//每100毫秒得到一次杀死事件
		//等待一下（延时用的）
		//如果不是心跳超时，不用再sleep两分钟
		//if (bBreakError != NOT_CONNECT & bBreakError != HEARTBEATTIMEOUT_ERROR)

		for (int i = 0; i < 20; i++)  //这段代码有两个用途（1.每两秒连接一次服务器  2.每100毫秒检测是否需要被关闭）
		{
			hEvent = OpenEvent(EVENT_ALL_ACCESS, false, pClientDlg->m_strKillEvent);  //打开一个事件
			//没有创建事件,一定打不开
			if (hEvent != NULL)
			{
				//事件被创建才能进来
				pClientDlg->m_socketClient.Disconnect();   //断开连接

				CloseHandle(hEvent);  //关闭事件句柄
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

		//如果杀死事件没有设置就连接服务器否则的话就break
		if (!pClientDlg->m_socketClient.Connect("127.0.0.1", 8888))
		{
			pClientDlg->SetDlgItemText(IDC_STC_ERROR, "连接服务器失败，请检查你的网络");
			DbgPrintA("Debug:服务器连接失败");

			((CButton*)pClientDlg->GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE);
			((CButton*)pClientDlg->GetDlgItem(IDC_STC_REGISTER))->EnableWindow(FALSE);
			((CButton*)pClientDlg->GetDlgItem(IDC_STC_BACKPWD))->EnableWindow(FALSE);
			continue;
		}
		else
		{
			//连接成功设置按钮为正常

			//设置消息回调类（自动处理服务器过来的封包）
		    manager = new CKernelManager(&pClientDlg->m_socketClient, pClientDlg->m_strKillEvent, "127.0.0.1", 8888);
			pClientDlg->m_socketClient.setManagerCallBack(manager);

			pClientDlg->SetDlgItemText(IDC_STC_ERROR, "连接服务器成功");
			((CButton*)pClientDlg->GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
			((CButton*)pClientDlg->GetDlgItem(IDC_STC_REGISTER))->EnableWindow(TRUE);
			((CButton*)pClientDlg->GetDlgItem(IDC_STC_BACKPWD))->EnableWindow(TRUE);
		}

		DbgPrintA("Debug:服务器连接成功");
		


		//以下代码用来阻塞线程
		DWORD dwIOCPEvent = 0;
		do
		{
			hEvent = OpenEvent(EVENT_ALL_ACCESS, false, pClientDlg->m_strKillEvent);  //如果关闭事件发生了，hEvent就不为NULL了
			dwIOCPEvent = WaitForSingleObject(pClientDlg->m_socketClient.m_hEvent, 100);  //这个是用来判断完成端口对列里时候还有东西

		} while (hEvent == NULL && dwIOCPEvent != WAIT_OBJECT_0);

		if (manager)
		{
			delete manager;
			manager = NULL;
		}

		if (hEvent != NULL)
		{
			pClientDlg->m_socketClient.Disconnect();   //断开和服务器的连接
			CloseHandle(hEvent);   //关闭事件句柄
			break;                 //掐断外部循环
		}
	}
	//能到这里，代表线程正常死亡
	return 0;
}


// CclientDlg 消息处理程序
BOOL CclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//******************************
	HANDLE hConnectThread = ::CreateThread(
		NULL,
		0,
		ConnectThreadProc,
		this,                               //参数是为回调函数准备的
		0,
		NULL);
	//*******************************

	CloseHandle(hConnectThread);  //关闭句柄，并不代表线程死亡，仅仅说明这个句柄没什么用了

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CclientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CclientDlg::OnStnClickedStcRegister()
{
	// TODO: 在此添加控件通知处理程序代码
	CRegisterDlg dlg(&m_socketClient);

	dlg.DoModal();
	
}


void CclientDlg::OnStnClickedStcBackpwd()
{
	AfxMessageBox("找回密码");
}



void CclientDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strUserName, strPassword;

	DWORD dwTickCount = GetTickCount();  //获得系统时间

	GetDlgItemText(IDC_EDIT1, strUserName);
	GetDlgItemText(IDC_EDIT2, strPassword);


	sendLoginInfo(NULL, &m_socketClient, GetTickCount() - dwTickCount,
		strUserName.GetString(), strPassword.GetString());
}


void CclientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_socketClient.Disconnect();

	CDialogEx::OnClose();
}
