#pragma once

#include <Windows.h>
#include <mfidl.h>
#include <string>
#include <mfapi.h>
#include <vector>
#include <wx/arrstr.h>
#include <opencv2/core/mat.hpp>
#include <wx/bitmap.h>


class Helpers
{
public:
	static wxArrayString wxArrStrFromVector(std::vector<std::wstring>vector);
	static wxStandardID getVideoDeviceNames(std::vector<wchar_t*>& deviceNames);

	template <class T> static void SafeRelease(T** ppT);
};

