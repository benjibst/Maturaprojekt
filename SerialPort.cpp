#include "SerialPort.h"

int SerialPort::Open(std::string Port)
{
	hSerial = CreateFileA(static_cast<LPCSTR>(portname),
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hSerial == INVALID_HANDLE_VALUE)
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			return 1;
		return -1;

	serialParams.DCBlength = sizeof(serialParams);
	if (!GetCommState(hSerial, &serialParams)) 
		return -1;
	serialParams.BaudRate = CBR_9600;
	serialParams.ByteSize = 8;
	serialParams.StopBits = ONESTOPBIT;
	serialParams.Parity = NOPARITY;
	if (!SetCommState(hSerial, &serialParams))
		return -1;

	commTimeOuts.ReadIntervalTimeout = 50;
	commTimeOuts.ReadTotalTimeoutConstant = 50;
	commTimeOuts.ReadTotalTimeoutMultiplier = 10;
	commTimeOuts.WriteTotalTimeoutConstant = 50;
	commTimeOuts.WriteTotalTimeoutMultiplier = 10;
	if (!SetCommTimeouts(hSerial, &commTimeOuts))
		return -1;

	open = true;
	return 0;
}

void SerialPort::WriteLine(std::string data)
{
	char szBuff[] = "PD2=1";
	DWORD dwBytesRead = 0;
	if (!WriteFile(hSerial, szBuff, sizeof(szBuff), &dwBytesRead, NULL)) {
	}

}

std::string SerialPort::ReadLine()
{
	char szBuff[5];
	DWORD dwBytesRead = 0;
	if (WriteFile(hSerial, szBuff, sizeof(szBuff), &dwBytesRead, NULL))
		return std::string(szBuff);
	return std::string();
}

void SerialPort::Close()
{
	CloseHandle(hSerial);
}
