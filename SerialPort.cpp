#include "SerialPort.h"

bool MCUConn::OpenPort(char* portName)
{
	hSerial = CreateFileA(portName,
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hSerial == INVALID_HANDLE_VALUE)
		return false;
	if (!SetSerialParams())
		return false;
	open = true;
	return true;
}

bool MCUConn::OpenPort(unsigned long portIndex)
{
	char portName[6];
	sprintf(portName, "COM%d", portIndex);
	return this->OpenPort(portName);
}

bool MCUConn::SetSerialParams()
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

	return PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
}

bool MCUConn::WriteData(unsigned char* data, int length)
{
	DWORD dwBytesRead = 0;
	return WriteFile(hSerial, data, length, &dwBytesRead, NULL);
}

bool MCUConn::Read(unsigned char* buf,int nobtr,DWORD* bytesRead)
{
	return ReadFile(hSerial, buf, nobtr, bytesRead, NULL);
}

bool MCUConn::SendCoordData(std::vector<cv::Point2f>& points)
{
	std::vector<unsigned char> data = CoordParams::ParamDataFromCoordinates(points);
	return WriteData(data.data(), (int)data.size());
}

std::vector<unsigned long> MCUConn::FindAvailableComPorts()
{
	ULONG portIDs[10];
	ULONG count;
	if (!GetCommPorts(portIDs, sizeof(portIDs), &count))
		return std::vector<ULONG>(portIDs,portIDs+count);
	return std::vector<ULONG>();
}

void MCUConn::Close()
{
	CloseHandle(hSerial);
}
