// KernelManager.h: interface for the CKernelManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KERNELMANAGER_H__D38BBAEA_31C6_4C8A_8BF7_BF3E80182EAE__INCLUDED_)
#define AFX_KERNELMANAGER_H__D38BBAEA_31C6_4C8A_8BF7_BF3E80182EAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Manager.h"

class CKernelManager : public CManager  
{
public:
	CKernelManager(CClientSocket *pClient,LPCTSTR lpszKillEvent,
		LPCTSTR lpszMasterHost, UINT nMasterPort);

	virtual ~CKernelManager();
	virtual void OnReceive(LPBYTE lpBuffer, UINT nSize);

	char	m_strKillEvent[256];

	static	char	m_strMasterHost[256];
	static	UINT	m_nMasterPort;
	void	UnInstallService();
	bool	IsActived();
private:
	HANDLE	m_hThread[10000]; // �㹻����
	UINT	m_nThreadCount;
 
	bool	m_bIsActived;
};

#endif // !defined(AFX_KERNELMANAGER_H__D38BBAEA_31C6_4C8A_8BF7_BF3E80182EAE__INCLUDED_)
