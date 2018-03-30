
// gh0st.h : gh0st Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "gh0stView.h"      //���Ӱ���View���Ǹ���
#include "MySqlMng.h"
#include "Ini.h"

// Cgh0stApp:
// �йش����ʵ�֣������ gh0st.cpp
//

class Cgh0stApp : public CWinAppEx
{
public:
	Cgh0stApp();

    Cgh0stView* m_gh0stView;   //����һ��View����(��ΪtheApp�������̶��ܿ���,������Ҳ���Ա���ӵؿ���)


// ��д
public:
	virtual BOOL InitInstance();  //������������������
	virtual int ExitInstance();   //����رյ�ʱ����������

// ʵ��
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
