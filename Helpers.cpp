#include "Helpers.h"

wxArrayString Helpers::wxArrStrFromVector(std::vector<wchar_t*> vec)
{
	wxArrayString deviceDisplayNames;
	if (vec.empty())
		return deviceDisplayNames;
	for (int i = 0; i < vec.size(); i++)
		deviceDisplayNames.Add(vec[i]);
	return deviceDisplayNames;
}

HRESULT Helpers::getVideoDeviceNames(std::vector<wchar_t*>& deviceNames)
{
	unsigned int videoDevicesCount;
	unsigned int nameLength = 0;
	wchar_t* deviceName = 0;
	HRESULT result;
	IMFActivate** pMFVideoDevicesArray = NULL;
	IMFAttributes* pAttributes = NULL;
	// Create an attribute store to specify the enumeration parameters.
	result = MFCreateAttributes(&pAttributes, 1);


	// Source type: video capture devices
	result = pAttributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);

	// Enumerate devices.
	result = MFEnumDeviceSources(pAttributes, &pMFVideoDevicesArray, &videoDevicesCount);

	for (UINT32 i = 0; i < videoDevicesCount; i++)
	{
		result = pMFVideoDevicesArray[i]->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, &deviceName, &nameLength);
		deviceNames.push_back(deviceName);
	}
	SafeRelease(&pAttributes);
	for (unsigned int i = 0; i < videoDevicesCount; i++)
		SafeRelease(&pMFVideoDevicesArray[i]);
	CoTaskMemFree(pMFVideoDevicesArray);
	return result;
}

template <class T>
static void Helpers::SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}
