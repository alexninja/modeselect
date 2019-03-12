#include "common.h"


/******************************************************************************************/

struct DiskRep
{
	DWORD BitsPerPel;
	DWORD PelsWidth;
	DWORD PelsHeight;
	DWORD DisplayFlags;
	DWORD DisplayFrequency;
	wchar_t Name[NAME_LIMIT + 1];
};


/******************************************************************************************/

PCTSTR FileName()
{
	static wchar_t buf[MAX_PATH + 100];
	static wchar_t const* result = NULL;

	if (result == NULL)
	{
		SHGetSpecialFolderPath(g_hwnd, buf, CSIDL_APPDATA, FALSE);
		_tcscat(buf, L"\\modeselect.ini");
		result = buf;
	}

	return result;
}


/******************************************************************************************/

void Save()
{
	HANDLE file = CreateFile(FileName(), GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD bytes_written;
	BOOL success;
	BYTE no_of_records;

	if (g_modes.size() > 255)
		goto error;

	no_of_records = (BYTE)g_modes.size();

	success = WriteFile(file, &no_of_records, 1, &bytes_written, NULL);
	if (success == 0 || bytes_written != 1)
		goto error;


	for (int i = 0; i < no_of_records; i++)
	{
		DiskRep dr;
		dr.BitsPerPel = g_modes[i].BitsPerPel;
		dr.PelsWidth = g_modes[i].PelsWidth;
		dr.PelsHeight = g_modes[i].PelsHeight;
		dr.DisplayFlags = g_modes[i].DisplayFlags;
		dr.DisplayFrequency = g_modes[i].DisplayFrequency;
		_tcsncpy(dr.Name, g_modes[i].GetName().c_str(), NAME_LIMIT);
		dr.Name[NAME_LIMIT] = '\0';

		success = WriteFile(file, &dr, sizeof(DiskRep), &bytes_written, NULL);
		if (success == 0 || bytes_written != sizeof(DiskRep))
			goto error;
	}

	CloseHandle(file);
	return;


error:
	CloseHandle(file);
	MessageBox(NULL, L"Failed to save to file", L"video mode selector", MB_OK | MB_ICONHAND);
}


/******************************************************************************************/

void Load()
{
	HANDLE file = CreateFile(FileName(), GENERIC_READ,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file == INVALID_HANDLE_VALUE)
		return; // no file yet, start off with empty list


	DWORD bytes_read;
	BOOL success;

	BYTE no_of_records;

	success = ReadFile(file, &no_of_records, 1, &bytes_read, NULL);
	if (success == 0 || bytes_read != 1)
		goto error;


	for (int i = 0; i < no_of_records; i++)
	{
		DiskRep dr;

		success = ReadFile(file, &dr, sizeof(DiskRep), &bytes_read, NULL);
		if (success == 0 || bytes_read != sizeof(DiskRep))
			goto error;

		dr.Name[NAME_LIMIT] = '\0';

		DisplayMode dm(
			dr.BitsPerPel,
			dr.PelsWidth,
			dr.PelsHeight,
			dr.DisplayFlags,
			dr.DisplayFrequency,
			dr.Name);

		g_modes.push_back(dm);
	}

	CloseHandle(file);
	return;



error:
	CloseHandle(file);
	MessageBox(NULL, L"Failed to read from file", L"video mode selector", MB_OK | MB_ICONHAND);
}


/******************************************************************************************/


