#include "common.h"

/********************************************************************************************/

TrayIcon::TrayIcon()
{
	is_showing = false;
	memset(&m_nid, 0, sizeof(m_nid));

	m_nid.cbSize = NOTIFYICONDATA_V1_SIZE; // behave like win95 struct
	m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_nid.uID = 1;
	m_nid.uCallbackMessage = WM_TRAYICON;
	m_nid.szTip[0] = 0;
}

/********************************************************************************************/

void TrayIcon::Attach(HWND parent, HINSTANCE hInst)
{
	m_nid.hWnd = parent;
	m_nid.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
}

/********************************************************************************************/

void TrayIcon::Add()
{
	if (!is_showing)
	{
		is_showing = true;
		::Shell_NotifyIcon(NIM_ADD, &m_nid);
		UpdateTip();
	}
}

/********************************************************************************************/

void TrayIcon::Delete()
{
	if (is_showing)
	{
		::Shell_NotifyIcon(NIM_DELETE, &m_nid);
		is_showing = false;
	}
}

/********************************************************************************************/

void TrayIcon::UpdateTip()
{
	if (is_showing)
	{
		// if current mode isn't in the database, show in XxYxC (Hz) format
		// if listed, show by name

		DisplayMode curr_mode;
		curr_mode.Get();
		std::wstring name = curr_mode.GetTemporaryName();

		for (unsigned i = 0; i < g_modes.size(); ++i)
		{
			if (g_modes[i] == curr_mode)
			{
				name = g_modes[i].GetName();
				break;
			}
		}

		// copy to nid structure
		if (!name.empty())
		{
			_tcsncpy(m_nid.szTip, name.c_str(), 63);
			m_nid.szTip[63] = 0;
		}
		else
			m_nid.szTip[0] = 0;

		// update
		::Shell_NotifyIcon(NIM_MODIFY, &m_nid);
	}
}
