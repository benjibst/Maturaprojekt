#include "Helpers.h"

wxArrayString Helpers::wxArrStrFromVector(std::vector<std::wstring> vec)
{
	wxArrayString deviceDisplayNames;
	if (!vec.empty())
	{
		for (int i = 0; i < vec.size(); i++)
			deviceDisplayNames.Add(vec[i]);
	}
	return deviceDisplayNames;
}

wxStandardID Helpers::getVideoDeviceNames(std::vector<wchar_t*>& deviceNames)
{
	uint32_t videoDevicesCount;
	uint32_t nameLength = 0;
	wchar_t* deviceName;

	IMFActivate** pMFVideoDevicesArray = NULL;
	IMFAttributes* pAttributes = NULL;
	// Create an attribute store to specify the enumeration parameters.
	if (MFCreateAttributes(&pAttributes, 1) != S_OK)
		goto RELEASE;

	// Source type: video capture devices
	if (pAttributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID) != S_OK)
		goto RELEASE;

	// Enumerate devices.
	if (MFEnumDeviceSources(pAttributes, &pMFVideoDevicesArray, &videoDevicesCount) != S_OK)
		goto RELEASE;

	for (UINT32 i = 0; i < videoDevicesCount; i++)
	{
		if (pMFVideoDevicesArray[i]->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, &deviceName, &nameLength) != S_OK)
			return wxID_CANCEL;
		deviceNames.push_back(deviceName);
	}
	return wxID_OK;

	RELEASE:
	SafeRelease(&pAttributes);
	for (unsigned int i = 0; i < videoDevicesCount; i++)
		SafeRelease(&pMFVideoDevicesArray[i]);
	return wxID_CANCEL;
}

bool Helpers::ConvertMatBitmapTowxBitmapMSW(cv::Mat& matBitmap, wxBitmap& bitmap)
{
	wxCHECK(!matBitmap.empty(), false);
	wxCHECK(matBitmap.type() == CV_8UC3, false);
	wxCHECK(matBitmap.dims == 2, false);
	wxCHECK(bitmap.IsOk(), false);
	wxCHECK(bitmap.GetWidth() == matBitmap.cols && bitmap.GetHeight() == matBitmap.rows, false);
	wxCHECK(bitmap.GetDepth() == 24, false);

	if (!(bitmap.IsDIB()
		&& matBitmap.isContinuous()
		&& matBitmap.cols % 4 == 0))
		return false;
	else
	{
		const HDC  hScreenDC = ::GetDC(nullptr);
		BITMAPINFO bitmapInfo{ 0 };
		bool       success;

		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFO) - sizeof(RGBQUAD);
		bitmapInfo.bmiHeader.biWidth = bitmap.GetWidth();
		bitmapInfo.bmiHeader.biHeight = 0 - bitmap.GetHeight();
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biBitCount = 24;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;

		success = ::SetDIBits(hScreenDC, bitmap.GetHBITMAP(), 0, bitmap.GetHeight(),
			matBitmap.data, &bitmapInfo, DIB_RGB_COLORS) != 0;
		::ReleaseDC(nullptr, hScreenDC);

		return success;
	}
}

template <class T> static void Helpers::SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}
