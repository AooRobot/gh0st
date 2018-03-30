#if !defined(AFX_UNTIL_CPP_INCLUDED)
#define AFX_UNTIL_CPP_INCLUDED
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <Wtsapi32.h>
#include <wininet.h>
#pragma comment(lib, "Wtsapi32.lib")
#include "until.h"

unsigned int __stdcall ThreadLoader(LPVOID param)
{
	unsigned int	nRet = 0;

	THREAD_ARGLIST	arg;
	memcpy(&arg, param, sizeof(arg));
	SetEvent(arg.hEventTransferArg);
	// Óë×¿Ãæ½»»¥
	//if (arg.bInteractive)
	//	SelectDesktop(NULL);

	nRet = arg.start_address(arg.arglist);

	return nRet;
}

HANDLE MyCreateThread (LPSECURITY_ATTRIBUTES lpThreadAttributes, // SD
					   SIZE_T dwStackSize,                       // initial stack size
					   LPTHREAD_START_ROUTINE lpStartAddress,    // thread function
					   LPVOID lpParameter,                       // thread argument
					   DWORD dwCreationFlags,                    // creation option
					   LPDWORD lpThreadId, bool bInteractive)
{
	HANDLE	hThread = INVALID_HANDLE_VALUE;
	THREAD_ARGLIST	arg;
	arg.start_address = (unsigned ( __stdcall *)( void * ))lpStartAddress;
	arg.arglist = (void *)lpParameter;
	arg.bInteractive = bInteractive;
	arg.hEventTransferArg = CreateEvent(NULL, false, false, NULL);
	hThread = (HANDLE)_beginthreadex((void *)lpThreadAttributes, dwStackSize, ThreadLoader, &arg, dwCreationFlags, (unsigned *)lpThreadId);
	WaitForSingleObject(arg.hEventTransferArg, INFINITE);
	CloseHandle(arg.hEventTransferArg);
	
	return hThread;
}


#endif // !defined(AFX_UNTIL_CPP_INCLUDED)