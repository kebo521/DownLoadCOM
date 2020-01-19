/************************************************************ 
  Copyright (C),   szzt Tech. Co., Ltd. 
  FileName:    Pdkbdoc.cpp
  Author:      chenbin
  Version :    1.0        Date: 2012/03/31
  Description: ʵ���������ж�̬��ӿڵĸ������ܽӿں���  
  Version:     v1.0
  
 ************************************************************/

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "comm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define   TIMEOUT      1
#define   I_DELAYTIME  20 //д���ڵ�ʱ����Ҫ��ʱ20���� 
#define COM_OK          0   //���سɹ� �޸�ʱ��20120225 �±�
#define COM_OPEN_ERR    -1  //�򿪴��ڴ�
#define COM_WRITE_ER    -2  //д���ڴ�
#define COM_READ_ERR    -3  //�����ڴ�
#define COM_CLOSE_ER    -4  //�رմ��ڴ�
#define COM_PIDPAD_ER   -5  //�豸���ر���
#define COM_ERRPAR      -11 //���������
#define COM_ERROR		-12 //ϵͳ�ײ㱨��
#define COM_TIMEOUT		-13 //��ȡ��Ϣ��ʱ


CPdkbdoc::CPdkbdoc()
{
	hDev = NULL;
}

CPdkbdoc::~CPdkbdoc()
{
	if (hDev != NULL)
	{
		CloseCom();
		hDev = NULL;
	}
}


/*************************************************************
�������ƣ�OpenCom
����˵�����򿪴��� 
���������portName:���ں� 	 
		  m_baudrate :������
��������� 
����ֵ��   0�ɹ�������ʧ��
���ߣ�    chenbin
�޸�˵����
�޸�ʱ�䣺
**************************************************************/
int CPdkbdoc::OpenCom(const char *portName, const DWORD m_baudrate)
{
	DCB comconfig;
	// instance an object of COMMTIMEOUTS.
	COMMTIMEOUTS comTimeOut;
	if(hDev != NULL )
	{
		PurgeComm(hDev,PURGE_TXCLEAR|PURGE_RXCLEAR);
		CloseHandle(hDev);
		hDev=NULL;
	}
	
	Sleep(I_DELAYTIME*2);
	
	hDev = CreateFile(portName,								// Specify port device: default "COM1"
							 GENERIC_READ | GENERIC_WRITE,       // Specify mode that open device.
							 0,                                  // the devide isn't shared.
							 NULL,                               // the object gets a default security.
							 OPEN_EXISTING,                      // Specify which action to take on file. 
							 0,                                  // default.
							NULL);                              // default.
	if( hDev == INVALID_HANDLE_VALUE )
	{
		//Trace("CreateFile err=%d", GetLastError());
		hDev = NULL;
		return COM_ERROR;
	}
		
	PurgeComm(hDev,PURGE_TXCLEAR|PURGE_RXCLEAR);
	//�趨��������С
	BOOL err = SetupComm(hDev,1024,1024);
	if (err == FALSE)
	{
		//Trace("SetupComm failed err=%d", GetLastError());
		return COM_ERROR;
	}
	// Get current configuration of serial communication port.
	if (GetCommState(hDev,&comconfig) == 0)
	{
		//Trace("GetCommState failed err=%d", GetLastError());
		return COM_ERROR;
	}

	// constant parameter.
	comconfig.ByteSize = 8;
	comconfig.StopBits = ONESTOPBIT;
	comconfig.Parity = NOPARITY;
	comconfig.BaudRate = m_baudrate;

	// Set current configuration of serial communication port.
	if (SetCommState(hDev,&comconfig) == 0)
	{
		//Trace("SetCommState failed err=%d", GetLastError());
		return COM_ERROR;
	}
               
	// Specify time-out between charactor for receiving.
	comTimeOut.ReadIntervalTimeout = 10;
	// Specify value that is multiplied 
	// by the requested number of bytes to be read. 
	comTimeOut.ReadTotalTimeoutMultiplier = 3;
	// Specify value is added to the product of the 
	// ReadTotalTimeoutMultiplier member
	comTimeOut.ReadTotalTimeoutConstant = 2;
	// Specify value that is multiplied 
	// by the requested number of bytes to be sent. 
	comTimeOut.WriteTotalTimeoutMultiplier = 3;
	// Specify value is added to the product of the 
	// WriteTotalTimeoutMultiplier member
	comTimeOut.WriteTotalTimeoutConstant = 2;
	// set the time-out parameter into device control.
	SetCommTimeouts(hDev,&comTimeOut);
	// Updata port's status.
	strComName = portName;
	return COM_OK;      
}

void CPdkbdoc::SetComm(CString portName)
{
	strComName = portName;
}

int CPdkbdoc::SetupParam(DWORD BaudRate, BYTE Parity, BYTE ByteSize, BYTE StopBits)
{
	DCB comconfig;
	// instance an object of COMMTIMEOUTS.
	COMMTIMEOUTS comTimeOut;
	if (hDev != NULL)
	{
		PurgeComm(hDev, PURGE_TXCLEAR | PURGE_RXCLEAR);
		CloseHandle(hDev);
		hDev = NULL;
	}

	Sleep(I_DELAYTIME * 2);
	
	hDev = CreateFile(strComName,			// Specify port device: default "COM1"
		GENERIC_READ | GENERIC_WRITE,       // Specify mode that open device.
		0,                                  // the devide isn't shared.
		NULL,                               // the object gets a default security.
		OPEN_EXISTING,                      // Specify which action to take on file. 
		0,                                  // default.
		NULL);                              // default.
	if (hDev == INVALID_HANDLE_VALUE)
	{
		//Trace("CreateFile err=%d", GetLastError());
		hDev = NULL;
		return COM_ERROR;
	}

	PurgeComm(hDev, PURGE_TXCLEAR | PURGE_RXCLEAR);
	//�趨��������С
	BOOL err = SetupComm(hDev, 1024, 1024);
	if (err == FALSE)
	{
		//Trace("SetupComm failed err=%d", GetLastError());
		return COM_ERROR;
	}
	// Get current configuration of serial communication port.
	if (GetCommState(hDev, &comconfig) == 0)
	{
		//Trace("GetCommState failed err=%d", GetLastError());
		return COM_ERROR;
	}

	// constant parameter.
	comconfig.ByteSize = ByteSize;
	comconfig.StopBits = StopBits;
	comconfig.Parity =   Parity;
	comconfig.BaudRate = BaudRate;

	// Set current configuration of serial communication port.
	if (SetCommState(hDev, &comconfig) == 0)
	{
		//Trace("SetCommState failed err=%d", GetLastError());
		return COM_ERROR;
	}

	// Specify time-out between charactor for receiving.
	comTimeOut.ReadIntervalTimeout = 1;// 10;
	// Specify value that is multiplied 
	// by the requested number of bytes to be read. 
	comTimeOut.ReadTotalTimeoutMultiplier = 1;// 3;
	// Specify value is added to the product of the 
	// ReadTotalTimeoutMultiplier member
	comTimeOut.ReadTotalTimeoutConstant = 1;//2;
	// Specify value that is multiplied 
	// by the requested number of bytes to be sent. 
	comTimeOut.WriteTotalTimeoutMultiplier = 0;//3;
	// Specify value is added to the product of the 
	// WriteTotalTimeoutMultiplier member
	comTimeOut.WriteTotalTimeoutConstant = 0;//2;
	// set the time-out parameter into device control.
	SetCommTimeouts(hDev, &comTimeOut);
	// Updata port's status.
	return COM_OK;

}
/*************************************************************
�������ƣ�CloseCom
����˵�����رմ���
��������� 	   
��������� 
����ֵ��   0�ɹ�������ʧ��
���ߣ�    chenbin
�޸�˵����
�޸�ʱ�䣺
**************************************************************/
int CPdkbdoc::CloseCom()
{
	if (hDev != NULL)
	{
		if (!CloseHandle(hDev))
		{
			return COM_ERROR ;
		}
		hDev =NULL;
	}
	return COM_OK ;
}

/*************************************************************
�������ƣ�WriteCom
����˵�����򴮿�д����
���������buf:д�봮�ڵ���������   
		  len:д��Ĵ������ݳ���
�����������
����ֵ��   0�ɹ�������ʧ��
���ߣ�    chenbin
�޸�˵����
�޸�ʱ�䣺
**************************************************************/
int CPdkbdoc::WriteCom(const BYTE *buf, DWORD len )
{
	unsigned long length=0,i ;
	if(hDev== NULL)
	{
		//Trace("WriteCom hDev is NULL");
		return COM_ERROR ;
	}
	//HexTrace("WriteCom:", (char *)buf, (int)len);
	PurgeComm(hDev, PURGE_TXCLEAR | PURGE_RXCLEAR);
	do{
		if (WriteFile( hDev,// handle to file to write to
			buf+length,              // pointer to data to write to file
			len-length,              // number of bytes to write
			&i,NULL) == 0)      // pointer to number of bytes written
		{
			//Trace("WriteFile err=%d", GetLastError());
			return COM_ERROR ;
		}
		length += i ;
	}while(length < len) ;

	return COM_OK ;
}

/*************************************************************************
����: �Ӵ��ڶ�����
����: 
	char *pData[out]
	int *pLen[in,out]
	int iTimeout[in]
����: 0�ɹ�������ʧ��
add by lch 
*************************************************************************/
DWORD CPdkbdoc::ReadCom(BYTE *pData,DWORD dwBytesRead)
{
	DWORD length=0;
	if(hDev == NULL)
	{
		//Trace("ReadCom hDev is NULL");
		return 0 ;
	}
	if (!ReadFile(hDev, pData, dwBytesRead, &length, NULL)) 
	{
		//Trace("ReadFile err=%d", GetLastError());
		return 0 ;
	}
	return length;
}

BOOL CPdkbdoc::IsOpen()
{	
	return (hDev>0) ? TRUE:FALSE;
}

DWORD CPdkbdoc::IsRecvEevent(void)
{
	COMSTAT ComStat={0};
	DWORD dwErrorFlags = 0;
	static DWORD dwBytesRead=0;
	if(hDev == NULL)
	{
		return 0 ;
	}
	//memset(&ComStat, 0, sizeof(ComStat));
	ClearCommError(hDev, &dwErrorFlags, &ComStat);
	if(dwBytesRead == 0)
	{
		if(ComStat.cbInQue)
		{
			dwBytesRead = ComStat.cbInQue;
			//Sleep(I_DELAYTIME+I_DELAYTIME);	//������ʱ
		}
	}
	else
	{
		if(dwBytesRead == ComStat.cbInQue)
		{
			dwBytesRead=0;
			return ComStat.cbInQue;
		}
		dwBytesRead=ComStat.cbInQue;
		//Sleep(I_DELAYTIME+I_DELAYTIME);	//������ʱ
	}
	return 0;
}

int CPdkbdoc::SupReadCom(BYTE *pData, DWORD dwBytesRead, int timeOutMs)
{
	DWORD dwReadLen = 0,offset=0;
	clock_t timeOut;

	if (hDev == NULL)
	{
		return -1;
	}
	if (timeOutMs)
	{
		timeOut = clock() + timeOutMs;
	}

	for (;;)
	{
		//for (int delay = 0; delay < 1000; delay++) SwitchToThread();
		//---0.25ms
		//for (int delay = 0; delay < 1667; delay++) SwitchToThread();
		Sleep(1);
		if (ReadFile(hDev, pData+ offset, dwBytesRead- offset, &dwReadLen, NULL))
		{
			if(dwReadLen>0)
				offset += dwReadLen;
		}
		
		if (dwBytesRead)
		{
			if(offset >= dwBytesRead)
				return offset;
		}

		if (timeOutMs)
		{
			if ((clock() - timeOut) >= 0)
			{
				return offset;
			}	
		}
	}
}

