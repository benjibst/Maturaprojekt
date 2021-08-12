#pragma once
#include <mfapi.h>
#include <string>
#include <mfobjects.h>
#include <mfidl.h>
#include <evr.h>
#include <Windows.h>
#include <vector>

class MediaFoundationLib
{
public:
	MediaFoundationLib();
	VOID RepaintVideo();
	VOID Dispose();
	HRESULT InitMFPreview(HWND* hWnd, IMFActivate* videoSource);
	UINT GetVideoDeviceCount();
	WCHAR* GetDeviceNames(int index);
	HRESULT GetVideoDevices();
	std::vector<LPWSTR> GetDeviceNames();
private:
	IMFMediaSource* pMFMediaSource; 
	IMFPresentationDescriptor* pMFPresentationDescriptor;
	IMFStreamDescriptor* pMFStreamDescriptor;
	IMFMediaSession* pMFMediaSession;
	IMFVideoDisplayControl* pMFVideoDisplayControl;
	IMFGetService* pMFGetService;
	IMFTopology* pMFTopology;
	IMFTopologyNode* pMFTopologySource;
	IMFTopologyNode* pMFTopologyOutput;
	IMFActivate* pMFActivateEVR;
	IMFActivate** devices;
	std::vector<LPWSTR> deviceNames;
	UINT32 videoDevicesCount;

	HRESULT CreateVideoDeviceSource(IMFMediaSource** ppMFMS, IMFActivate* device);
	HRESULT AddSourceNode(IMFTopology* pTopology, IMFMediaSource* pSource, IMFPresentationDescriptor* pPD, IMFStreamDescriptor* pSD, IMFTopologyNode** ppNode);
	HRESULT AddOutputNode(IMFTopology* pTopology, IMFActivate* pActivate, DWORD dwId, IMFTopologyNode** ppNode);
	VOID DebugShowDeviceNames(IMFActivate** ppDevices, UINT count);
	template <class T> VOID SafeRelease(T** ppT);
};



