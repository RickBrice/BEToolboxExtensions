// stdafx.cpp : source file that includes just the standard includes
// BEToolboxToolExtensions.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#if defined _DEBUG
#if defined _WIN64
#pragma comment(lib,"coin3d_x64.lib")
#pragma comment(lib,"sowin1d_x64.lib")
#else
#pragma comment(lib,"coin3d_Win32.lib")
#pragma comment(lib,"sowin1d_Win32.lib")
#endif
#else
#if defined _WIN64
#pragma comment(lib,"coin3_x64.lib")
#pragma comment(lib,"sowin1_x64.lib")
#else
#pragma comment(lib,"coin3_Win32.lib")
#pragma comment(lib,"sowin1_Win32.lib")
#endif
#endif // _DEBUG

