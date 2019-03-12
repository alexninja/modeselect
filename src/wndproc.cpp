#include "common.h"
extern void AddTrayIcon();

/******************************************************************************************/

HMENU MakeMenu()
{
	HMENU hmenu = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_MENU1));
	HMENU popup = GetSubMenu(hmenu, 0);
	SetMenuDefaultItem(popup, 0, TRUE);

	bool exists = false;
	DisplayMode current_mode;
	current_mode.Get();

	int menu_position = 2;
	MENUITEMINFO mii;

	for (unsigned i = 0; i < g_modes.size(); i++)
	{
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_STATE | MIIM_FTYPE | MIIM_STRING | MIIM_ID | MIIM_STATE;
		mii.fType = MFT_STRING;

		const std::wstring& text = g_modes[i].GetName();
		mii.dwTypeData = (LPWSTR)text.c_str();
		mii.cch = wcslen(mii.dwTypeData);
		mii.fState = MFS_ENABLED;
		mii.wID = ID_MODE + i;

		if (g_modes[i] == current_mode)
		{
			exists = true;
			mii.fState |= MFS_CHECKED;
		}

		InsertMenuItem(popup, menu_position++, TRUE, &mii);
	}

	if (g_modes.size() > 0)
	{
		mii.fMask = MIIM_FTYPE;
		mii.fType = MFT_SEPARATOR;

		InsertMenuItem(popup, menu_position, TRUE, &mii);
	}

	if (exists)
		EnableMenuItem(popup, ID_SAVEAS, MF_GRAYED);

	return popup;
}

/******************************************************************************************/

LRESULT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static UINT wm_taskbar_created = 0;

	switch (message)
	{
	case WM_CREATE:
		wm_taskbar_created = ::RegisterWindowMessage(L"TaskbarCreated");
		break;

	case WM_TRAYICON:
		if (lParam == WM_RBUTTONUP)
		{
			POINT mouse;
			::GetCursorPos(&mouse);
			::SetForegroundWindow(g_hwnd); //Q135788

			HMENU popup = MakeMenu();
			::TrackPopupMenu(popup, 0, mouse.x, mouse.y, 0, g_hwnd, NULL);
			::DestroyMenu(popup);
		}
		break;


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_EXIT:
			::PostQuitMessage(0);
			break;

		case ID_SAVEAS:
		{
			DisplayMode dm;
			dm.Get();
			g_name = dm.GetTemporaryName();

			INT_PTR res =
				::DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), g_hwnd, DlgProc);

			if (res) {
				dm.SetName(g_name);
				g_modes.push_back(dm);
				std::sort(g_modes.begin(), g_modes.end());
			}

			Save();
			g_trayicon.UpdateTip();
		}
		break;
		}

		// select new mode
		if (LOWORD(wParam) >= ID_MODE) // i.e. "case ID_MODE and up"
		{
			int index = LOWORD(wParam) - ID_MODE;
			g_modes[index].Set();
			g_trayicon.UpdateTip();
		}
		break;


	case WM_MENURBUTTONUP:
	{
		// delete mode from database
		unsigned item_index = wParam - 2;
		if (item_index >= 0 && item_index < g_modes.size())
		{
			POINT mouse;
			::GetCursorPos(&mouse);

			HMENU menu = ::LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_MENU2));
			HMENU popup = ::GetSubMenu(menu, 0);

			if (::TrackPopupMenu(popup, TPM_RECURSE | TPM_RETURNCMD,
				mouse.x, mouse.y, 0, g_hwnd, NULL) == ID_DELETE)
			{
				if (item_index >= 0 && item_index < g_modes.size())
				{
					g_modes.erase(g_modes.begin() + item_index);
					Save();
					g_trayicon.UpdateTip();
					// fake ESC keypress, otherwise TrackPopupMenu stays on screen
					::PostMessage(g_hwnd, WM_KEYDOWN, 0x1B, 0);
				}
			}

			::DestroyMenu(menu);
		}
	}
	break;


	case WM_DISPLAYCHANGE:
		g_trayicon.UpdateTip();  // resolution changed
		break;


	default:
		if (message == wm_taskbar_created) {
			g_trayicon.Delete(); // clears internal "is_showing" flag
			g_trayicon.Add();
		}

		return ::DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}
