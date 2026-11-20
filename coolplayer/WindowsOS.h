
/*
 * CoolPlayer - Blazing fast audio player.
 * Copyright (C) 2000-2001 Niek Albers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
//
// OS functions
//
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Win2000/98 API stuff
// needed only for VC6 compile **/
#ifndef MONITOR_DEFAULTTONULL
typedef void* HMONITOR;

typedef struct _MONITORINFO
{
	DWORD  cbSize;
	RECT   rcMonitor;
	RECT   rcWork;
	DWORD  dwFlags;
} MONITORINFO;

#define MONITOR_DEFAULTTONULL       0x00000000
#define MONITOR_DEFAULTTOPRIMARY    0x00000001
#define MONITOR_DEFAULTTONEAREST    0x00000002
#endif
/* NT 4.0; not in the Visual C++ 4.1 headers (1995) */
#ifndef TME_LEAVE
typedef struct tagTRACKMOUSEEVENT {
	DWORD cbSize;
	DWORD dwFlags;
	HWND  hwndTrack;
	DWORD dwHoverTime;
} TRACKMOUSEEVENT, *LPTRACKMOUSEEVENT;

#define TME_LEAVE     0x00000002
#define WM_MOUSELEAVE 0x02A3
#endif
typedef BOOL (WINAPI *wp_GetMonitorInfo)(HMONITOR hMonitor, MONITORINFO* lpmi);
typedef HMONITOR(WINAPI *wp_MonitorFromWindow)(HWND hwnd, DWORD dwFlags);
typedef BOOL (WINAPI *wp_TrackMouseEvent)(LPTRACKMOUSEEVENT lpEventTrack);
//
extern wp_GetMonitorInfo pfnGetMonitorInfo;
extern wp_MonitorFromWindow pfnMonitorFromWindow;
extern wp_TrackMouseEvent pfnTrackMouseEvent;
//
////////////////////////////////////////////////////////////////////////////////
void CP_InitWindowsRoutines();

/* WinINet. WININET.DLL comes with Internet Explorer: NT 4.0 alone has none,
   and the 1996 SDK has no wininet.h or wininet.lib. CP_LoadWinInet loads it
   on first use. These are the names from wininet.h that CoolPlayer uses. */
#ifndef INTERNET_OPEN_TYPE_PRECONFIG
typedef LPVOID HINTERNET;
#define INTERNET_OPEN_TYPE_PRECONFIG    0
#define INTERNET_OPTION_CONNECT_TIMEOUT 2
#define INTERNET_FLAG_NO_CACHE_WRITE    0x04000000
#endif
#ifndef INTERNET_FLAG_PRAGMA_NOCACHE
#define INTERNET_FLAG_PRAGMA_NOCACHE    0x00000100
#endif

typedef struct _CPs_InternetBuffers {  /* INTERNET_BUFFERS */
	DWORD dwStructSize;
	struct _CPs_InternetBuffers* Next;
	LPCSTR lpcszHeader;
	DWORD dwHeadersLength;
	DWORD dwHeadersTotal;
	LPVOID lpvBuffer;
	DWORD dwBufferLength;
	DWORD dwBufferTotal;
	DWORD dwOffsetLow;
	DWORD dwOffsetHigh;
} CPs_InternetBuffers;

typedef HINTERNET (WINAPI *wp_InternetOpen)(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags);
typedef HINTERNET (WINAPI *wp_InternetOpenUrl)(HINTERNET hInternet, LPCSTR lpszUrl, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD dwContext);
typedef BOOL (WINAPI *wp_InternetReadFileEx)(HINTERNET hFile, CPs_InternetBuffers* lpBuffersOut, DWORD dwFlags, DWORD dwContext);
typedef BOOL (WINAPI *wp_InternetSetOption)(HINTERNET hInternet, DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength);
typedef BOOL (WINAPI *wp_InternetCloseHandle)(HINTERNET hInternet);

extern wp_InternetOpen pfnInternetOpen;
extern wp_InternetOpenUrl pfnInternetOpenUrl;
extern wp_InternetReadFileEx pfnInternetReadFileEx;
extern wp_InternetSetOption pfnInternetSetOption;
extern wp_InternetCloseHandle pfnInternetCloseHandle;

BOOL CP_LoadWinInet(void);
