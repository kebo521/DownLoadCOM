
// DownLoadKNDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "comm.h"
#include "afxcmn.h"

typedef struct _PE_DATA_PACK
{
	struct _PE_DATA_PACK  *pNext;
	char sName[64];
	BYTE* pData;
	int	DataSize;
	int	line,unitNum;
}PE_DATA_PACK;
// CDownLoadKNDlg 对话框
class CDownLoadKNDlg : public CDialogEx
{
// 构造
public:
	CDownLoadKNDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CDownLoadKNDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DOWNLOADKN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CPdkbdoc m_serial;          // 串口对象
	HANDLE m_thread=NULL;            // 工作线程ID
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton3();
	CString sOutPutMsg;
	CComboBox m_ComList;
	CEdit sOutPutCtrl;
	
	PE_DATA_PACK *pAllPackFile = NULL;
	void RefreshProgress(WPARAM Pos);
	LRESULT OnOutPutProgress(WPARAM wParam, LPARAM lParam);
	LRESULT OnOutPutEditUpdate(WPARAM wParam, LPARAM lParam);
	void RefreshDisplayEdit1(void);
	void OutPutEdit1(char* pStr);
	void OutSprintEdit1(const char *format, ...);
	void OutPutHexEdit1(char *pTitle, BYTE *pBcd, int len);
	void FindComPort(void);

	unsigned long g_FontAdd = 0;
	DWORD FONT_GeneratePack(BYTE *si, DWORD silen, BYTE cmd, BYTE *so);
	int Font_ExecCmd(BYTE cmd, BYTE *s, DWORD len, BYTE tmOutSec);
	int EXIT_ExecCmd(BYTE tmOutSec);
	int Font_BuildLink(BYTE tmOutSec);
	void DownLoadKN();
	int Down_SendRecvPross(BYTE *s, DWORD len, BYTE *pRecv, DWORD rMixLen, int mTimeOut);
	int User_BuildLink();
	int User_EraseChip();
	int Down_DownDataPack(BYTE* pCodeAddress, BYTE *Data, DWORD DataLen);
	int User_DownCode();

	static DWORD WINAPI ThreadFunc(LPVOID pParam);
	BOOL StartDownLoadCode(void);
	
	int OpenComm();	//打开串口
	int CloseComm();//关闭串口
	void fReadFileMsg(CString m_FilePath);
	afx_msg void OnBnClickedButton1();
	CProgressCtrl ProgressDown;
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton2();
};
