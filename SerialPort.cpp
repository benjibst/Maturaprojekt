#include "SerialPort.h"
#include <vector>

bool SerialPort::OpenPort(std::string portName)
{
	hSerial = CreateFileA(portName.c_str(),
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hSerial == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			return false;
		return false;
	}
	SetSerialParams();
	return true;
}
bool SerialPort::OpenPort(unsigned long portIndex)
{
	char portName[5] = "COM";
	portName[3] = (char)portIndex + 48;
	portName[4] = 0;

	hSerial = CreateFileA(portName,
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hSerial == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			return false;
		return false;
	}
	SetSerialParams();
	return true;
}

bool SerialPort::SetSerialParams()
{
	serialParams = DCB{ 0 };
	if (!GetCommState(hSerial, &serialParams))
		return false;

	serialParams.BaudRate = CBR_9600;
	serialParams.ByteSize = 8;
	serialParams.StopBits = ONESTOPBIT;
	serialParams.Parity = NOPARITY;
	serialParams.fDtrControl = DTR_CONTROL_ENABLE;
	if (!SetCommState(hSerial, &serialParams))
		return false;

	commTimeOuts.ReadIntervalTimeout = 50;
	commTimeOuts.ReadTotalTimeoutConstant = 50;
	commTimeOuts.ReadTotalTimeoutMultiplier = 10;
	commTimeOuts.WriteTotalTimeoutConstant = 50;
	commTimeOuts.WriteTotalTimeoutMultiplier = 10;
	if (!SetCommTimeouts(hSerial, &commTimeOuts))
		return false;

	PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);

	open = true;
	return true;
}

bool SerialPort::Write(unsigned char* data, int length)
{
	DWORD dwBytesRead = 0;
	if (!WriteFile(hSerial, data, length, &dwBytesRead, NULL))
		return false;
	return true;
}

std::string SerialPort::Read()
{
	char szBuff[10] = { 0 };
	char sz1Buff[10] = { 0 };
	DWORD dwBytesRead = 0;

	bool ret = ReadFile(hSerial, szBuff, 5, &dwBytesRead, NULL);
	ReadFile(hSerial, sz1Buff, 5, &dwBytesRead, NULL);
	return std::string(szBuff);

}

std::vector<unsigned long> SerialPort::FindAvailableComPorts()
{
	ULONG portIDs[10];
	ULONG count;
	if (!GetCommPorts(portIDs, sizeof(portIDs), &count))
		return std::vector<ULONG>(portIDs,portIDs+count);
	return std::vector<ULONG>();
}

void SerialPort::Close()
{
	CloseHandle(hSerial);
}
