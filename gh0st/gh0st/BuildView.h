#pragma once



// CBuildView ������ͼ

class CBuildView : public CFormView
{
	DECLARE_DYNCREATE(CBuildView)

public:
    CBuildView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
    virtual ~CBuildView();
protected:


public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BUILDVIEW };
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
};


