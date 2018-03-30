
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once

#include "TabSDIFrameWnd.h"  //���������ͷ�ļ�
#include "TrayIcon.h"

#define CFrameWnd CTabSDIFrameWnd

class CMainFrame : public CFrameWnd
{
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:
    void Activate(UINT nPort, UINT nMaxConnections);
    static void CALLBACK NotifyProc(LPVOID lpParam, ClientContext *pContext, UINT nCode);
    static void ProcessReceiveComplete(ClientContext *pContext);

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar        m_wndStatusBar;
	CTrayIcon         m_TrayIcon;    //����һ������ͼ��

// ���ɵ���Ϣӳ�亯��
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


