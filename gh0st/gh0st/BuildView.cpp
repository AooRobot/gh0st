// BuildView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "gh0st.h"
#include "BuildView.h"


// CBuildView

IMPLEMENT_DYNCREATE(CBuildView, CFormView)

CBuildView::CBuildView()
	: CFormView(IDD_BUILDVIEW)
{

}

CBuildView::~CBuildView()
{
}

void CBuildView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBuildView, CFormView)
END_MESSAGE_MAP()


// CBuildView ���

#ifdef _DEBUG
void CBuildView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CBuildView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBuildView ��Ϣ�������
