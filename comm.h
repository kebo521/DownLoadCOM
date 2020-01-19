// Pdkbdoc.h: interface for the CPdkbdoc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDKBDOC_H__A47E46A4_15A8_4DDA_BBF9_A41D76994631__INCLUDED_)
#define AFX_PDKBDOC_H__A47E46A4_15A8_4DDA_BBF9_A41D76994631__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const BYTE PINPAD_115200 = 0; //波特率0:115200
const BYTE PINPAD_57600  = 1; //波特率1:57600
const BYTE PINPAD_38400  = 2; //波特率2:38400
const BYTE PINPAD_28800  = 3; //波特率3:28800
const BYTE PINPAD_19200  = 4; //波特率4:19200
const BYTE PINPAD_14400  = 5; //波特率5:14400
const BYTE PINPAD_9600   = 6; //波特率6:9600

typedef int (*CHECK_DATA)(char*,int*); 

class CPdkbdoc  
{
public:
	CPdkbdoc();
	virtual ~CPdkbdoc();

public:
	int OpenCom(const char *portName, const DWORD m_baudrate);  //打开COM口
	void SetComm(CString portName);
	int SetupParam(DWORD BaudRate, BYTE Parity, BYTE ByteSize, BYTE StopBits);
	int CloseCom();												//关闭COM口
	DWORD ReadCom(BYTE *pData,DWORD dwBytesRead);			//读取串口数据
	int WriteCom(const BYTE *buf, DWORD len );			//写串口数据
	void Crc16CCITT(BYTE *pDataIn, DWORD dwLen, BYTE *pOut);	//计算CRC16
	BOOL IsOpen();
	DWORD IsRecvEevent();
	int SupReadCom(BYTE *pData, DWORD dwBytesRead, int timeOutMs);
private:
	HANDLE hDev; //设备句柄
	CString strComName;
};

#endif // !defined(AFX_PDKBDOC_H__A47E46A4_15A8_4DDA_BBF9_A41D76994631__INCLUDED_)
