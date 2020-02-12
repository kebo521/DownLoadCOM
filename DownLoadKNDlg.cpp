
// DownLoadKNDlg.cpp : ʵ���ļ�
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CDownLoadKNDlg �Ի���



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


// CDownLoadKNDlg ��Ϣ�������

BOOL CDownLoadKNDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	FindComPort();

	if (__argc > 1)
	{
		fReadFileMsg(__argv[1]);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDownLoadKNDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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

//	�ַ�ת����ӦHex����
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
//  �ַ���ת��BCD�룬BCD�������
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
	if (pct>0) //���pct>0������ʾ������
		dwNewStyle |= WS_VISIBLE;
	else //�������ؽ�������
		dwNewStyle &= ~WS_VISIBLE;
	if (dwNewStyle != dwOldStyle) {
		SetWindowText(NULL); //��ʾ������ǰ���״̬����
		SetWindowLong(pc.m_hWnd, GWL_STYLE, dwNewStyle);
		//���ý�����������ʾ״̬��
	}
	*/
	//SetWindowText(NULL); //��ʾ������ǰ���״̬����
	// ���ý������ĵ�ǰλ�ã�
	ProgressDown.SetPos(wParam);
	/*
	if (pct == 0)
		// ���pct����0��֪ͨ����ܴ�����ʾ������Ϣ��
		GetParent()->PostMessage(WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);
	*/
	return 0;
}

void CDownLoadKNDlg::RefreshProgress(WPARAM Pos)
{
	::SendMessage(m_hWnd, WM_OUT_PROGRESS, Pos, 0);
}


// �Զ������Ϣ������С
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
	sOutPutEdit1 = "....���ڻ�������Զ��ض���־.." + sOutPutEdit1.Mid(pos-1);
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


//ö�ٱ������еĿ��ö˿�
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

#define FONT_CMD_ERASEALL	0X01 //[LEN 2B][01][DATA KINGSEE][BCC 1B] �������е�FLASH
#define FONT_CMD_SETADD		0X11 //[LEN 2B][11][AD][DATA N][BCC 1B]ָ����ַ, ���� 00=256 01=1 FF=255
#define FONT_CMD_READ		0X30 //[LEN 2B][30][RD][DATA N][BCC 1B]�����ݴ�ָ����ַ, ���� 00=256 01=1 FF=255
#define FONT_CMD_WRITE		0X83 //[LEN 2B][83][WR][DATA N][BCC 1B]д����ָ����ַ������00=256 01=1 FF=255

DWORD Down_StrAddBccByte(BYTE *str, DWORD len)
{
	DWORD i;
	BYTE bcc = 0;
	bcc = 0;
	for (i = 0; i<len; i++)
	{
		bcc ^= str[i];
	}
	str[i++] = bcc;  //���ִ����������BCCУ��λ
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
	return retLen;  //(pOut-so);  //���س���
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
	return retLen;  //(pOut-so);  //���س���
					//return Make_FBFEpack(si,silen,so);
}


//const char SN[] = { "\x10\x01\x20\x46\x12\x99\x00\x00\x01" };



void CDownLoadKNDlg::fReadFileMsg(CString m_FilePath)
{
	CStdioFile fin;
	//����txt�ļ�
	if (fin.Open(m_FilePath, CFile::modeNoTruncate | CFile::modeRead | CFile::typeBinary))
	{
		DWORD ReadLen;
		//fin.SeekToBegin();
		ReadLen = fin.SeekToEnd();
		//OutSprintEdit1("���ļ�����%d", ReadLen);
		pAllPackFile= (PE_DATA_PACK *)malloc(sizeof(PE_DATA_PACK)+ReadLen);
		if(pAllPackFile == NULL)
		{
			MessageBox("��ȡ�ļ�ʧ��[����ռ�ʧ��]");
		}
		fin.SeekToBegin();
		pAllPackFile->lenData=fin.Read(pAllPackFile->pData,ReadLen);
		fin.Close();
		if(pAllPackFile->lenData != ReadLen)
		{
			MessageBox("��ȡ�ļ�ʧ��[���ȴ�]");
			free(pAllPackFile);
			pAllPackFile=NULL;
		}
		strcpy_s(pAllPackFile->sName,m_FilePath.GetBuffer(0));
		pAllPackFile->offset = 0;
		//-----�����ļ�Md5ֵ-----------
	//	md5(pAllPackFile->pData,pAllPackFile->lenData,pAllPackFile->uMd5);
	}
}

void CDownLoadKNDlg::OnBnClickedButton3()
{
	char pBuf[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pBuf);
	CFileDialog dlg(true, ".ksp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "ksp�ļ�(*.ksp)|*.ksp");
	dlg.m_ofn.lpstrTitle = "��ѡ������ļ�";

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
	
	//��ȡ��ǰCOM����
	m_ComList.GetLBText(m_ComList.GetCurSel(), strComName);

	// �򿪡��رմ���
	if (!m_serial.IsOpen()) {
		ret = m_serial.OpenCom(strComName, 115200);
		if (ret != 0) {
			MessageBox("�򿪴���ʧ��");
			FindComPort();
			return 1;
		}
		OutSprintEdit1("�򿪴���[%s]�ɹ�", strComName.GetBuffer(0));
	}
	else {
		//�رմ���
		m_serial.CloseCom();
		FindComPort();          //ÿ�ιرպ�����ö�ٴ���
		OutPutEdit1("�����ѹر�");
	}

	// ������ֹ�޸�COM
	pWnd = GetDlgItem(IDC_COMBO1);
	pWnd->EnableWindow(!m_serial.IsOpen());
	return 0;
}

int CDownLoadKNDlg::CloseComm()
{
	CWnd *pWnd;
	//�رմ���
	m_serial.CloseCom();
	FindComPort();          //ÿ�ιرպ�����ö�ٴ���
	OutPutEdit1("�����ѹر�");

	// ������ֹ�޸�COM
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

// �̺߳�����ѭ�������������ְ�
DWORD WINAPI CDownLoadKNDlg::ThreadFunc(LPVOID pParam)
{
	CDownLoadKNDlg *dlg = (CDownLoadKNDlg *)pParam;
	DWORD ReadLen,DataLen;
	int ret;
	BYTE sBuffSend[4096+64],sBuffRecv[512],*pRecvData,*p;

	if (NULL == dlg) {
		//Trace("�����̻߳�ȡ���Ի���ָ��ʧ��.");
		dlg->m_thread = NULL;
		return -1;
	}
	dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	if (0 == (ret=dlg->m_serial.SetupParam(115200, NOPARITY, 8, ONESTOPBIT)))
	{
		while(1)
		{
			//m_Port.SetupParam(nPort,115200,'N','8','1');
			dlg->OutSprintEdit1("�ȴ��ն�����...");
			ReadLen = sizeof(sBuffRecv);
			pRecvData = dlg->m_serial.SupReadPackCom(sBuffRecv,&ReadLen,30*1000);
			if(pRecvData == NULL)
				return (-1100 + 1);
			pRecvData[ReadLen]='\0';
			p = (BYTE*)strstr((char*)pRecvData,"->hello");
			if(p)
			{
				dlg->OutSprintEdit1("�յ�����[%s]",pRecvData);
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

			
			dlg->OutSprintEdit1("��ʼ����оƬ...");
			
			dlg->OutSprintEdit1("��ʼ���س���...");
			
			dlg->OutSprintEdit1("���س������");
			//SNautoInc();  //���к��Զ�����
		}
	}
	else
	{
		dlg->OutSprintEdit1("�򿪴���ʧ��[%d]", ret);
	}
	
	dlg->m_serial.CloseCom();

	dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	dlg->m_thread = NULL;
	return 1;
}

// �������������߳�
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
		OutSprintEdit1("δ�򿪳����ļ���");
		return;
	}
	if (m_thread)
		return;
	//��ȡ��ǰCOM����
	m_ComList.GetLBText(m_ComList.GetCurSel(), strComName);
	// �򿪡��رմ���
	m_serial.SetComm(strComName);
	OutSprintEdit1("�򿪴���[%s]...", strComName);

	if(FALSE == StartDownLoadCode())
	{
		m_serial.CloseCom();
	}
}


void CDownLoadKNDlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CDownLoadKNDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
