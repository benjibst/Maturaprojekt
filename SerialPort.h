#pragma once
#include <Windows.h>
#include <vector>
#include "CoordParams.h"

class MCUConn
{
private:
	HANDLE hSerial;
	DCB serialParams;
	COMMTIMEOUTS commTimeOuts;
	bool SetSerialParams();
public:
	bool open = false;
	bool OpenPort(char* portName);
	bool OpenPort(unsigned long portIndex);
	bool WriteData(unsigned char* data,int length);
	bool Read(unsigned char* buf, int nobtr);
	bool SendCoordData(std::vector<cv::Point2f>& points);
	void SetManMode();
	void SetAutoMode();
	static std::vector<unsigned long> FindAvailableComPorts();
	void Close();
};

