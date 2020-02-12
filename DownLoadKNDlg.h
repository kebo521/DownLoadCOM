
// DownLoadKNDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "comm.h"
#include "afxcmn.h"

typedef struct _PE_DATA_PACK
{
	char sName[64];
	BYTE uMd5[16];
	int	offset;
	int	lenData;
	BYTE pData[0];
}PE_DATA_PACK;
// CDownLoadKNDlg �Ի���
class CDownLoadKNDlg : public CDialogEx
{
// ����
public:
	CDownLoadKNDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CDownLoadKNDlg();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DOWNLOADKN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CPdkbdoc m_serial;          // ���ڶ���
	HANDLE m_thread=NULL;            // �����߳�ID
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString sOutPutMsg;
	CComboBox m_ComList;
	CEdit sOutPutCtrl;
	
	PE_DATA_PACK *pAllPackFile = NULL;

	
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnCbnSelchangeCombo1();
	void RefreshProgress(WPARAM Pos);
	LRESULT OnOutPutProgress(WPARAM wParam, LPARAM lParam);
	LRESULT OnOutPutEditUpdate(WPARAM wParam, LPARAM lParam);
	void RefreshDisplayEdit1(void);
	void OutPutEdit1(char* pStr);
	void OutSprintEdit1(const char *format, ...);
	void OutPutHexEdit1(char *pTitle, BYTE *pBcd, int len);
	void FindComPort(void);

	unsigned long g_FontAdd = 0;
	static DWORD WINAPI ThreadFunc(LPVOID pParam);
	BOOL StartDownLoadCode(void);
	
	int OpenComm();	//�򿪴���
	int CloseComm();//�رմ���
	void fReadFileMsg(CString m_FilePath);
	afx_msg void OnBnClickedButton1();
	CProgressCtrl ProgressDown;
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
};
