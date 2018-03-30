
// gh0stView.h : Cgh0stView ��Ľӿ�
//

#pragma once
#include "gh0stDoc.h"

#define CListView CCJListView

class Cgh0stView : public CListView
{
protected: // �������л�����
	Cgh0stView();
	DECLARE_DYNCREATE(Cgh0stView)

// ����
public:
	Cgh0stDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~Cgh0stView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void SendSelectCommand(BYTE * pData, UINT nSize);

private:
    CListCtrl *m_pListCtrl;

// ���ɵ���Ϣӳ�亯��
protected:
    afx_msg LRESULT OnMyInitialUpdate(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnAddToList(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnRemoveFromList(WPARAM wParam, LPARAM lParam);


	DECLARE_MESSAGE_MAP()  //��Ϣ�궨��
public:
	afx_msg void OnRename();
	afx_msg void OnPostmsg();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // gh0stView.cpp �еĵ��԰汾
inline Cgh0stDoc* Cgh0stView::GetDocument() const
   { return reinterpret_cast<Cgh0stDoc*>(m_pDocument); }
#endif

