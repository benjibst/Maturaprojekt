#pragma once
#include <Windows.h>
#include <string>
#include <thread>
class SerialPort
{
private:
	HANDLE hSerial;
	DCB serialParams;
	COMMTIMEOUTS commTimeOuts;
	bool open = false;
	const char* portname = "\\\\.\\COM";
public:
	int Open(std::string Port);
	void WriteLine(std::string data);
	std::string ReadLine();
	void Close();
};

