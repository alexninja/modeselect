#include "common.h"


/******************************************************************************************/

std::wstring GetEditText(HWND hdlg)
{
	TCHAR buf[NAME_LIMIT + 1];
	WORD length = (WORD)SendDlgItemMessage(hdlg, IDC_EDIT1, EM_LINELENGTH, 0, 0);
	*(WORD*)buf = length;
	SendDlgItemMessage(hdlg, IDC_EDIT1, EM_GETLINE, 0, (LPARAM)buf);
	buf[length] = '\0';

	return std::wstring(buf);
}

/******************************************************************************************/

INT_PTR CALLBACK DlgProc(HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		// move window to bottom-right
		RECT rc;
		GetWindowRect(hdlg, &rc);
		SIZE window;
		window.cx = rc.right - rc.left;
		window.cy = rc.bottom - rc.top;

		SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);

		MoveWindow(hdlg, rc.right - window.cx - 10, rc.bottom - window.cy - 10,
			window.cx, window.cy, FALSE);

		// init edit field
		SendDlgItemMessage(hdlg, IDC_EDIT1, EM_LIMITTEXT, NAME_LIMIT, 0);
		SetDlgItemText(hdlg, IDC_EDIT1, g_name.c_str());
	}
	return TRUE;


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_EDIT1:
			if (HIWORD(wParam) == EN_CHANGE)
			{ // disable OK if name is already taken
				std::wstring text = GetEditText(hdlg);

				bool exists = false;
				for (unsigned i = 0; i < g_modes.size(); i++)
					if (g_modes[i].GetName() == text) exists = true;

				HWND hwndOK = GetDlgItem(hdlg, IDOK);

				if (exists)
					EnableWindow(hwndOK, FALSE);
				else
					EnableWindow(hwndOK, TRUE);
			}
			return TRUE;

		case IDOK:
			g_name = GetEditText(hdlg);
			if (!g_name.empty()) EndDialog(hdlg, 1);
			return TRUE;

		case IDCANCEL:
			EndDialog(hdlg, 0);
			return TRUE;
		}


	default:
		return FALSE;
	}
}

