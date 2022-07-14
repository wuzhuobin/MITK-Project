#include "SCBWindowsUtil.h"
#include <Windows.h>

 bool EnableShutDownPriv()
{
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tkp = { 0 };
	//打开当前程序的权限令牌
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		return FALSE;
	}
	//获得某一特定权限的权限标识LUID，保存在tkp中
	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid))
	{
		CloseHandle(hToken);
		return FALSE;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	//调用AdjustTokenPrivileges来提升我们需要的系统权限
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
	{
		CloseHandle(hToken);
		return FALSE;
	}
	return TRUE;
}

 bool ReSetWindows(DWORD dwFlags, bool bForce)
{
	//检查参数是否正确
	if (dwFlags != EWX_LOGOFF&&dwFlags != EWX_REBOOT&&dwFlags != EWX_SHUTDOWN)
		return FALSE;
	//获得系统的版本信息，让我们后面确定是否需要提升系统权限
	OSVERSIONINFO osvi = { 0 };
	//获得参数的大小，以提供给GetVersionEx来判断这是一个新版本的OSVERSIONINFO，还是旧版本的
	//新版本是OSVERSIONINFOEX。扩充版本
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (!GetVersionEx(&osvi))
	{
		return FALSE;
	}
	//检查操作系统的版本，如果是NT类型的系统，需要提升系统权限
	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		EnableShutDownPriv();
	}
	//判断是否是强制关机，强制关闭系统进程。
	dwFlags |= (bForce != FALSE) ? EWX_FORCE : EWX_FORCEIFHUNG;
	//调用API
	return ExitWindowsEx(dwFlags, 0);
}
