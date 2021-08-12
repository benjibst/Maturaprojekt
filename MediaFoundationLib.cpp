#include "MediaFoundationLib.h"

MediaFoundationLib::MediaFoundationLib()
{
	pMFMediaSource = NULL;
	pMFPresentationDescriptor = NULL;
	pMFStreamDescriptor = NULL;
	pMFMediaSession = NULL;
	pMFTopology = NULL;
	pMFTopologySource = NULL;
	pMFTopologyOutput = NULL;
	pMFActivateEVR = NULL;
	pMFGetService = NULL;
	pMFVideoDisplayControl = NULL;
	devices = NULL;
	videoDevicesCount = 0;
}

VOID MediaFoundationLib::RepaintVideo()
{
	if (pMFVideoDisplayControl)
		pMFVideoDisplayControl->RepaintVideo();
}

HRESULT MediaFoundationLib::GetVideoDevices()
{
	devices = NULL;
	IMFAttributes* pAttributes = NULL;
	// Create an attribute store to specify the enumeration parameters.
	HRESULT hr = MFCreateAttributes(&pAttributes, 1);
	if (FAILED(hr))
		return hr;

	// Source type: video capture devices
	hr = pAttributes->SetGUID(
		MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
		MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
	);
	if (FAILED(hr))
		return hr;

	// Enumerate devices.

	hr = MFEnumDeviceSources(pAttributes, &devices, &videoDevicesCount);
	if (FAILED(hr))
		return hr;
	UINT32 nameLength = 0;
	LPWSTR deviceName;
	for (UINT32 i = 0; i < videoDevicesCount; i++)
	{
		hr = devices[i]->GetAllocatedString(
			MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME,
			&deviceName, &nameLength);
		if (SUCCEEDED(hr))
			deviceNames.push_back(deviceName);
	}
	return hr;
}

std::vector<LPWSTR> MediaFoundationLib::GetDeviceNames()
{
	return deviceNames;
}

HRESULT MediaFoundationLib::CreateVideoDeviceSource(IMFMediaSource** ppMFMS, IMFActivate* device)
{
	HRESULT hr;
	*ppMFMS = NULL;

	IMFMediaSource* pSource = NULL;

	DebugShowDeviceNames(&device, 1);


	// Create the media source object.
	hr = device->ActivateObject(IID_PPV_ARGS(&pSource));
	if (FAILED(hr))
		return hr;

	*ppMFMS = pSource;
	(*ppMFMS)->AddRef();
	return hr;
}

VOID MediaFoundationLib::DebugShowDeviceNames(IMFActivate** ppDevices, UINT count)
{
	for (DWORD i = 0; i < count; i++)
	{
		HRESULT hr = S_OK;
		WCHAR* szFriendlyName = NULL;

		// Try to get the display name.
		UINT32 cchName;
		hr = ppDevices[i]->GetAllocatedString(
			MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME,
			&szFriendlyName, &cchName);

		if (SUCCEEDED(hr))
		{
			OutputDebugString(szFriendlyName);
			OutputDebugString(L"\n");
		}
		CoTaskMemFree(szFriendlyName);
	}
}

HRESULT MediaFoundationLib::AddSourceNode(IMFTopology* pTopology, IMFMediaSource* pSource, IMFPresentationDescriptor* pPD, IMFStreamDescriptor* pSD, IMFTopologyNode** ppNode)
{
	IMFTopologyNode* pNode = NULL;

	// Create the node.
	HRESULT hr = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	// Set the attributes.
	hr = pNode->SetUnknown(MF_TOPONODE_SOURCE, pSource);
	if (FAILED(hr))
	{
		goto done;
	}

	hr = pNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pPD);
	if (FAILED(hr))
	{
		goto done;
	}

	hr = pNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pSD);
	if (FAILED(hr))
	{
		goto done;
	}

	// Add the node to the topology.
	hr = pTopology->AddNode(pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	// Return the pointer to the caller.
	*ppNode = pNode;
	(*ppNode)->AddRef();

done:
	SafeRelease(&pNode);
	return hr;
}

HRESULT MediaFoundationLib::AddOutputNode(IMFTopology* pTopology, IMFActivate* pActivate, DWORD dwId, IMFTopologyNode** ppNode)
{
	IMFTopologyNode* pNode = NULL;

	// Create the node.
	HRESULT hr = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	// Set the object pointer.
	hr = pNode->SetObject(pActivate);
	if (FAILED(hr))
	{
		goto done;
	}

	// Set the stream sink ID attribute.
	hr = pNode->SetUINT32(MF_TOPONODE_STREAMID, dwId);
	if (FAILED(hr))
	{
		goto done;
	}

	hr = pNode->SetUINT32(MF_TOPONODE_NOSHUTDOWN_ON_REMOVE, FALSE);
	if (FAILED(hr))
	{
		goto done;
	}

	// Add the node to the topology.
	hr = pTopology->AddNode(pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	// Return the pointer to the caller.
	*ppNode = pNode;
	(*ppNode)->AddRef();

done:
	SafeRelease(&pNode);
	return hr;
}



HRESULT MediaFoundationLib::InitMFPreview(HWND* hWnd, IMFActivate* videoSource)
{
	HRESULT hr;
	pMFMediaSource = NULL;
	pMFPresentationDescriptor = NULL;
	pMFStreamDescriptor = NULL;
	pMFMediaSession = NULL;
	pMFTopology = NULL;
	pMFTopologySource = NULL;
	pMFTopologyOutput = NULL;
	pMFActivateEVR = NULL;
	pMFGetService = NULL;
	pMFVideoDisplayControl = NULL;

	hr = CreateVideoDeviceSource(&pMFMediaSource, videoSource);
	if (FAILED(hr))
		return hr;

	hr = pMFMediaSource->CreatePresentationDescriptor(&pMFPresentationDescriptor);
	if (FAILED(hr))
		return hr;

	BOOL select = false;
	hr = pMFPresentationDescriptor->GetStreamDescriptorByIndex(0, &select, &pMFStreamDescriptor);
	if (FAILED(hr))
		return hr;

	hr = MFCreateMediaSession(NULL, &pMFMediaSession);
	if (FAILED(hr))
		return hr;

	hr = MFCreateTopology(&pMFTopology);
	if (FAILED(hr))
		return hr;

	hr = AddSourceNode(pMFTopology, pMFMediaSource, pMFPresentationDescriptor, pMFStreamDescriptor, &pMFTopologySource);
	if (FAILED(hr))
		return hr;

	hr = MFCreateVideoRendererActivate(*hWnd, &pMFActivateEVR);
	if (FAILED(hr))
		return hr;

	hr = AddOutputNode(pMFTopology, pMFActivateEVR, 0, &pMFTopologyOutput);
	if (FAILED(hr))
		return hr;

	hr = pMFTopologySource->ConnectOutput(0, pMFTopologyOutput, 0);
	if (FAILED(hr))
		return hr;

	hr = pMFMediaSession->SetTopology(MFSESSION_SETTOPOLOGY_IMMEDIATE, pMFTopology);
	if (FAILED(hr))
		return hr;

	hr = pMFMediaSession->QueryInterface<IMFGetService>(&pMFGetService);
	if (FAILED(hr))
		return hr;

	Sleep(500);

	hr = pMFGetService->GetService(MR_VIDEO_RENDER_SERVICE, IID_IMFVideoDisplayControl, (VOID**)&pMFVideoDisplayControl);
	if (FAILED(hr))
		return hr;

	//SIZE szVideo = {};
	//SIZE arVideo = {};
	//hr = pMFVideoDisplayControl->GetNativeVideoSize(&szVideo, &arVideo);
	//if (FAILED(hr))
	//	return hr;

	//RECT targetRect = {};
	//targetRect.left = 50;
	//targetRect.right = szVideo.cx + targetRect.left;
	//targetRect.top = 50;
	//targetRect.bottom = szVideo.cy + targetRect.top;

	/*hr = pMFVideoDisplayControl->SetVideoPosition(NULL, &targetRect);
	if (FAILED(hr))
		return hr;*/

	hr = pMFVideoDisplayControl->SetVideoWindow(*hWnd);
	if (FAILED(hr))
		return hr;

	PROPVARIANT varStart;
	PropVariantInit(&varStart);
	hr = pMFMediaSession->Start(&GUID_NULL, &varStart);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

UINT MediaFoundationLib::GetVideoDeviceCount()
{
	return videoDevicesCount;
}

LPWSTR MediaFoundationLib::GetDeviceNames(int index)
{
	try
	{
		return deviceNames[index];
	}
	catch (const std::exception&)
	{
		return nullptr;
	}

}

VOID MediaFoundationLib::Dispose()
{
	if (pMFMediaSession)
	{
		pMFMediaSession->Stop();
		pMFMediaSession->ClearTopologies();
	}

	SafeRelease(&pMFMediaSource);
	SafeRelease(&pMFPresentationDescriptor);
	SafeRelease(&pMFStreamDescriptor);
	SafeRelease(&pMFMediaSession);
	SafeRelease(&pMFVideoDisplayControl);
	SafeRelease(&pMFGetService);
	SafeRelease(&pMFTopology);
	SafeRelease(&pMFTopologySource);
	SafeRelease(&pMFTopologyOutput);
	SafeRelease(&pMFActivateEVR);
}

template <class T> VOID MediaFoundationLib::SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}
