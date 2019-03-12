class TrayIcon
{
	NOTIFYICONDATA m_nid;
	bool is_showing;

public:
	TrayIcon();
	void Attach(HWND, HINSTANCE);
	void Add();
	void Delete();
	void UpdateTip();
};
