#pragma once



// CSettingsView ������ͼ

class CSettingsView : public CFormView
{
	DECLARE_DYNCREATE(CSettingsView)

public:
    CSettingsView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
    virtual ~CSettingsView();

protected:
	
public:
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_SETTINGSVIEW };
#endif
#ifdef _DEBUG
    virtual void AssertValid() const;
#ifndef _WIN32_WCE
    virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	afx_msg void OnBnClickedButton1();
	afx_msg CString GetOpenSoundFile();

};


