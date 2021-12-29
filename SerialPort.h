#pragma once
#include <Windows.h>
#include <string>
#include <vector>
class SerialPort
{
private:
	HANDLE hSerial;
	DCB serialParams;
	COMMTIMEOUTS commTimeOuts;
	bool open = false;
public:
	bool OpenPort(std::string portName);
	bool OpenPort(unsigned long portIndex);
	bool SetSerialParams();
	bool Write(unsigned char* data,int length);
	std::string Read();
	static std::vector<unsigned long> FindAvailableComPorts();
	void Close();
};

