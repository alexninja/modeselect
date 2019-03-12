#include "common.h"

/******************************************************************************************/

HINSTANCE g_hInstance;
HWND g_hwnd;
std::vector<DisplayMode> g_modes; //NOTE: we don't maintain a "current mode" index
std::wstring g_name;
TrayIcon g_trayicon;


/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, PSTR, int)
{
	// check for "already running" flag, set if not present

	wchar_t const* event_name =
		L"Graphics mode selector tool {B3748C40-6DB6-46a7-9117-A2BDB8FC2B23}";

	HANDLE event = OpenEvent(SYNCHRONIZE, FALSE, event_name);

	if (event)
		exit(1);
	else
		CreateEvent(NULL, FALSE, FALSE, event_name);


	/******************************************************************************************/

	g_hInstance = hInst;
	Load();


	/******************************************************************************************/

	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = NULL;
	wc.hCursor = NULL;
	wc.hIcon = NULL;
	wc.hInstance = g_hInstance;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.lpszClassName = L"hidden";
	wc.lpszMenuName = NULL;
	wc.style = 0;

	::RegisterClass(&wc);


	g_hwnd = ::CreateWindowEx(0, L"hidden", L"", NULL,
		0, 0, 0, 0, NULL, NULL, g_hInstance, NULL);


	/******************************************************************************************/

	g_trayicon.Attach(g_hwnd, g_hInstance);
	g_trayicon.Add();


	/******************************************************************************************/

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	/******************************************************************************************/

	g_trayicon.Delete();
}

