#pragma once
#include <mfapi.h>
#include <string>
#include <mfobjects.h>
#include <mfidl.h>
#include <evr.h>
#include <Windows.h>

using std::wstring;

class MediaFoundationLib
{
public:
	MediaFoundationLib();
	void RepaintVideo();
	void Dispose();
	HRESULT InitMFPreview(HWND* hWnd);
private:
	IMFMediaSource* pMFMediaSource; //pointerMediaFoundationMediaSource
	IMFPresentationDescriptor* pMFPresentationDescriptor;
	IMFStreamDescriptor* pMFStreamDescriptor;
	IMFMediaSession* pMFMediaSession;
	IMFVideoDisplayControl* pMFVideoDisplayControl;
	IMFGetService* pMFGetService;
	IMFTopology* pMFTopology;
	IMFTopologyNode* pMFTopologySource;
	IMFTopologyNode* pMFTopologyOutput;
	IMFActivate* pMFActivate;

	HRESULT CreateVideoDeviceSource(IMFMediaSource** ppMFMS);
	HRESULT AddSourceNode(IMFTopology* pTopology, IMFMediaSource* pSource, IMFPresentationDescriptor* pPD, IMFStreamDescriptor* pSD, IMFTopologyNode** ppNode);
	HRESULT AddOutputNode(IMFTopology* pTopology, IMFActivate* pActivate, DWORD dwId, IMFTopologyNode** ppNode);
	void DebugShowDeviceNames(IMFActivate** ppDevices, UINT count);
	template <class T> void SafeRelease(T** ppT);
};



