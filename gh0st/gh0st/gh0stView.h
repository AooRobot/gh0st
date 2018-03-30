
// gh0stView.h : Cgh0stView 类的接口
//

#pragma once
#include "gh0stDoc.h"

#define CListView CCJListView

class Cgh0stView : public CListView
{
protected: // 仅从序列化创建
	Cgh0stView();
	DECLARE_DYNCREATE(Cgh0stView)

// 特性
public:
	Cgh0stDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
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

// 生成的消息映射函数
protected:
    afx_msg LRESULT OnMyInitialUpdate(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnAddToList(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnRemoveFromList(WPARAM wParam, LPARAM lParam);


	DECLARE_MESSAGE_MAP()  //消息宏定义
public:
	afx_msg void OnRename();
	afx_msg void OnPostmsg();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // gh0stView.cpp 中的调试版本
inline Cgh0stDoc* Cgh0stView::GetDocument() const
   { return reinterpret_cast<Cgh0stDoc*>(m_pDocument); }
#endif

