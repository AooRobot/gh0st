
// gh0st.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "gh0st.h"
#include "MainFrm.h"

#include "gh0stDoc.h"
#include "gh0stView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cgh0stApp

BEGIN_MESSAGE_MAP(Cgh0stApp, CWinAppEx)    //消息映射头
	ON_COMMAND(ID_APP_ABOUT, &Cgh0stApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()                          //消息映射尾


// Cgh0stApp 构造

Cgh0stApp::Cgh0stApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则: 
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("gh0st.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	GetCurrentDirectory(MAX_PATH, m_AppPath);   //获得当前路径


	CString strIni;
	strIni.Format("%s\\配置文件.ini", m_AppPath);   //格式化一个INI的路径
	m_Ini.SetPathName(strIni);  //设置INI路径
}

// 唯一的一个 Cgh0stApp 对象

Cgh0stApp theApp;   //全局的对象


// Cgh0stApp 初始化

BOOL Cgh0stApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要 AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	// 注册应用程序的文档模板。  文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;

	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(Cgh0stDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(Cgh0stView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 调度在命令行中指定的命令。  如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

    m_pMainWnd->SetMenu(NULL);

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);   //显示窗体
	m_pMainWnd->UpdateWindow();        //更新窗体

    //激活服务器
    ((CMainFrame*)m_pMainWnd)->Activate(8888, 2000);
	//连接数据库
	CONNECT_INFO connectinfo;
	connectinfo.host = "127.0.0.1";
	connectinfo.user = "root";
	connectinfo.port = "3306";
	connectinfo.passwd = "111111";
	connectinfo.Db = "authdemo";
	connectinfo.charset = "UTF8";  //支持中文
	connectinfo.Msg = "";          //错误消息

	if (m_MysqlMng.Connect(connectinfo))
	{
		AfxMessageBox(connectinfo.Msg.c_str());
	}

	return TRUE;
}

int Cgh0stApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	m_MysqlMng.DisConnect();

	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// Cgh0stApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void Cgh0stApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// Cgh0stApp 消息处理程序



