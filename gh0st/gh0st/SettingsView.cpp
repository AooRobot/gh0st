// SettingsView.cpp : 实现文件
//

#include "stdafx.h"
#include "gh0st.h"
#include "SettingsView.h"


// CSettingsView

IMPLEMENT_DYNCREATE(CSettingsView, CFormView)

CSettingsView::CSettingsView()
	: CFormView(IDD_SETTINGSVIEW)
{

}

CSettingsView::~CSettingsView()
{
}

void CSettingsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSettingsView, CFormView)
	ON_BN_CLICKED(IDC_CHECK1, &CSettingsView::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CSettingsView::OnBnClickedCheck2)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON1, &CSettingsView::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSettingsView 诊断

#ifdef _DEBUG
void CSettingsView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSettingsView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSettingsView 消息处理程序


void CSettingsView::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() )
	{
		((Cgh0stApp*)AfxGetApp())->m_bSound = TRUE;
		((Cgh0stApp*)AfxGetApp())->m_Ini.WriteBool("Setting", "PlaySound", TRUE);  //TRUE 00000001   true 01
	}
	else
	{
		((Cgh0stApp*)AfxGetApp())->m_Ini.WriteBool("Setting", "PlaySound", FALSE); //FALSE 00000000   false 00
		((Cgh0stApp*)AfxGetApp())->m_bSound = FALSE;
	}
}


void CSettingsView::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck())
	{
		((Cgh0stApp*)AfxGetApp())->m_Ini.WriteBool("Setting", "IconFlash", TRUE);  //TRUE 00000001   true 01
		((Cgh0stApp*)AfxGetApp())->m_bFlash = TRUE;
	}
	else
	{
		((Cgh0stApp*)AfxGetApp())->m_Ini.WriteBool("Setting", "IconFlash", FALSE); //FALSE 00000000   false 00
		((Cgh0stApp*)AfxGetApp())->m_bFlash = FALSE;
	}
}




void CSettingsView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: 在此添加专用代码和/或调用基类
	static bool bInit = false;

	if (!bInit)
	{
		BOOL bSound = ((Cgh0stApp*)AfxGetApp())->m_Ini.GetBool("Setting", "PlaySound", FALSE);

		((Cgh0stApp*)AfxGetApp())->m_bSound = bSound;

		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(bSound);

		


		BOOL bFlash = ((Cgh0stApp*)AfxGetApp())->m_Ini.GetBool("Setting", "IconFlash", FALSE);

		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(bFlash);

		((Cgh0stApp*)AfxGetApp())->m_bFlash = bFlash;


		CString strSoundFile = ((Cgh0stApp*)AfxGetApp())->m_Ini.GetString("Setting", "SoundFile", "");

		SetDlgItemText(IDC_EDIT1, strSoundFile);

		((Cgh0stApp*)AfxGetApp())->m_strSoundFile = strSoundFile;
		bInit = true;
	}

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

CString CSettingsView::GetOpenSoundFile()
{
	CFileDialog dlgFileOpen(TRUE, _T(".wav"), NULL, OFN_FILEMUSTEXIST,
		_T("声音文件(*.wav)|*.wav||"), NULL);

	char Path[MAX_PATH];
	GetModuleFileName(NULL, Path, MAX_PATH);
	PathRemoveFileSpec(Path);

	int StructSize = 0;
	DWORD dwVersion = GetVersion();
	if (dwVersion < 0x80000000)
	{
		StructSize = 88;
	}
	else
	{
		StructSize = 76;
	}
		
	
	dlgFileOpen.m_ofn.lStructSize = StructSize;
	dlgFileOpen.m_ofn.lpstrInitialDir = _T(Path);

	if (dlgFileOpen.DoModal() == IDOK)
	{
		return (CString)dlgFileOpen.m_ofn.lpstrFile;
	}

	return _T("");
}

void CSettingsView::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strPath = GetOpenSoundFile();
	if (!strPath.IsEmpty())
	{
		SetDlgItemText(IDC_EDIT1, strPath);

		((Cgh0stApp*)AfxGetApp())->m_Ini.WriteString("Setting", "SoundFile", strPath);

		((Cgh0stApp*)AfxGetApp())->m_strSoundFile = strPath;
	}
	
}
