
// DownLoadKNDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DownLoadKN.h"
#include "DownLoadKNDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_OUTPUTEDIT1  	(WM_USER+1)   // do something
#define WM_OUT_PROGRESS   	(WM_USER+2) 

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDownLoadKNDlg 对话框



CDownLoadKNDlg::CDownLoadKNDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DOWNLOADKN_DIALOG, pParent)
	, sOutPutMsg(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDownLoadKNDlg::~CDownLoadKNDlg()
{
	PE_DATA_PACK *pNext;
	while (pAllPackFile)
	{
		pNext = pAllPackFile->pNext;
		free(pAllPackFile->pData);
		free(pAllPackFile);
		pAllPackFile = pNext;
	}
}

void CDownLoadKNDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, sOutPutMsg);
	DDX_Control(pDX, IDC_COMBO1, m_ComList);
	DDX_Control(pDX, IDC_EDIT1, sOutPutCtrl);
	DDX_Control(pDX, IDC_PROGRESS1, ProgressDown);
}

BEGIN_MESSAGE_MAP(CDownLoadKNDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_OUTPUTEDIT1, &CDownLoadKNDlg::OnOutPutEditUpdate)
	ON_MESSAGE(WM_OUT_PROGRESS, &CDownLoadKNDlg::OnOutPutProgress)
	ON_BN_CLICKED(IDC_BUTTON3, &CDownLoadKNDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CDownLoadKNDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CDownLoadKNDlg::OnNMCustomdrawProgress1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDownLoadKNDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDownLoadKNDlg 消息处理程序

BOOL CDownLoadKNDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	FindComPort();

	if (__argc > 1)
	{
		fReadFileMsg(__argv[1]);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDownLoadKNDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDownLoadKNDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDownLoadKNDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//==============================================================================
char tohex(char b)
{
	if (b >= 0 && b <= 9)
	{
		return (b + '0');
	}
	else if (b >= 0xA && b <= 0xF)
	{
		return (b - 0xA + 'A');
	}
	else return ' ';
}


BYTE  ishex(byte b)
{
	if ((b >= '0' && b <= '9')
		|| (b >= 'a' && b <= 'f')
		|| (b >= 'A' && b <= 'F'))
		return 1;
	return 0;
}

//	字符转成相应Hex数字
//	'0'-'9'=0x0-0x9  'a'-'f'=0xA-0xF	'A'-'F'=0xA-0xF
BYTE Conv_CharToNum(char c)
{
	if (c >= '0' && c <= '9')
	{
		return (c - '0');
	}
	else if (c >= 'a' && c <= 'f')
	{
		return (c - 'a' + 10);
	}
	else if (c >= 'A' && c <= 'F')
	{
		return (c - 'A' + 10);
	}
	else return 0;
}
//  字符串转成BCD码，BCD码左对齐
//	bcd	="\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
//	Conv_StrToBcd("12345",5,bcd);	
//	bcd	="\x12\x34\x5F\xFF\xFF\xFF\xFF";

void Conv_StrToBcd_Left(char* str, int strlen, BYTE* bcd)
{
	BYTE	c;
	BYTE	b;
	int		i = 0;
	int		j = 0;
	for (i = 0; i<strlen; i++)
	{
		if (ishex(str[i]))
		{
			c = Conv_CharToNum(str[i]);
			b = bcd[j];
			if (i % 2) //Right
			{
				b &= 0xF0;
				b |= c;
				bcd[j++] = b;
			}
			else
			{
				b &= 0x0F;
				b |= (c << 4);
				bcd[j] = b;
			}
		}
		else
			break;
	}
}

//==============================================================================
LRESULT CDownLoadKNDlg::OnOutPutProgress(WPARAM wParam, LPARAM lParam)
{
	//OutSprintEdit1("OnOutPutProgress[%d][%d]", wParam, lParam);
	/*
	DWORD dwOldStyle = ProgressDown.GetStyle();
	DWORD dwNewStyle = dwOldStyle;
	if (pct>0) //如果pct>0，将显示进度条
		dwNewStyle |= WS_VISIBLE;
	else //否则隐藏进度条；
		dwNewStyle &= ~WS_VISIBLE;
	if (dwNewStyle != dwOldStyle) {
		SetWindowText(NULL); //显示进度条前清空状态条；
		SetWindowLong(pc.m_hWnd, GWL_STYLE, dwNewStyle);
		//设置进度条处于显示状态；
	}
	*/
	//SetWindowText(NULL); //显示进度条前清空状态条；
	// 设置进度条的当前位置；
	ProgressDown.SetPos(wParam);
	/*
	if (pct == 0)
		// 如果pct等于0，通知主框架窗口显示空闲信息；
		GetParent()->PostMessage(WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);
	*/
	return 0;
}

void CDownLoadKNDlg::RefreshProgress(WPARAM Pos)
{
	::SendMessage(m_hWnd, WM_OUT_PROGRESS, Pos, 0);
}


// 自动清空信息输出框大小
const int AOTO_CLEAR_OUTPUT = 2 * 1024;
const int AOTO_CLEAR_OUTPUT_RETAIN = 300;

LRESULT CDownLoadKNDlg::OnOutPutEditUpdate(WPARAM wParam, LPARAM lParam)
{
	UpdateData(FALSE);
	int nLength = sOutPutCtrl.GetWindowTextLength();
	sOutPutCtrl.SetSel(nLength, nLength, TRUE);
	sOutPutCtrl.LineScroll(sOutPutCtrl.GetLineCount() - 1);
	return 0;
}

void CDownLoadKNDlg::RefreshDisplayEdit1(void)
{
	//===AutoClearOutPut=====
	/*
	if (sOutPutEdit1.GetLength() > AOTO_CLEAR_OUTPUT)
	{
	int pos = sOutPutEdit1.Find('\n', AOTO_CLEAR_OUTPUT_RETAIN);
	if (pos){
	sOutPutEdit1 = "....由于缓冲过大自动截断日志.." + sOutPutEdit1.Mid(pos-1);
	}
	}*/
	//======Send Msg to OnOutPutEditUpdate====
	::SendMessage(m_hWnd, WM_OUTPUTEDIT1, 0, 0);
}

void CDownLoadKNDlg::OutPutEdit1(char* pStr)
{
	//Trace("%s", str);
	sOutPutMsg += pStr;
	sOutPutMsg += "\r\n";
	RefreshDisplayEdit1();
}


void CDownLoadKNDlg::OutSprintEdit1(const char *format, ...)
{
	va_list arg;
	char str[4096];
	va_start(arg, format);
	vsnprintf_s(str,sizeof(str), format, arg);
	sOutPutMsg += str;
	sOutPutMsg += "\r\n";
	va_end(arg);
	RefreshDisplayEdit1();
}


void CDownLoadKNDlg::OutPutHexEdit1(char *pTitle, BYTE *pBcd, int len)
{
	int i;
	char ShowMsg[256];
	SYSTEMTIME systm;
	GetLocalTime(&systm);
	sprintf_s(ShowMsg, ">%08d:====<%s>[%d]=======", systm.wMilliseconds, pTitle, len);
	sOutPutMsg += ShowMsg;
	if (pBcd == NULL)
	{
		sOutPutMsg += "Data=NULL";
	}
	else for (i = 0; i<len; i++)
	{
		if (i % 16 == 0)
			sOutPutMsg += "\r\n";
		sprintf_s(ShowMsg, "%02X ", pBcd[i]);
		sOutPutMsg += ShowMsg;
	}
	sOutPutMsg += "\r\n";
	RefreshDisplayEdit1();
}


//枚举本机所有的可用端口
void CDownLoadKNDlg::FindComPort(void)
{
	HKEY   hKey;
	CString strMsg = "";
	LONG result;
	result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Hardware\\DeviceMap\\SerialComm"), NULL, KEY_READ, &hKey);
	if (result == ERROR_SUCCESS) {
		int		nCount = 0;
		DWORD	dwLong, dwSize;
		TCHAR	szPortName[256], szComName[256];
		CComboBox*	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO1);
		int iSel = pCombo->GetCurSel();
		pCombo->ResetContent();
		while (true) {
			dwLong = dwSize = 256;
			result = RegEnumValue(hKey, nCount, szPortName, &dwLong, NULL, NULL, (PUCHAR)szComName, &dwSize);
			if (result == ERROR_NO_MORE_ITEMS) {
				break;
			}
			pCombo->InsertString(nCount, szComName);
			nCount++;
		}
		RegCloseKey(hKey);
		if (iSel > pCombo->GetCount()) {
			m_ComList.SetCurSel(iSel);
		}
		else {
			m_ComList.SetCurSel(0);
		}
		UpdateData(TRUE);
	}
}

#define EXCMD_FONT		0x0f
#define EXCMD_MAGCARD	0x01
#define EXCMD_ICCARD	0x02
#define EXCMD_ACK       0x06

#define FONT_CMD_ERASEALL	0X01 //[LEN 2B][01][DATA KINGSEE][BCC 1B] 擦除所有的FLASH
#define FONT_CMD_SETADD		0X11 //[LEN 2B][11][AD][DATA N][BCC 1B]指定地址, 定长 00=256 01=1 FF=255
#define FONT_CMD_READ		0X30 //[LEN 2B][30][RD][DATA N][BCC 1B]读数据从指定地址, 定长 00=256 01=1 FF=255
#define FONT_CMD_WRITE		0X83 //[LEN 2B][83][WR][DATA N][BCC 1B]写数据指定地址，定长00=256 01=1 FF=255

DWORD Down_StrAddBccByte(BYTE *str, DWORD len)
{
	DWORD i;
	BYTE bcc = 0;
	bcc = 0;
	for (i = 0; i<len; i++)
	{
		bcc ^= str[i];
	}
	str[i++] = bcc;  //在字串的最后增加BCC校验位
	return i;
}

DWORD Make_FBFEpack(BYTE *si, DWORD silen, BYTE *so)
{
	BYTE c, cksum = 0;
	DWORD i;
	BYTE *pOut;

	pOut = so;
	for (i = 0; i<silen; i++)
		cksum += si[i];
	cksum = ~cksum;
	cksum += 1;
	*pOut++ = 0xfb;
	for (i = 0; i<silen; i++)
	{
		c = si[i];
		if (c == 0xFA || c == 0xFB || c == 0xFD || c == 0xFE)
		{
			*pOut++ = 0xfd;
			*pOut++ = (c & 0x0f);
		}
		else
		{
			*pOut++ = c;
		}
	}
	if (cksum == 0xFA || cksum == 0xFB || cksum == 0xFD || cksum == 0xFE)
	{
		*pOut++ = 0xfd;
		*pOut++ = (cksum & 0x0f);
	}
	else
	{
		*pOut++ = cksum;
	}
	*pOut++ = 0xfe;
	return (pOut - so);
}
DWORD EXCMD_GeneratePack(BYTE *si, DWORD silen, BYTE cmd, BYTE *so)
{// 1b ee len0 len1 cmd data[n] bcc
	BYTE bcc = 0;
	BYTE *pOut;
	DWORD i, len, retLen;
	BYTE stemp[1024] = { 0 };
	pOut = stemp;
	*pOut++ = (BYTE)silen % 256;
	*pOut++ = (BYTE)silen / 256;
	memcpy(pOut, "FONT", 4);
	pOut += 4;

	for (i = 0; i<silen; i++)
	{
		*pOut++ = si[i];
	}

	len = pOut - stemp;

	retLen = Make_FBFEpack(stemp, len, so);
	return retLen;  //(pOut-so);  //返回长度
					//return Make_FBFEpack(si,silen,so);
}

DWORD EXCMD_GenerateExitPack(BYTE *so)
{// 1b ee len0 len1 cmd data[n] bcc
	BYTE bcc = 0;
	BYTE *pOut;
	DWORD len, retLen;
	BYTE stemp[1024] = { 0 };
	pOut = stemp;
	*pOut++ = 0;  //silen%256;
	*pOut++ = 1;  //silen/256;
	memcpy(pOut, "EXIT", 4);
	pOut += 4;
	*pOut++ = 0;

	len = pOut - stemp;
	retLen = Make_FBFEpack(stemp, len, so);
	return retLen;  //(pOut-so);  //返回长度
					//return Make_FBFEpack(si,silen,so);
}


DWORD CDownLoadKNDlg::FONT_GeneratePack(BYTE *si, DWORD silen, BYTE cmd, BYTE *so)
{// 1b ee len0 len1 cmd data[n] bcc
	BYTE bcc = 0;
	BYTE stemp[520] = { 0 };
	BYTE *pOut;
	DWORD i, len;
	pOut = stemp;
	pOut++;
	pOut++;
	*pOut++ = cmd;
	switch (cmd)
	{
	case FONT_CMD_ERASEALL:
		memcpy(pOut, "KINGSEE@", 8);
		pOut += 8;
		break;
	case FONT_CMD_SETADD:
		memcpy(pOut, "AD", 2);
		pOut += 2;
		break;
	case FONT_CMD_READ:
		memcpy(pOut, "RD", 2);
		pOut += 2;
		*pOut++ = (BYTE)((g_FontAdd >> 24) & 0x0ff);
		*pOut++ = (BYTE)((g_FontAdd >> 16) & 0x0ff);
		*pOut++ = (BYTE)((g_FontAdd >> 8) & 0x0ff);
		*pOut++ = (BYTE)((g_FontAdd)& 0x0ff);
		break;
	case FONT_CMD_WRITE:
		memcpy(pOut, "WR", 2);
		pOut += 2;
		*pOut++ = (BYTE)((g_FontAdd >> 24) & 0x0ff);
		*pOut++ = (BYTE)((g_FontAdd >> 16) & 0x0ff);
		*pOut++ = (BYTE)((g_FontAdd >> 8) & 0x0ff);
		*pOut++ = (BYTE)((g_FontAdd)& 0x0ff);
		break;
	default:
		break;
	}
	for (i = 0; i<silen; i++)
	{
		*pOut++ = si[i];
	}
	len = pOut - stemp;
	stemp[0] = (BYTE)((len - 2) >> 8);
	stemp[1] = (BYTE)((len - 2) & 0xff);

	for (i = 0; i<(len); i++)
	{
		bcc ^= stemp[i];
	}
	*pOut++ = bcc;
	len++;
	TRACE("LEN=%02x\r\n", len);
	return EXCMD_GeneratePack(stemp, len, EXCMD_FONT, so);  //返回长度
}


int CDownLoadKNDlg::Font_ExecCmd(BYTE cmd, BYTE *s, DWORD len, BYTE tmOutSec)
{
	int i;
	DWORD l;
	BYTE sBuf[512], chRcvbuf[512];
	l = FONT_GeneratePack(s, len, cmd, sBuf);
	for (i = 0; i<5; i++)
	{
		m_serial.WriteCom(sBuf, l);
		if (m_serial.SupReadCom(chRcvbuf, 1, tmOutSec))
		{
			if (chRcvbuf[0] == EXCMD_ACK)
			{
				//ShowMsg("执行命令成功");
				if (cmd == FONT_CMD_READ)
				{
					m_serial.SupReadCom(chRcvbuf, 1000, tmOutSec);
				}
				return  0;
			}
			else
			{
				//m_Port.sReadByLen(1000,tmOutSec);
				return 1;
			}
		}
	}
	return 1;
}

int CDownLoadKNDlg::EXIT_ExecCmd(BYTE tmOutSec)
{
	int i;
	DWORD l;
	BYTE sBuf[512], chRcvbuf[512];
	l = EXCMD_GenerateExitPack(sBuf);
	for (i = 0; i<5; i++)
	{
		m_serial.WriteCom(sBuf, l);
		if (m_serial.SupReadCom(chRcvbuf, 1, tmOutSec))
		{
			return  0;
		}
		else
		{
			//m_Port.sReadByLen(1000,tmOutSec);
			return 1;
		}
	}
	return 1;
}



int CDownLoadKNDlg::Font_BuildLink(BYTE tmOutSec)
{
	int i;
	BYTE chRcvbuf[256];
	for (i = 0; i<5; i++)
	{
		m_serial.WriteCom((BYTE*)"K", 1);
		if (m_serial.SupReadCom(chRcvbuf, 1, tmOutSec))
		{
			if (chRcvbuf[0] == 'S')
			{
				//ShowMsg("执行命令成功");
				m_serial.WriteCom((BYTE*)"A", 1);
				return  0;
			}
		}
	}
	return 1;
}

void CDownLoadKNDlg::DownLoadKN()
{
	char sMsg[200] = { 0 };
	BYTE   sBuf[1024];

	//2-1增加建立连接 BUILDLINK
	if (Font_BuildLink(2))
	{
		OutPutEdit1("建立连接失败");
		return;
	}
	
	// m_Port.AsynClose();
	// m_Port.SetupParam(nPort,38400,'N','8','2');
	//m_Port.AsynOpen();
	//3 发送数据擦除芯片
	OutPutEdit1("正在擦除...");
	if (0 == Font_ExecCmd(FONT_CMD_ERASEALL, sBuf, 0, 10))
	{
		OutPutEdit1("擦除成功！");
		Sleep(100);
		//3 loop send file
		OutPutEdit1("开始下载资源");
	
	}
	/*
	for(i=0;i<1160;i++)   //FILE 大小
	//for(i=0;i<(iFileLength+127)/128; i++)
	{
	FileRead(FileFont, sBuf, 128);
	//sprintf(szCmd,"%x %x %x %x %x %x %x %x",sBuf[0], sBuf[1],sBuf[2],sBuf[3],
	//sBuf[4],sBuf[5],sBuf[6],sBuf[7]);
	//ShowMsg(szCmd);
	if(Font_ExecCmd(FONT_CMD_WRITE,sBuf,128,1))
	{
	ShowMsg("写入失败！");
	Sleep(5000);
	break;
	}
	g_FontAdd +=128;
	ProgressDown->Position=i;
	}
	ProgressDown->Position=0;
	}
	FileClose(FileFont);     //关闭文件

	EXIT_ExecCmd(5);     //发送退出指令
	//4 下载完成，关闭串口
	ProgressDown->Position=0;
	m_Port.AsynClose();
	ShowMsg("关闭串口");
	*/

}




int CDownLoadKNDlg::Down_SendRecvPross(BYTE *s, DWORD len,BYTE *pRecv,DWORD rMixLen,int mTimeOut)
{
	int ret;
	BYTE chRcvbuf[512];
	m_serial.WriteCom(s, len);
	ret = m_serial.SupReadCom(chRcvbuf, 1 + rMixLen, mTimeOut);
	if (ret > 0)
	{
		if (chRcvbuf[0] == 'y')
		{
			ret--;
			if (pRecv)
			{
				memcpy(pRecv, chRcvbuf + 1, ret);
			}
			return ret;
		}
		else if (chRcvbuf[0] == 0x1f) //chip locked
		{
			m_serial.WriteCom((BYTE*)"\x92\x6d", 2);
			ret = m_serial.SupReadCom(chRcvbuf, 1, 5);
			if (chRcvbuf[0] == 'y')
				return 0;
			return -32;
		}
		return -51;
	}
	//失败
	return ret;
}


int CDownLoadKNDlg::Down_DownDataPack(BYTE* pCodeAddress, BYTE *Data, DWORD DataLen)
{
	int ret;
	DWORD i;
	BYTE  s[512], chRcvbuf[512];
	//--31 CE  .YES
	m_serial.WriteCom((BYTE*)"\x31\xce", 2);
	//--DATA ADD SEND
	memcpy(s,pCodeAddress, 4);
	Down_StrAddBccByte(s, 4);

	ret = m_serial.SupReadCom(chRcvbuf, 1,5);
	if (ret<1 || chRcvbuf[0] != 'y')
		return (-1100 + ret);

	m_serial.WriteCom(s, 5);//SEND Address 

	s[0] = (BYTE)(DataLen - 1);
	for (i = 0; i<DataLen; i++)
		s[i + 1] = Data[i];
	Down_StrAddBccByte(s, DataLen + 1);

	ret = m_serial.SupReadCom(chRcvbuf, 1, 5);
	if (ret<1 || chRcvbuf[0] != 'y') //Recv Address flag
		return (-1200 + ret);
	//SEND code data and Recv flag
	if (0>(ret = Down_SendRecvPross(s, DataLen + 2,NULL,0,50)))
		return (-1300 + ret);
	return 0;
}

int  CDownLoadKNDlg::User_BuildLink()
{
	int ret;
	BYTE s[200];
	//ShowMsg("User_BuildLink::");
	if (0>(ret= Down_SendRecvPross((BYTE*)"\x7f", 1,NULL,0,5)))
	{
		OutPutEdit1("User_BuildLink::send err");
		return (-2100+ ret);
	}
	
	m_serial.CloseCom();
	//m_Port.SetupParam(nPort,115200,'E','8','1');
	m_serial.SetupParam(57600, EVENPARITY, 8, ONESTOPBIT);
	if (!m_serial.IsOpen())
	{
		//sprintf(sMsg,"打开串口[%d]失败,请选择正确的端口",nPort);
		OutPutEdit1("打开串口失败");
		return -3100;
	}
	//ShowMsg("User_BuildLink::Asynopen 2");
	//Sleep(100);   //REV STR
	if (0 > (ret = Down_SendRecvPross((BYTE*)"\x00\xff",2,s,260,50)))
		return (-1100 + ret);
	//OutSprintEdit1("SupReadCom[%d],[%d]", ret, s[0]);
	if (ret < (s[0]+2)) return (-1300 + ret);
		
	Sleep(10);   //REV STR
	if (0 > (ret = Down_SendRecvPross((BYTE*)"\x02\xfd",2, s, 260, 50)))
		return (-1400 + ret);
	
	if (ret < (s[0] + 2)) return (-1500 + ret);
		
	//OutSprintEdit1("111SupReadCom[%d]", ret);
	Sleep(10);   //REV STR
	if (0 > (ret = Down_SendRecvPross((BYTE*)"\x02\xfd", 2,s,260,50)))
		return (-1700 + ret);
	if (ret < (s[0] + 2)) return (-1800 + ret);

	Sleep(10);   //REV STR
	if (0 > (ret = Down_SendRecvPross((BYTE*)"\x01\xfe", 2, s, 260, 5000)))
		return (-2000 + ret);
	//OutSprintEdit1("2SupReadCom[%d],[%d]", ret, s[0]);
	//Sleep(10);   //REV STR
	//OutPutEdit1("与终端连接成功");
	return 0;
	//return 0;
}

int CDownLoadKNDlg::User_EraseChip()
{
	BYTE s[256];
	BYTE chRcvbuf[256];
	int ret;
	int EraseBlock, m_CodeLen;
	//Sleep(100);   //REV STR

	if (0>(ret = Down_SendRecvPross((BYTE*)"\x11\xee", 2, NULL, 0, 5)))
	{
		OutPutEdit1("User_Erase::send err");
		return (-1000 + ret);
	}
	
	//Sleep(50);   //REV STR
	if (0>(ret = Down_SendRecvPross((BYTE*)"\x08\x00\x00\x00\x08", 5,NULL,0,10)))
		return (-1100 + ret);
	//Sleep(50);   //REV STR
	if (0>(ret = Down_SendRecvPross((BYTE*)"\x00\xff", 2, chRcvbuf, 1, 10)))
		return (-1200 + ret);
	
	//Sleep(50);    //REV STR
	if (0>(ret = Down_SendRecvPross((BYTE*)"\x73\x8c", 2, chRcvbuf, 1, 100)))
		return (-1300 + ret);
	Sleep(50);   //REV STR
	if (0>(ret = Down_SendRecvPross((BYTE*)"\x7f", 1, NULL, 0, 5)))
		return (-1400 + ret);
	//Sleep(50);   //REV STR
	if (0>(ret = Down_SendRecvPross((BYTE*)"\x43\xbc", 2, NULL, 0, 5)))
		return (-1500 + ret);
	if (0>(ret = Down_SendRecvPross((BYTE*)"\x00\x11\x11", 3, NULL, 0, 500)))
		return (-1600 + ret);

	m_CodeLen = pAllPackFile->line * 16;
	EraseBlock = m_CodeLen / 1024;
	EraseBlock++;  //代码有多大就要擦除多少K空间
	int i = 0;
	int j;
	int len;
	for (i = 0; i<EraseBlock; i += 10)
	{
		if (i + 10 <= EraseBlock)
			len = 10;
		else
			len = EraseBlock - i;

		if (0>(ret = Down_SendRecvPross((BYTE*)"\x43\xbc", 2, NULL, 0, 5)))
			return (-1700 + ret);

		s[0] = len - 1;
		for (j = 0; j<len; j++)
			s[1 + j] = i + j;
		Down_StrAddBccByte(s, len + 1);

		if (0>(ret = Down_SendRecvPross(s, len + 2, NULL, 0, 500)))
			return (-1800 + ret);
	}
	//if(false==Down_SendCmdPack("\x09\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x08",12))
	//	return false;   //09 00 01 02 03 04 05 06 07 08 09 08 
	//if(false==Down_SendCmdPack("\x43\xbc",2))
	//	return false;   //43 BC 
	//if(false==Down_SendCmdPack("\x04\x0A\x0B\x0C\x0D\x0E\x0A",7))
	//	return false;   //03 0A 0B 0C 0D 03 
	return 0;
}
//const char SN[] = { "\x10\x01\x20\x46\x12\x99\x00\x00\x01" };
int CDownLoadKNDlg::User_DownCode()
{
	BYTE *pCodeData;
	int	ret,line, unitNum, Percen,uNitPer,SendLen, Offset,MaxCodeLen;
	BYTE sAddress[8],sSendData[512];

	pCodeData = pAllPackFile->pData;
	MaxCodeLen = pAllPackFile->DataSize;
	unitNum= pAllPackFile->unitNum;
	uNitPer = pAllPackFile->line / 100;
	line = 0;
	SendLen = 0;
	Percen = 0;
	ProgressDown.SetRange(0, 100);
	ProgressDown.SetStep(1);
	RefreshProgress(Percen);
	for (;;)
	{
		Offset = line*unitNum;
		if (SendLen == 0)
		{
			memcpy(sAddress, pCodeData + Offset, 4);
		}
		Offset += 4;
		if (Offset >= MaxCodeLen) break;
		if ((MaxCodeLen - Offset) <= 16)
		{
			memcpy(sSendData + SendLen, pCodeData + Offset, MaxCodeLen- Offset);
			SendLen += MaxCodeLen - Offset;
			Offset = 0; //最后一段
		}
		else
		{
			memcpy(sSendData + SendLen, pCodeData + Offset, 16);
			SendLen += 16;
		}
		line++;

		if (SendLen >= (16*15) || Offset==0)
		{
			if (0>(ret = Down_DownDataPack(sAddress, sSendData, SendLen)))
			{
				OutSprintEdit1("下载[%s]失败[%d]", sAddress, ret);
				return -1;
			}
			SendLen = 0;
		}
		//---更新滚动条------
		if (Percen != (line / uNitPer))
		{
			Percen = line / uNitPer;
			RefreshProgress(Percen);
		}
	}
	OutPutEdit1("下载成功！");
	return 0; 
}


void CDownLoadKNDlg::fReadFileMsg(CString m_FilePath)
{
	CStdioFile fin;
	//加载txt文件
	if (fin.Open(m_FilePath, CFile::modeNoTruncate | CFile::modeRead | CFile::typeBinary))
	{
		PE_DATA_PACK *pStart = NULL, *pNext = NULL;
		char *pFileBuff, *pReadBuff, *pReadEnd;
		char cBcdBuff[512];
		DWORD DataLen, ReadLen;
		int m_CodeLen = 0, offset, line = 0;
		//fin.SeekToBegin();
		ReadLen = fin.SeekToEnd();
		//OutSprintEdit1("总文件长度%d", ReadLen);
		pFileBuff = (char*)malloc(ReadLen);
		fin.SeekToBegin();
		ReadLen = fin.Read(pFileBuff, ReadLen);
		fin.Close();
		//OutSprintEdit1("已读文件长度%d", ReadLen);
		pReadBuff = pFileBuff;
		pReadEnd = pReadBuff + ReadLen;
		for (line = 0; line < 2; line++)
		{
			DataLen = Conv_CharToNum(pReadBuff[2]) * 16 + Conv_CharToNum(pReadBuff[3]);
			DataLen *= 2;	//实际为2倍长度
							//---------------------------------------------------------------------------------
			if (!memcmp(pReadBuff, "S0", 2))
			{
				Conv_StrToBcd_Left(pReadBuff + 4, DataLen, (BYTE*)cBcdBuff);
				cBcdBuff[DataLen / 2] = '\0';
				if (line == 0)
				{
					OutPutEdit1(cBcdBuff);
					if (0 != memcmp(cBcdBuff, "KSHD@K35FV", 10))
					{
						break;
					}
				}
				else if (!memcmp(cBcdBuff, "KSCD", 4))
				{
					BYTE buff[8] = { 0 };
					Conv_StrToBcd_Left(&(cBcdBuff[4 + 8]), 8, buff);
					m_CodeLen = buff[0] * 0x1000000 + buff[1] * 0x10000 + buff[2] * 0x100 + buff[3];
					//OutSprintEdit1("代码长度%d字节", m_CodeLen);
				}
			}
			pReadBuff += 4 + DataLen;
			while (*pReadBuff == '\r' || *pReadBuff == '\n') pReadBuff++;
		}

		while (pReadBuff < pReadEnd)
		{
			DataLen = Conv_CharToNum(pReadBuff[2]) * 16 + Conv_CharToNum(pReadBuff[3]);
			Conv_StrToBcd_Left(pReadBuff + 4, DataLen * 2, (BYTE*)cBcdBuff);
			if (!memcmp(pReadBuff, "S0", 2))
			{
				int i = 0;
				cBcdBuff[DataLen] = '\0';
				while (cBcdBuff[i] < 0x20) i++;
				if (pStart == NULL)
				{
					pStart = (PE_DATA_PACK*)malloc(sizeof(PE_DATA_PACK));
					pNext = pStart;
				}
				else
				{
					pNext->pNext = (PE_DATA_PACK*)malloc(sizeof(PE_DATA_PACK));
					pNext = pNext->pNext;
				}
				strcpy_s(pNext->sName, sizeof(pNext->sName), cBcdBuff + i);
				pNext->pNext = NULL;
				pNext->pData = (BYTE*)malloc(ReadLen);
				pNext->line = 0;
				offset = 0;
			}
			else if (!memcmp(pReadBuff, "S3", 2))
			{
				if (pNext == NULL)
				{
					OutSprintEdit1("获取S3文件失败");
					break;
				}
				if (offset == 0)
				{
					pNext->unitNum = DataLen;
				}
				memcpy(pNext->pData + offset, cBcdBuff, DataLen);
				offset += DataLen;
				pNext->line++;
			}
			else if (!memcmp(pReadBuff, "S7", 2))
			{
				pNext->pData = (BYTE*)realloc(pNext->pData, offset);
				pNext->DataSize = offset;
				OutSprintEdit1("获取[%s ]长度[%d]行数[%d]", pNext->sName, offset, pNext->line);
			}
			pReadBuff += 4 + DataLen * 2;
			while (*pReadBuff == '\r' || *pReadBuff == '\n') pReadBuff++;
		}
		free(pFileBuff);
		if (pAllPackFile)
		{
			PE_DATA_PACK *pNext;
			while (pAllPackFile)
			{
				pNext = pAllPackFile->pNext;
				free(pAllPackFile->pData);
				free(pAllPackFile);
				pAllPackFile = pNext;
			}
		}
		pAllPackFile = pStart;

	}

}

void CDownLoadKNDlg::OnBnClickedButton3()
{
	char pBuf[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pBuf);
	CFileDialog dlg(true, ".K35", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "K35文件(*.K35)|*.K35");
	dlg.m_ofn.lpstrTitle = "请选择程序文件";

	if (dlg.DoModal() == IDOK)
	{
		CString m_FilePath = dlg.GetPathName();
		sOutPutMsg += m_FilePath;
		sOutPutMsg += "\r\n";
		UpdateData(FALSE);
		fReadFileMsg(m_FilePath);
	}
}


int CDownLoadKNDlg::OpenComm()
{
	CWnd *pWnd;
	int ret;
	CString strComName;
	
	//获取当前COM名称
	m_ComList.GetLBText(m_ComList.GetCurSel(), strComName);

	// 打开、关闭串口
	if (!m_serial.IsOpen()) {
		ret = m_serial.OpenCom(strComName, 115200);
		if (ret != 0) {
			MessageBox("打开串口失败");
			FindComPort();
			return 1;
		}
		OutSprintEdit1("打开串口[%s]成功", strComName.GetBuffer(0));
	}
	else {
		//关闭串口
		m_serial.CloseCom();
		FindComPort();          //每次关闭后重新枚举串口
		OutPutEdit1("串口已关闭");
	}

	// 允许或禁止修改COM
	pWnd = GetDlgItem(IDC_COMBO1);
	pWnd->EnableWindow(!m_serial.IsOpen());
	return 0;
}

int CDownLoadKNDlg::CloseComm()
{
	CWnd *pWnd;
	//关闭串口
	m_serial.CloseCom();
	FindComPort();          //每次关闭后重新枚举串口
	OutPutEdit1("串口已关闭");

	// 允许或禁止修改COM
	pWnd = GetDlgItem(IDC_COMBO1);
	pWnd->EnableWindow(!m_serial.IsOpen());
	return 0;
}

// 线程函数，循环监听串口握手包
DWORD WINAPI CDownLoadKNDlg::ThreadFunc(LPVOID pParam)
{
	CDownLoadKNDlg *dlg = (CDownLoadKNDlg *)pParam;
	int ret;

	if (NULL == dlg) {
		//Trace("工作线程获取主对话框指针失败.");
		dlg->m_thread = NULL;
		return -1;
	}
	dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	do {
		//m_Port.SetupParam(nPort,115200,'N','8','1');
		if (ret = dlg->m_serial.SetupParam(57600, NOPARITY, 8, ONESTOPBIT))
		{
			dlg->OutSprintEdit1("打开串口失败[%d]", ret);
			break;
		}
		dlg->OutSprintEdit1("开始连接终端...");
		if (ret = dlg->User_BuildLink())
		{
			dlg->OutSprintEdit1("连接终端[%d]", ret);
			break;
		}
		dlg->OutSprintEdit1("开始擦除芯片...");
		if (ret = dlg->User_EraseChip())
		{
			dlg->OutSprintEdit1("擦除芯片失败[%d]", ret);
			break;
		}
		dlg->OutSprintEdit1("开始下载程序...");
		if (ret = dlg->User_DownCode())
		{
			dlg->OutSprintEdit1("下载程序失败[%d]", ret);
			break;
		}
		dlg->OutSprintEdit1("下载程序完成");
		//SNautoInc();  //序列号自动增加
	} while (0);
	dlg->m_serial.CloseCom();

	dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	dlg->m_thread = NULL;
	return 1;
}

// 创建监听串口线程
BOOL CDownLoadKNDlg::StartDownLoadCode(void)
{
	m_thread = CreateThread(NULL, 0, ThreadFunc, this, 0, NULL);
	if (!m_thread) {
		//Trace("create thread failed. thread = [%d]", m_thread);
		m_thread = NULL;
		return FALSE;
	}
	return TRUE;
}


void CDownLoadKNDlg::OnBnClickedButton1()
{
	CString strComName;
	if (pAllPackFile == NULL)
	{
		OutSprintEdit1("未打开程序文件！");
		return;
	}
	if (m_thread)
		return;
	//获取当前COM名称
	m_ComList.GetLBText(m_ComList.GetCurSel(), strComName);
	// 打开、关闭串口
	m_serial.SetComm(strComName);
	OutSprintEdit1("打开串口[%s]...", strComName);

	StartDownLoadCode();
	/*
	OutSprintEdit1("开始连接终端...");
	if (ret = User_BuildLink())
	{
		OutSprintEdit1("连接终端[%d]", ret);
		m_serial.CloseCom();
		return;
	}
	OutSprintEdit1("开始擦除芯片...");
	if (ret = User_EraseChip())
	{
		OutSprintEdit1("擦除芯片失败[%d]", ret);
		m_serial.CloseCom();
		return;
	}
	OutSprintEdit1("开始下载程序...");
	if (ret = User_DownCode())
	{
		OutSprintEdit1("下载程序失败[%d]", ret);
		m_serial.CloseCom();
		return;
	}
	OutSprintEdit1("下载程序完成");
	//SNautoInc();  //序列号自动增加
	


	m_serial.CloseCom();
	*/
}


void CDownLoadKNDlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CDownLoadKNDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	int delay;
	clock_t timeEnd,timeStrat;
	
	timeStrat = clock();
	Sleep(1);
	timeEnd = clock();
	OutSprintEdit1("Sleep(1)=[%d]", timeEnd - timeStrat);

	timeStrat = clock();
	for (delay = 0; delay < 6666; delay++) SwitchToThread();
	timeEnd = clock();
	
	OutSprintEdit1("SwitchToThread(%d)=[%d]", delay,timeEnd - timeStrat);
}
