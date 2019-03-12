#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN
#define ISOLATION_AWARE_ENABLED 1
#define WM_TRAYICON (WM_USER+1000)
#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#include <shlobj.h>
#include <vector>
#include <string>
#include <algorithm>

#define NAME_LIMIT 50

#include "resource.h"
#include "displaymode.h"
#include "trayicon.h"


// externs

LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
void Save();
void Load();

extern HINSTANCE g_hInstance;
extern HWND g_hwnd;
extern std::vector<DisplayMode> g_modes;
extern std::wstring g_name;
extern TrayIcon g_trayicon;
