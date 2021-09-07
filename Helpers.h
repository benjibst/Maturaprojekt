#pragma once
#include <mfapi.h>
#include <string>
#include <mfidl.h>
#include <Windows.h>
#include <vector>
#include <wx/arrstr.h>


class Helpers
{
public:
	static wxArrayString wxArrStrFromVector(std::vector<std::wstring>vector);
	static std::vector<std::wstring> getVideoDeviceNames();
	template <class T> static void SafeRelease(T** ppT);
};

