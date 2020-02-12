
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
	if(pAllPackFile)
	{
		free(pAllPackFile);
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
	ON_CBN_SELCHANGE(IDC_COMBO1,&CDownLoadKNDlg::OnCbnSelchangeCombo1)
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


//const char SN[] = { "\x10\x01\x20\x46\x12\x99\x00\x00\x01" };



void CDownLoadKNDlg::fReadFileMsg(CString m_FilePath)
{
	CStdioFile fin;
	//加载txt文件
	if (fin.Open(m_FilePath, CFile::modeNoTruncate | CFile::modeRead | CFile::typeBinary))
	{
		DWORD ReadLen;
		//fin.SeekToBegin();
		ReadLen = fin.SeekToEnd();
		//OutSprintEdit1("总文件长度%d", ReadLen);
		pAllPackFile= (PE_DATA_PACK *)malloc(sizeof(PE_DATA_PACK)+ReadLen);
		if(pAllPackFile == NULL)
		{
			MessageBox("读取文件失败[申请空间失败]");
		}
		fin.SeekToBegin();
		pAllPackFile->lenData=fin.Read(pAllPackFile->pData,ReadLen);
		fin.Close();
		if(pAllPackFile->lenData != ReadLen)
		{
			MessageBox("读取文件失败[长度错]");
			free(pAllPackFile);
			pAllPackFile=NULL;
		}
		strcpy_s(pAllPackFile->sName,m_FilePath.GetBuffer(0));
		pAllPackFile->offset = 0;
		//-----计算文件Md5值-----------
	//	md5(pAllPackFile->pData,pAllPackFile->lenData,pAllPackFile->uMd5);
	}
}

void CDownLoadKNDlg::OnBnClickedButton3()
{
	char pBuf[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pBuf);
	CFileDialog dlg(true, ".ksp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "ksp文件(*.ksp)|*.ksp");
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

void CDownLoadKNDlg::OnCbnSelchangeCombo1()
{
	FindComPort();
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

typedef struct
{
	char		sTuSN[32];
	char		sModel[8];
	char		sType[2];
	char		sComType[6];
	char		sParVer[12];
}tData_TMS_Const;

int TMS_CheckFullData(BYTE *pIndata,int InLen)
{
	int Len=0;
	//TRACE_HEX("TMS_CheckFullData",pIndata,InLen);
	if(InLen > 2)
	{
		Len = *pIndata++;
		Len *= 256;
		Len |= *pIndata++;
		Len += 2;
		//if(Len > 4096) return -1;
		if(InLen >= Len) return Len;
	}
	return 0;
}



BYTE* TMS_AddSendData(BYTE *pSendData,int hID,char *format,...)
{
	int ret;
	va_list arg;
	*pSendData++ = hID/256;
	*pSendData++ = hID&0xff;
	va_start( arg, format);
	ret=vsnprintf((char*)pSendData+2,256,format,arg);
	va_end( arg );
	*pSendData++ = ret/256;
	*pSendData++ = ret&0xff;
	pSendData += ret;
	return pSendData;
}

BYTE* TMS_AddSendBuff(BYTE *pSendData, int hID,BYTE *pInBuf, int inLen)
{
	*pSendData++ = hID / 256;
	*pSendData++ = hID & 0xff;
	*pSendData++ = inLen / 256;
	*pSendData++ = inLen & 0xff;
	memcpy(pSendData, pInBuf, inLen);
	pSendData += inLen;
	return pSendData;
}

BYTE* TMS_FindRecvData(BYTE *pRecvData,DWORD DataLen,WORD uInID,DWORD *pOutLen)
{
	DWORD Len,Tag;
	int cont = 0;
	BYTE *pEnd = pRecvData+DataLen;
	while(pRecvData < pEnd)
	{	
		Tag = *pRecvData++;
		Tag *= 256;
		Tag |= *pRecvData++;
		
		Len =* pRecvData++;
		Len *= 256;
		Len |= *pRecvData++;

		if(Tag == uInID)
		{
			*pOutLen = Len;
			return pRecvData;
		}
		pRecvData += Len;
	}
	return NULL;
}

// 线程函数，循环监听串口握手包
DWORD WINAPI CDownLoadKNDlg::ThreadFunc(LPVOID pParam)
{
	CDownLoadKNDlg *dlg = (CDownLoadKNDlg *)pParam;
	DWORD ReadLen,DataLen;
	int ret;
	BYTE sBuffSend[4096+64],sBuffRecv[512],*pRecvData,*p;

	if (NULL == dlg) {
		//Trace("工作线程获取主对话框指针失败.");
		dlg->m_thread = NULL;
		return -1;
	}
	dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	if (0 == (ret=dlg->m_serial.SetupParam(115200, NOPARITY, 8, ONESTOPBIT)))
	{
		while(1)
		{
			//m_Port.SetupParam(nPort,115200,'N','8','1');
			dlg->OutSprintEdit1("等待终端请求...");
			ReadLen = sizeof(sBuffRecv);
			pRecvData = dlg->m_serial.SupReadPackCom(sBuffRecv,&ReadLen,30*1000);
			if(pRecvData == NULL)
				return (-1100 + 1);
			pRecvData[ReadLen]='\0';
			p = (BYTE*)strstr((char*)pRecvData,"->hello");
			if(p)
			{
				dlg->OutSprintEdit1("收到请求[%s]",pRecvData);
				*p='\0';
				ret=sprintf_s((char*)sBuffSend+4,sizeof(sBuffSend)-4,"Down->%s",pRecvData);
				if(dlg->m_serial.SupSendPackCom(sBuffSend+4,ret))
					return -1201;
			}
			pRecvData = dlg->m_serial.SupReadPackCom(sBuffRecv,&ReadLen,30*1000);
			if(pRecvData == NULL)
				return (-1100 + 2);

			p=TMS_FindRecvData(pRecvData,ReadLen,0x3103,&DataLen);	//pFixed->sTuSN
			p=TMS_FindRecvData(pRecvData,ReadLen,0x3140,&DataLen);	//pFixed->sType
			p=TMS_FindRecvData(pRecvData,ReadLen,0x3141,&DataLen);	//pFixed->sModel
			p=TMS_FindRecvData(pRecvData,ReadLen,0x3126,&DataLen);	//"{\"net\":\"%s\",\"app_v\":\"%s\",\"source_v\":\"%s\",\"life\":\"%d\",\"bat\":\"%d\"}"


			
			{
				time_t tt = time(NULL); 
				tm t; localtime_s(&t, &tt); 
				//"date": '2018-05-01 10:00:00'
				printf("%d-%02d-%02d %02d:%02d:%02d", t.tm_year + 1900, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
			}
		
			dlg->m_serial.WriteCom(sBuffSend, 4096);//SEND Address 

			
			dlg->OutSprintEdit1("开始擦除芯片...");
			
			dlg->OutSprintEdit1("开始下载程序...");
			
			dlg->OutSprintEdit1("下载程序完成");
			//SNautoInc();  //序列号自动增加
		}
	}
	else
	{
		dlg->OutSprintEdit1("打开串口失败[%d]", ret);
	}
	
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

	if(FALSE == StartDownLoadCode())
	{
		m_serial.CloseCom();
	}
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
