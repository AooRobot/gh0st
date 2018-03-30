
// clientDlg.h : 头文件
//

#pragma once


// CclientDlg 对话框
class CclientDlg : public CDialogEx
{
// 构造
public:
	CclientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStcRegister();
	afx_msg void OnStnClickedStcBackpwd();
	afx_msg void OnBnClickedButton1();

public:
	CClientSocket m_socketClient; //定义一个客户端
	CString m_strKillEvent;

	afx_msg void OnClose();
};
