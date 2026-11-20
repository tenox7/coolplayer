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



#include "stdafx.h"
#include "globals.h"
#include "WindowsOS.h"



wp_GetMonitorInfo pfnGetMonitorInfo;
wp_MonitorFromWindow pfnMonitorFromWindow;
wp_TrackMouseEvent pfnTrackMouseEvent;
wp_InternetOpen pfnInternetOpen;
wp_InternetOpenUrl pfnInternetOpenUrl;
wp_InternetReadFileEx pfnInternetReadFileEx;
wp_InternetSetOption pfnInternetSetOption;
wp_InternetCloseHandle pfnInternetCloseHandle;
////////////////////////////////////////////////////////////////////////////////
//
//
//
void CP_InitWindowsRoutines()
{
	HMODULE hmUser32 = GetModuleHandle("USER32");
	pfnGetMonitorInfo = (wp_GetMonitorInfo)GetProcAddress(hmUser32, "GetMonitorInfoA");
	pfnMonitorFromWindow = (wp_MonitorFromWindow)GetProcAddress(hmUser32, "MonitorFromWindow");
	pfnTrackMouseEvent = (wp_TrackMouseEvent)GetProcAddress(hmUser32, "TrackMouseEvent");
}

/* Loads WININET.DLL on first use. FALSE: there is none, or it lacks one of
   the functions below. Then there are no internet streams. */
BOOL CP_LoadWinInet(void) {
	static BOOL bTried = FALSE;
	HMODULE hmWinInet;

	if (bTried)
		return pfnInternetOpen != NULL;
	bTried = TRUE;
	hmWinInet = LoadLibrary("WININET.DLL");
	if (hmWinInet == NULL)
		return FALSE;
	pfnInternetOpenUrl = (wp_InternetOpenUrl)GetProcAddress(hmWinInet, "InternetOpenUrlA");
	pfnInternetReadFileEx = (wp_InternetReadFileEx)GetProcAddress(hmWinInet, "InternetReadFileExA");
	pfnInternetSetOption = (wp_InternetSetOption)GetProcAddress(hmWinInet, "InternetSetOptionA");
	pfnInternetCloseHandle = (wp_InternetCloseHandle)GetProcAddress(hmWinInet, "InternetCloseHandle");
	if (!pfnInternetOpenUrl || !pfnInternetReadFileEx || !pfnInternetSetOption || !pfnInternetCloseHandle) {
		FreeLibrary(hmWinInet);
		return FALSE;
	}
	/* last: the callers test this one */
	pfnInternetOpen = (wp_InternetOpen)GetProcAddress(hmWinInet, "InternetOpenA");
	return pfnInternetOpen != NULL;
}

//
//
//
