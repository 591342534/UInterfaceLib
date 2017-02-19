#pragma once

/************************************************* 
Copyright:forour
Author: 
Date:2015-06-28
Description:������ӿ�
**************************************************/  
class   CCommTools
{
public:
	CCommTools(void);
	~CCommTools(void);

public:
	/* �ִ�ת16���� */
	UCHAR *ToByte(CString csframe);

	/* �������� */
	void SetFont(CFont *pfont, long lHeight, CString csFaceName);

	/* У��� */
	UCHAR CheckSum(UCHAR *pRecvBuf, int iLen);

	/* ������ת16���� */
	CString BinaryToHex(CString strBin);

	/* ��ʾ��Ϣ��List */
	void ShowInList(CListBox *pList, CString csMessage);

	//void  MajorMap(CImage *pImage);

	CString FormatString(UCHAR *pBuf, int iMode, int iLen);

	CString Deal16ByteFomat(CString &csData);

	UCHAR BCC(UCHAR *pData, int ilen);

	char* CCommTools::EncodeToUTF8(const char* mbcsStr);

	std::string GetLocTime(int iMode = 0);

	std::string GetLocDate(int iMode = 0);

	static char*   WToA(CString csData);

	static WCHAR*  AToW(char *pstr, int ilen);

	char* WTUTF8(CString csBuffer);

	// hWnd ����Ϊ͸������
	// clr ͸����ɫ
	// byAlpha ͸��������0-255
	// dwFlags==LWA_COLORKEY clr��Ч
	// dwFlags==LWA_ALPHA byAlpha��Ч
	void SetTransparentWnd(HWND hWnd,COLORREF clr,BYTE byAlpha,DWORD dwFlags);

	void CloseDisable(char *title);

	BOOL DeleteMyFile(CString csPath);

	bool FindMyFile(CString csPath);

	bool ChkFileType(CString csType);

	void OpenFileWithDlg(CString csFileFilter, CStringArray &csPath);

	void OpenPicWithDlg(CStringArray &csPath);

	int GetAngle(CPoint center, CPoint startPoint, CPoint desPooint);

	int DealDirection(CPoint startPoint, CPoint endPoint);

	void WriteDataToIni(CString csA, CString csB, CString csC);

	bool FileIsExist(CString csPath);

	void SetiniPath(CString csPath)
	{
		m_csIniPath = csPath;
	}

protected:
	CString m_csIniPath;

private:
};

namespace ulib
{
	/************************************************* 
	Function:				InitTools
	Description:		��ʼ��������
	Input:					void 
	Return:					int���ɹ��󷵻�1������Ϊ����
	Others:					NULL
	*************************************************/  

	extern "C" __declspec(dllexport) CCommTools* InitTools();

	/************************************************* 
	Function:				ReleaseTools
	Description:		�ͷŹ�����
	Input:					void 
	Return:					int���ɹ��󷵻�1������Ϊ����
	Others:					NULL
	*************************************************/  
	extern "C" __declspec(dllexport) void ReleaseTools();
}