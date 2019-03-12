#include "common.h"


/******************************************************************************************/

void DisplayMode::Get()
{
	DEVMODE dm;
	dm.dmSize = sizeof(DEVMODE);
	dm.dmDriverExtra = 0;

	::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);

	BitsPerPel = dm.dmBitsPerPel;
	PelsWidth = dm.dmPelsWidth;
	PelsHeight = dm.dmPelsHeight;
	DisplayFlags = dm.dmDisplayFlags;
	DisplayFrequency = dm.dmDisplayFrequency;
}

/******************************************************************************************/

void DisplayMode::Set()
{
	DEVMODE dm;
	dm.dmSize = sizeof(DEVMODE);
	dm.dmDriverExtra = 0;
	dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT
		| DM_DISPLAYFLAGS | DM_DISPLAYFREQUENCY;

	dm.dmBitsPerPel = BitsPerPel;
	dm.dmPelsWidth = PelsWidth;
	dm.dmPelsHeight = PelsHeight;
	dm.dmDisplayFlags = DisplayFlags;
	dm.dmDisplayFrequency = DisplayFrequency;

	// set
	if (S_OK == ::ChangeDisplaySettings(&dm, CDS_TEST))
	{
		::ChangeDisplaySettings(&dm, CDS_UPDATEREGISTRY);
	}
	else
		::MessageBox(g_hwnd,
			L"This display mode is not supported.", L"",
			MB_OK | MB_ICONHAND);
}

/******************************************************************************************/

bool DisplayMode::operator == (const DisplayMode& rhs)
{
	return (BitsPerPel == rhs.BitsPerPel &&
		PelsWidth == rhs.PelsWidth &&
		PelsHeight == rhs.PelsHeight &&
		DisplayFlags == rhs.DisplayFlags &&
		DisplayFrequency == rhs.DisplayFrequency);
}

/******************************************************************************************/
// used in std::sort

bool DisplayMode::operator < (const DisplayMode& rhs)
{
	if (PelsWidth < rhs.PelsWidth) return true;
	if (PelsWidth > rhs.PelsWidth) return false;

	if (PelsHeight < rhs.PelsHeight) return true;
	if (PelsHeight > rhs.PelsHeight) return false;

	if (BitsPerPel < rhs.BitsPerPel) return true;
	if (BitsPerPel > rhs.BitsPerPel) return false;

	if (DisplayFrequency < rhs.DisplayFrequency) return true;
	if (DisplayFrequency > rhs.DisplayFrequency) return false;

	return Name < rhs.Name;
}


/******************************************************************************************/

std::wstring DisplayMode::GetTemporaryName()
{
	wchar_t buf[100];
	_stprintf_s(buf, L"%dx%dx%d (%dHz)",
		PelsWidth,
		PelsHeight,
		BitsPerPel,
		DisplayFrequency);

	return buf;
}

/******************************************************************************************/
