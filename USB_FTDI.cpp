
#include <string>
#include <chrono>

#include "measure2.h"
#include "USB_FTDI.h"

using namespace std::chrono;
using namespace std;




USB_FTDI_Channel::USB_FTDI_Channel(uint32_t FTD_TxBuffer_Size_Init, uint32_t FTD_RxBuffer_Size_Init)
{
	NORMAL_MODE = true;
	Mask = 0xff;
	ftStatus = FT_Open(0, &ftHandle);

	FTD_TxBuffer = new char[FTD_TxBuffer_Size_Init];
	FTD_RxBuffer = new char[FTD_RxBuffer_Size_Init];

	FTD_TxBuffer_Size = FTD_TxBuffer_Size_Init;
	FTD_RxBuffer_Size = FTD_RxBuffer_Size_Init;

	if (ftStatus != FT_OK)
	{
		// FT_Open failed return;
		printf("FT_Open FAILED! \r\n");
		exit(0);
	}
	else
	{
		printf("made it #1");
		//set interface into FT245 Synchronous FIFO mode
		Mode = 0x00; //reset mode
		ftStatus = FT_SetBitMode(ftHandle, Mask, Mode);
		Mode = 0x40; //Sync FIFO mode
		ftStatus = FT_SetBitMode(ftHandle, Mask, Mode);
		if (ftStatus != FT_OK)
			printf("FT_SetBitMode FAILED! \r\n"); // FT_SetBitMode FAILED!
		FT_SetLatencyTimer(ftHandle, 1);
		FT_SetUSBParameters(ftHandle, 0x10000, 0x10000);
		FT_SetFlowControl(ftHandle, FT_FLOW_RTS_CTS, 0x0, 0x0);
		FT_Purge(ftHandle, FT_PURGE_RX);

		ftStatus = FT_GetStatus(ftHandle, &RxBytes, &TxBytes, &EventDWord);
		FT_Purge(ftHandle, FT_PURGE_RX);
	}
};

void USB_FTDI_Channel::FTDI_Load_TxBuffer(char *TxBuffer_In)
{
	FTD_TxBuffer = TxBuffer_In;
	// cout << "Tx "  << (int)TxBuffer_In[0]  << "  "  << (int)*(TxBuffer_In + 1)  << "   "   <<   (int)*(TxBuffer_In + 10)  << endl ;
	// cout << "Tx "  << (int)FTD_TxBuffer[0]  << "  "  << (int)*(FTD_TxBuffer + 1)  << "   "   <<   (int)*(FTD_TxBuffer + 10)  << endl ;
}

void USB_FTDI_Channel::FTDI_Rx(uint32_t &Bytes_Read)
{
	Bytes_Read = 0;
	FT_GetQueueStatus(ftHandle, &RxBytes);
	if (RxBytes > 0)
	{
		// check for too many bytes in que for local buffer
		if (RxBytes > FTD_RxBuffer_Size)
			ftStatus = FT_Read(ftHandle, FTD_RxBuffer, FTD_RxBuffer_Size, &BytesReceived);
		else
			ftStatus = FT_Read(ftHandle, FTD_RxBuffer, RxBytes, &BytesReceived);
		Bytes_Read = RxBytes;
	}
};

void USB_FTDI_Channel::FTDI_Tx(char *TxBuffer, uint32_t &How_Many_Bytes_Written)
{
	How_Many_Bytes_Written = 0;
	ftStatus = FT_GetStatus(ftHandle, &RxBytes, &TxBytes, &EventDWord);
	if ((ftStatus == FT_OK) && (TxBytes == 0))
		ftStatus = FT_Write(ftHandle, TxBuffer, FTD_TxBuffer_Size, &BytesWritten);
	How_Many_Bytes_Written = BytesWritten;
};

// a pre-loaded TxBuffer version for threading
void USB_FTDI_Channel::FTDI_Txx(void)
{
	// cout << "Tx " << (int)FTD_TxBuffer[1]  << " "  << (int)FTD_TxBuffer[10]  << endl ;
	ftStatus = FT_GetStatus(ftHandle, &RxBytes, &TxBytes, &EventDWord);
	if ((ftStatus == FT_OK) && (TxBytes == 0))
		ftStatus = FT_Write(ftHandle, FTD_TxBuffer, FTD_TxBuffer_Size, &BytesWritten);
	FTD_Bytes_Written = BytesWritten;
};
