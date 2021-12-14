#include "SerialPort.h"

int SerialPort::Open(std::string Port)
{
	hSerial = CreateFileA("COM5",
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hSerial == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			return 1;
		return -1;
	}
		
	serialParams = DCB{ 0 };
	if (!GetCommState(hSerial, &serialParams)) 
		return -1;
	serialParams.BaudRate = CBR_9600;
	serialParams.ByteSize = 8;
	serialParams.StopBits = ONESTOPBIT;
	serialParams.Parity = NOPARITY;
	serialParams.fDtrControl = DTR_CONTROL_ENABLE;
	if (!SetCommState(hSerial, &serialParams))
		return -1;

	PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);

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
	char szBuff[] = "PD2=\n";
	DWORD dwBytesRead = 0;
	WriteFile(hSerial, szBuff, sizeof(szBuff), &dwBytesRead, NULL);
	int i = GetLastError();
}

std::string SerialPort::ReadLine()
{
	char szBuff[6]={0};
	DWORD dwBytesRead = 0;

	bool ret = ReadFile(hSerial, szBuff, 6, &dwBytesRead, NULL);
	return std::string(szBuff);

}

void SerialPort::Close()
{
	CloseHandle(hSerial);
}
