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

template <class T> static void Helpers::SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}
