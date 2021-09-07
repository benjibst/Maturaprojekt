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

	if(hr==S_OK)
		return deviceNames;
	else
		return std::vector<std::wstring>();

	
}

template <class T> static void Helpers::SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}
