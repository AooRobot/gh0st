
// clientDlg.h : ͷ�ļ�
//

#pragma once


// CclientDlg �Ի���
class CclientDlg : public CDialogEx
{
// ����
public:
	CclientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStcRegister();
	afx_msg void OnStnClickedStcBackpwd();
	afx_msg void OnBnClickedButton1();

public:
	CClientSocket m_socketClient; //����һ���ͻ���
	CString m_strKillEvent;

	afx_msg void OnClose();
};
