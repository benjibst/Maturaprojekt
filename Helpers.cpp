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

std::vector<std::wstring> Helpers::getVideoDeviceNames()
{
	std::vector<std::wstring> deviceNames;
	HRESULT hr;
	uint32_t videoDevicesCount;
	uint32_t nameLength = 0;
	wchar_t* deviceName;
	IMFActivate** pMFVideoDevicesArray = NULL;
	IMFAttributes* pAttributes = NULL;
	// Create an attribute store to specify the enumeration parameters.
	hr = MFCreateAttributes(&pAttributes, 1);

	// Source type: video capture devices
	hr = pAttributes->SetGUID(
		MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
		MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
	);

	// Enumerate devices.

	hr = MFEnumDeviceSources(pAttributes, &pMFVideoDevicesArray, &videoDevicesCount);

	for (UINT32 i = 0; i < videoDevicesCount; i++)
	{
		hr = pMFVideoDevicesArray[i]->GetAllocatedString(
			MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME,
			&deviceName, &nameLength);
		if (SUCCEEDED(hr) && deviceName != NULL)
			deviceNames.push_back(std::wstring(deviceName, nameLength));
	}

	SafeRelease(&pAttributes);
	for (int i = 0; i < videoDevicesCount; i++)
		SafeRelease(&pMFVideoDevicesArray[i]);

	if (hr == S_OK)
		return deviceNames;
	else
		return std::vector<std::wstring>();


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
