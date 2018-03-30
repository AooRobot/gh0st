
// MainFrm.h : CMainFrame 类的接口
//

#pragma once

#include "TabSDIFrameWnd.h"  //引入界面主头文件
#include "TrayIcon.h"

#define CFrameWnd CTabSDIFrameWnd

class CMainFrame : public CFrameWnd
{
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:
    void Activate(UINT nPort, UINT nMaxConnections);
    static void CALLBACK NotifyProc(LPVOID lpParam, ClientContext *pContext, UINT nCode);
    static void ProcessReceiveComplete(ClientContext *pContext);

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar        m_wndStatusBar;
	CTrayIcon         m_TrayIcon;    //定义一个托盘图标

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnShow();
	afx_msg void OnHide();
	afx_msg void OnQuit();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


