#include "common.h"


struct DisplayMode
{
	DWORD BitsPerPel;
	DWORD PelsWidth;
	DWORD PelsHeight;
	DWORD DisplayFlags;
	DWORD DisplayFrequency;
	std::wstring Name;


	DisplayMode() {}
	DisplayMode(DWORD bits, DWORD cx, DWORD cy, DWORD flags, DWORD freq, const std::wstring& name)
		: BitsPerPel(bits), PelsWidth(cx), PelsHeight(cy), DisplayFlags(flags),
		DisplayFrequency(freq), Name(name) { }

	bool operator == (const DisplayMode& rhs);
	bool operator < (const DisplayMode& rhs);
	bool operator != (const DisplayMode& rhs) { return !(*this == rhs); }

	void Get();
	void Set();

	std::wstring GetTemporaryName();

	const std::wstring& GetName() { return Name; }
	void SetName(const std::wstring& name) { Name = name; }
};

