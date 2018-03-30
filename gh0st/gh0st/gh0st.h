
// gh0st.h : gh0st 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "gh0stView.h"      //增加包含View的那个类
#include "MySqlMng.h"
#include "Ini.h"

// Cgh0stApp:
// 有关此类的实现，请参阅 gh0st.cpp
//

class Cgh0stApp : public CWinAppEx
{
public:
	Cgh0stApp();

    Cgh0stView* m_gh0stView;   //定义一个View对象(因为theApp整个工程都能看到,所以它也可以被间接地看到)


// 重写
public:
	virtual BOOL InitInstance();  //程序启动会做的事情
	virtual int ExitInstance();   //程序关闭的时候做的事情

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	CMySqlMng m_MysqlMng;
	CHAR      m_AppPath[MAX_PATH];
	CIni      m_Ini;

	BOOL      m_bSound;
	BOOL      m_bFlash;
	CString   m_strSoundFile;
};

extern Cgh0stApp theApp;
