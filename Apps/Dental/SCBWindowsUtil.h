#ifndef __SCB_WINDOWS_UTIL_H__
#define __SCB_WINDOWS_UTIL_H__
#include <Windows.h>

extern bool EnableShutDownPriv();
extern bool ReSetWindows(DWORD dwFlags, bool bForce);
#endif 