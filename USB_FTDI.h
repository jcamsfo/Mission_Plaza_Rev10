#ifndef USB_FTDI_H
#define USB_FTDI_H
#pragma once

#include <iostream>

#include "ftd2xx.h"
#include "WinTypes.h"


using namespace std;

class USB_FTDI_Channel
{
public:
	USB_FTDI_Channel(uint32_t FTD_TxBuffer_Size_Init, uint32_t FTD_RxBuffer_Size_Init);
	void FTDI_Tx(char *TxBuffer, uint32_t &How_Many_Bytes_Written);
	void FTDI_Txx(void);
	void FTDI_Rx(uint32_t &Bytes_Read);

	char *FTD_TxBuffer;
	void FTDI_Load_TxBuffer(char *TxBuffer_In);

	char *FTD_RxBuffer;

	uint32_t FTD_TxBuffer_Size;
	uint32_t FTD_RxBuffer_Size;

	uint32_t FTD_Bytes_Written;

	DWORD dwError, dwPriClass;
	DWORD bytes_written;
	FT_HANDLE ftHandle;
	FT_STATUS ftStatus;
	UCHAR Mask;
	UCHAR Mode;
	DWORD EventDWord;
	DWORD RxBytes;
	DWORD TxBytes;
	DWORD BytesWritten;
	DWORD BytesReceived;
	bool NORMAL_MODE;
};

#endif /* USB_FTDI_H */