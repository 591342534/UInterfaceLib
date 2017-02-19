#include "StdAfx.h"

CCommTools* CCommTools::m_Instance = nullptr;

/*******************************************
* Brief		: ���ֽ��ִ�תUCHAR
* Param	: CString
* Return	: *UCHAR
* Date		: 2014.10
* Author	: Liuxin-Uicctech
* Other		: 
* Memo	: New
*******************************************/
UCHAR *CCommTools::string_W2Byte(CString csframe)
{
	int iFramelen = csframe.GetLength() / 2;
	UCHAR *pFrame16 = new UCHAR[iFramelen];

	/* ���ֽ�ת���ֽ� */
	int iLen = WideCharToMultiByte(CP_ACP, 0, csframe, csframe.GetLength(), NULL, 0, NULL, NULL);
	char *pRecvdatabuf = new char[iLen + 1];
	WideCharToMultiByte(CP_ACP, 0, csframe, csframe.GetLength(), pRecvdatabuf, iLen, NULL, NULL);
	pRecvdatabuf[iLen] = '\0';

	string strFrame(pRecvdatabuf, iLen);
	for (int i = 0; i < iLen / 2 ; i++)
	{
		string strBuf;
		strBuf = strFrame.substr(i * 2, 2);		//���ز����ִ�
		pFrame16[i] = (UCHAR)strtol(strBuf.c_str(), NULL, 16);		//ת16����
	}

	delete pRecvdatabuf;
	pRecvdatabuf = NULL;
	return pFrame16;
}

/*******************************************
* Brief		: ��������
* Param	: *CFont  long  CString
* Return	: Void
* Date		: 2014.10
* Author	: Liuxin-Uicctech
* Other		: 
* Memo	: New
*******************************************/
void CCommTools::windows_setfont(CFont *pfont, long lHeight, CString csFaceName)
{
	LOGFONT logfont;
	logfont.lfHeight			= lHeight;			//����߶�
	logfont.lfWeight			= 500;				//������
	logfont.lfUnderline			= 0;					//�Ƿ���»���
	logfont.lfStrikeOut			= 0;
	logfont.lfWidth				= 0;
	logfont.lfEscapement		= 0;
	logfont.lfOrientation		= 0;
	logfont.lfItalic			= 0;
	logfont.lfCharSet			= 134; 
	logfont.lfOutPrecision		= 3;
	logfont.lfClipPrecision		= 2; 
	logfont.lfQuality			= 100;					//�����ϸ
	logfont.lfPitchAndFamily	=2; 

	//��������
	#ifdef UNICODE
		wcscpy_s(logfont.lfFaceName, csFaceName); 
	#else
		strcpy(logfont.lfFaceName, csFaceName); 
	#endif
	
	pfont->Detach();
	pfont->CreateFontIndirect(&logfont);
}

void CCommTools::windows_setfont( CFont *pfont, long lHeight, long lWeight, CString csFaceName )
{
	LOGFONT logfont;
	logfont.lfHeight			= lHeight;			//����߶�
	logfont.lfWeight			= lWeight;				//������
	logfont.lfUnderline			= 0;					//�Ƿ���»���
	logfont.lfStrikeOut			= 0;
	logfont.lfWidth				= 0;
	logfont.lfEscapement		= 0;
	logfont.lfOrientation		= 0;
	logfont.lfItalic			= 0;
	logfont.lfCharSet			= 134; 
	logfont.lfOutPrecision		= 3;
	logfont.lfClipPrecision		= 2; 
	logfont.lfQuality			= 100;					//�����ϸ
	logfont.lfPitchAndFamily	= 2; 

	//��������
#ifdef UNICODE
	wcscpy_s(logfont.lfFaceName, csFaceName); 
#else
	strcpy(logfont.lfFaceName, csFaceName); 
#endif

	pfont->Detach();
	pfont->CreateFontIndirect(&logfont);
}

/*************************************************
Function:       check_bcc
Description:    ���У��
Input:          ��unsigned char *������������
				��int�����ݳ���
Output:         null
Return:         ��unsigned char����У����
Others:         null
Author:			Liuxin
*************************************************/
unsigned char CCommTools::check_sum(unsigned char *pData, int iLen, int iOffset)
{
	unsigned char nSum = 0xFF;
	if ( pData == nullptr || iLen <= 0 )
	{
		return nSum;
	}

	if ( iOffset < 0 )
	{
		iOffset = 0;
	}
	
	for ( int i = iOffset; i < iLen; i++ )
	{
		nSum += pData[i];
	}

	return nSum & 0xFF;
}

/*************************************************
Function:       windows_show2List
Description:    ����Ϣ��ʾ��List��
Input:          ��CListBox *���������б�
				��CString������
Output:         null
Return:         null
Others:         null
Author:			Liuxin
*************************************************/
void CCommTools::windows_show2List(CListBox *pList, CString csMessage)
{
	if ( pList == nullptr )
	{
		return;
	}

	int nIndex = pList->GetCount();
	pList->InsertString(nIndex, csMessage); 
	pList->SetCurSel(nIndex);
}


CString  CCommTools::string_ucharformat(BYTE *pBuf, int iMode, int iLen)
{
	if( pBuf == nullptr || iLen <= 0 )
	{
		return _T("");
	}

	CString csTmp;
	CString csData;

	switch ( iMode )
	{
	case FORMAT_HEX:
		for (int  i = 0; i < iLen; i++)
		{
			csTmp.Format(_T("%02X"), pBuf[i]);
			csData += csTmp;
		}
		break;

	case FORMAT_ASC:
		for (int  i = 0; i < iLen; i++)
		{
			csTmp.Format(_T("%c"), pBuf[i]);
			csData += csTmp;
		}
		break;

	case FORMAT_HEX_SPACE:
		for (int  i = 0; i < iLen; i++)
		{
			csTmp.Format(_T("%02X "), pBuf[i]);
			csData += csTmp;
		}
		break;

	case FORMAT_ASC_SPACE:
		for (int  i = 0; i < iLen; i++)
		{
			csTmp.Format(_T("%c "), pBuf[i]);
			csData += csTmp;
		}
		break;

	default:
		break;
	}

	return csData;
}

/*************************************************
Function:       check_bcc
Description:    ���У��
Input:          ��unsigned char *������������
				��int�����ݳ���
Output:         null
Return:         ��unsigned char����У����
Others:         null
Author:			Liuxin
*************************************************/
unsigned char CCommTools::check_bcc(unsigned char *pData, int iLen, int iOffset) 
{ 
	unsigned char nResult = 0xFF;
	if ( pData == nullptr || iLen <= 0 )
	{
		return nResult;
	}

	if ( iOffset < 0 )
	{
		iOffset = 0;
	}

	for ( int i = iOffset; i < iLen; i++ ) 
	{ 
		nResult ^= pData[i]; 
	} 

	return nResult & 0xFF;
} 


char* CCommTools::string_Encode2UTF8(const char* mbcsStr) 
{ 
	wchar_t*  wideStr; 
	char*   utf8Str; 
	int   charLen; 

	charLen = MultiByteToWideChar(CP_UTF8, 0, mbcsStr, -1, NULL, 0); 
	wideStr = (wchar_t*) malloc(sizeof(wchar_t)*charLen); 
	MultiByteToWideChar(CP_ACP, 0, mbcsStr, -1, wideStr, charLen); 

	charLen = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, NULL, 0, NULL, NULL); 

	utf8Str = (char*) malloc(charLen);

	WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, utf8Str, charLen, NULL, NULL);


	free(wideStr); 
	return utf8Str;
} 

/*************************************************
Function:       date_getloctime
Description:    ���ʱ��
Input:          ��int������ʽ������ 
Output:         null
Return:         ��string��������ʱ���ִ�
Others:         null
Author:			Liuxin
*************************************************/
string CCommTools::date_getloctime(int iMode)
{
	std::string strTime = boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());  

	switch ( iMode )
	{
	case 0:
		{
			int pos = strTime.find('T');  
			strTime.replace(pos, 1, std::string("-"));  
			strTime.replace(pos + 3, 0,std::string(":"));  
			strTime.replace(pos + 6, 0, std::string(":"));
		}
		break;

	case 1:
		{
			int pos = strTime.find('T');  
			strTime.replace(pos, 1, std::string("_"));  
			strTime.replace(pos + 3, 0,std::string("_"));  
			strTime.replace(pos + 6, 0, std::string("_"));
		}
		break;

	default:
		break;
	}

	return strTime;
}

/*************************************************
Function:       date_getlocdate
Description:    �������
Input:          ��int������ʽ������ 
Output:         null
Return:         ��string�������������ִ�
Others:         null
Author:			Liuxin
*************************************************/
string CCommTools::date_getlocdate(int iMode)
{
	std::string strDate = boost::gregorian::to_iso_string(boost::gregorian::day_clock::local_day()); 

	switch ( iMode )
	{
	case 0:
		{
			int pos = strDate.find('T');  
			strDate.replace(pos, 1, std::string("-"));  
			strDate.replace(pos + 3, 0,std::string(":"));  
			strDate.replace(pos + 6, 0, std::string(":"));
		}
		break;

	case 1:
		{
			int pos = strDate.find('T');  
			strDate.replace(pos, 1, std::string("_"));  
			strDate.replace(pos + 3, 0,std::string("_"));  
			strDate.replace(pos + 6, 0, std::string("_"));
		}
		break;

	default:
		break;
	}

	return strDate;
}

/*************************************************
Function:       string_A2W
Description:    ���ֽ�ת���ֽ�
Input:          ��CString�������ֽ��ִ� 
Output:         null
Return:         ��char*�������ֽ��ִ�
Others:         null
Author:			Liuxin
*************************************************/
char*  CCommTools::string_W2A(CString csData)
{
	if ( csData.GetLength() <= 0 )
	{
		return "";
	}

	int ilen = WideCharToMultiByte(CP_ACP, 0, csData, -1, NULL, 0, NULL, NULL);
	char *pbuf = new char[ilen + 1];
	WideCharToMultiByte(CP_ACP, 0, csData, -1, pbuf, ilen, NULL, NULL);
	pbuf[ilen]='\0'; 

	return pbuf;
}

template<class T>
void CCommTools::to_string(string & result,const T& t)
{
	ostringstream oss;
	oss << t;
	result = oss.str();
}

template<class out_type, class in_value>
out_type CCommTools::string_convert(const in_value & t)
{
	stringstream stream;
	stream << t;
	out_type result;
	stream >> result;
	return result;
}


/*************************************************
Function:       string_A2W
Description:    ���ֽ�ת���ֽ�
Input:          ��char *�������ֽ��ִ� 
				��int�����ִ�����
Output:         null
Return:         ��WCHAR*�������ֽ��ִ�
Others:         null
Author:			Liuxin
*************************************************/
WCHAR* CCommTools::string_A2W(char *pstr, int iStrlen)
{
	if ( pstr == nullptr || iStrlen <= 0)
	{
		return _T("");
	}

	int ilen = MultiByteToWideChar(CP_ACP, 0, pstr, iStrlen, NULL, 0);
	WCHAR *pWbuf = new wchar_t[ilen + 1];
	MultiByteToWideChar(CP_ACP, 0,  pstr, ilen, pWbuf, ilen);
	pWbuf[ilen]='\0'; 

	return pWbuf;
}

/*************************************************
Function:       string_A2W
Description:    ���ֽ�ת���ֽ�
Input:          ��char *�������ֽ��ִ� 
				��int�����ִ�����
Output:         null
Return:         ��WCHAR*�������ֽ��ִ�
Others:         null
Author:			Liuxin
*************************************************/
WCHAR* CCommTools::string_A2W(const char *pstr, int iStrlen)
{
	if ( pstr == nullptr || iStrlen <= 0)
	{
		return _T("");
	}

	int ilen = MultiByteToWideChar(CP_ACP, 0, pstr, -1, NULL, 0);
	WCHAR *pWbuf = new wchar_t[ilen + 1];
	MultiByteToWideChar(CP_ACP, 0,  pstr, -1, pWbuf, ilen);
	pWbuf[ilen]='\0'; 

	return pWbuf;
}

/*************************************************
Function:       string_W2UTF8
Description:    CStringתUTF8
Input:          ��CString����CString�ִ�
Output:         null
Return:         ��char*����UTF8�ִ�
Others:         null
Author:			Liuxin
*************************************************/
char* CCommTools::string_W2UTF8(CString csBuffer) 
{ 
	if ( csBuffer.IsEmpty() )
	{
		return "";
	}

	int iLen = WideCharToMultiByte(CP_UTF8, 0, csBuffer, -1, NULL, 0, NULL, NULL); 
	char *utf8Str = new char[iLen + 1];
	WideCharToMultiByte(CP_UTF8, 0, csBuffer, -1, utf8Str, iLen, NULL, NULL);
	utf8Str[iLen]='\0'; 

	return utf8Str;
} 

/*************************************************
Function:       windows_transparentwnd
Description:    ����͸��
Input:          hWnd ����Ϊ͸������
				clr ͸����ɫ
				byAlpha ͸��������0-255
				dwFlags==LWA_COLORKEY clr��Ч
				dwFlags==LWA_ALPHA byAlpha��Ч
Output:         null
Return:         null
Others:         null
Author:			Liuxin
*************************************************/
void CCommTools::windows_transparentwnd(HWND hWnd,COLORREF clr,BYTE byAlpha,DWORD dwFlags)
{  
	typedef BOOL (FAR PASCAL *Fun)(HWND,COLORREF,BYTE,DWORD);  
	Fun SetTransparentFun;  
	HMODULE hModule = LoadLibrary(_T("User32.DLL"));
	if(hModule)  
	{  
		SetTransparentFun=(Fun)GetProcAddress(hModule,"SetLayeredWindowAttributes");  
		if(SetTransparentFun&&hWnd)  
		{  
			SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE)|0x80000L);  
			SetTransparentFun(hWnd,clr,byAlpha,dwFlags);  
		}  
		FreeLibrary(hModule);  
	}  
}

/*************************************************
Function:       imagedeal_closewindow
Description:    �ر�opencv����
Input:          ��char *����������
Output:         null
Return:         null
Others:         null
Author:			Liuxin
*************************************************/
void CCommTools::imagedeal_closewindow(char *title)
{
	HWND hCvWnd = (HWND)cvGetWindowHandle(title);
	CWnd* pCvWnd = CWnd::FromHandle(hCvWnd);
	if(pCvWnd != nullptr)
	{
		CMenu* pSystemMenu = pCvWnd->GetParent()->GetSystemMenu(FALSE);
		if(pSystemMenu != NULL)
		{
			pSystemMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND|MF_GRAYED);
		}
	} 
}

/*************************************************
Function:       imagedeal_ispic
Description:    �ж��ļ��Ƿ�ΪͼƬ�ļ�
Input:          ��CString�����ļ���
Output:         null
Return:         ��bool����true����
Others:         null
Author:			Liuxin
*************************************************/
bool CCommTools::imagedeal_ispic(CString csType)
{
	CString csTmp = csType.MakeLower();

	bool bRet = false;
	if ( csTmp.Find(_T(".png")) != -1 || csTmp.Find(_T(".jpg")) != -1  || csTmp.Find(_T(".bmp")) != -1 )
	{
		bRet = true;
	}

	return bRet;
}

/*************************************************
Function:       windows_filedlg
Description:    ͨ�����㣬�����н�
Input:          ��CString�����ļ�����
Output:         ��CStringArray&�����ļ�·������
Return:         null
Others:         null
Author:			Liuxin
*************************************************/
void CCommTools::windows_filedlg(CString csFileFilter, CStringArray &csPath)
{
	csPath.RemoveAll();

	TCHAR szFullPath[MAX_PATH] = {0x00}; 
	GetModuleFileName(NULL, szFullPath, sizeof(szFullPath)); 
	CString csFilePath = szFullPath;
	csFilePath = csFilePath.Left(csFilePath.ReverseFind('\\'));
	csFilePath += _T("\\");
	CFileDialog dlg(true, NULL, csFilePath, OFN_HIDEREADONLY|OFN_ALLOWMULTISELECT|OFN_NOCHANGEDIR, csFileFilter);
	POSITION pos = dlg.GetStartPosition();
	if ( dlg.DoModal() == IDOK )
	{
		while ( pos != NULL )
		{
			csFilePath = dlg.GetNextPathName(pos);
			csPath.Add(csFilePath);
		}
	}
}

/*************************************************
Function:       imagedeal_openPicDlg
Description:    ��ͼƬ�ļ�����Ի���
Input:          ��int������ȡ�������ƣ�Ĭ��Ϊ0����������
Output:         ��CStringArray&��·������
Return:         null
Others:         null
Author:			Liuxin
*************************************************/
void CCommTools::imagedeal_openpicdlg(CStringArray &csPath, int iCount)
{
	CString csFileFilter=_T("jpg (*.jpg);png (*.png);bmp (*.bmp)|*.jpg;*.png;*.bmp| All Files (*.*)|*.*||");
	this->windows_filedlg(csFileFilter, csPath);
	if ( iCount > 1 )
	{
		if ( csPath.GetCount() > iCount  )
		{
			csPath.RemoveAt(iCount, csPath.GetCount() - iCount);
		}
	}
}

/*************************************************
Function:       imagedeal_angle
Description:    ͨ�����㣬�����н�
Input:          ��CPoint������ʼ��������㣬�Լ�ͼ���е�
Output:         null
Return:         ��int�����н�
Others:         null
Author:			Liuxin
*************************************************/
int CCommTools::imagedeal_angle(CPoint center, CPoint startPoint, CPoint desPooint) 
{                  
	float dx1 = startPoint.x - center.x;     
	float dy1 = startPoint.y - center.y;        
	float dx2 = desPooint.x - startPoint.x;     
	float dy2 = desPooint.y - startPoint.y;        

	double c = (double)sqrt(dx1 * dx1 + dy1 * dy1) * (double)sqrt(dx2 * dx2 + dy2 * dy2);      
	if (c == 0) 
	{
		return -1;  
	}
	int iAngle = (int)acos((dx1 * dx2 + dy1 * dy2) / c);

	return iAngle;  
}

/*************************************************
Function:       imagedeal_direction
Description:    ͨ�����㣬�����ת����
Input:          ��CPoint������ʼ��������㣬�Լ�ͼ���е�
Output:         null
Return:         ��int����1��˳ʱ�� 2����ʱ��
Others:         null
Author:			Liuxin
*************************************************/
int CCommTools::imagedeal_direction(CPoint startPoint, CPoint endPoint, CPoint cenPoint)
{
	int iState = -1;
	if ( startPoint.x >= cenPoint.x )
	{
		if ( endPoint.y - startPoint.y < 0  )
		{
			iState = 1;
		}
		else
		{
			iState = 2;
		}
	}
	else if ( startPoint.x < cenPoint.x )
	{
		if ( endPoint.y - startPoint.y > 0  )
		{
			iState = 1;
		}
		else
		{
			iState = 2;
		}
	}

	return iState;
}

/*************************************************
Function:       imagedeal_showimage
Description:    ��ͼƬ��ʾ�ڿؼ�
Input:          ��IplImage*����ͼ��
				��CWnd*�����ؼ�
Output:         null
Return:         null
Others:         null
Author:			Liuxin
*************************************************/
void CCommTools::imagedeal_showimage(IplImage *image, CWnd* pWnd)
{
	if ( image == nullptr || image->imageData == nullptr || pWnd == nullptr)
	{
		return;
	}

	IplImage *imageTmp;
	imageTmp = cvCloneImage(image);    

	CDC* dc = pWnd->GetDC();

	HDC hDC= dc->GetSafeHdc(); 
	CRect rect;
	pWnd->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(imageTmp);
	cimg.DrawToHDC(hDC,&rect);

	dc = nullptr;
	cvReleaseImage(&imageTmp);
}

/*************************************************
Function:       windows_getmac
Description:    windows�»��mac��ַ
Input:          null
Output:         ��vector<CString> &����mac��ַ�飨���������mac��ַ��
Return:         ��ULIB_RETURN����ULIB_RETURN_TRUE�����
Others:         null
Author:			Liuxin
*************************************************/
ULIB_RETURN CCommTools::windows_getmac(vector<CString> &v_mac)
{
	ULIB_RETURN eRet = ULIB_RETURN_FALSE;
	v_mac.clear();

	ULONG nBufLen = sizeof(IP_ADAPTER_ADDRESSES);
	PIP_ADAPTER_ADDRESSES pAddress = new IP_ADAPTER_ADDRESSES();
	if ( pAddress == nullptr ) 
	{
		return eRet;
	}

	try
	{
		if(GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddress, &nBufLen) == ERROR_BUFFER_OVERFLOW)
		{
			free(pAddress);
			pAddress = (IP_ADAPTER_ADDRESSES*)malloc(nBufLen);
			if (pAddress == nullptr) 
			{
				return eRet;
			}	
		}

		if ( GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddress, &nBufLen) == NO_ERROR )
		{
			for ( PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddress; pCurrAddresses != NULL; pCurrAddresses = pCurrAddresses->Next )
			{
				if ( pCurrAddresses->PhysicalAddressLength != 6 )
				{
					continue;
				}	
				CString csMac;
				csMac.Format(_T("%02X%02X%02X%02X%02X%02X"), pCurrAddresses->PhysicalAddress[0], 
					pCurrAddresses->PhysicalAddress[1],
					pCurrAddresses->PhysicalAddress[2],
					pCurrAddresses->PhysicalAddress[3],
					pCurrAddresses->PhysicalAddress[4],
					pCurrAddresses->PhysicalAddress[5]);

				v_mac.push_back(csMac);
			}
		}

		delete pAddress;
		pAddress = nullptr;

		eRet = ULIB_RETURN_TRUE;
	}
	catch (CException* e)
	{
		
	}

	return eRet;
}

/*************************************************
Function:       windows_openURL
Description:    windows����Ĭ���������url
Input:          ��CString����URL
Output:         ��string &������������
Return:         ��ULIB_RETURN����ULIB_RETURN_TRUE�����
Others:         null
Author:			Liuxin
*************************************************/
ULIB_RETURN CCommTools::windows_openURL(CString csUrl)  
{  
	ULIB_RETURN eRet = ULIB_RETURN_FALSE;

	if( csUrl.IsEmpty() )
	{
		return eRet;
	}
		 
	HKEY  hkRoot, hSubKey;   //ע�����ؼ��ּ��ӹؼ���  
	CString ValueName;  
	unsigned char DataValue[MAX_PATH];  
	unsigned long cbValueNAME = MAX_PATH;  
	unsigned long cbDataNAME = MAX_PATH;      
	CString ShellChar;//����������  
	DWORD  dwType = 0;  
	//��ע�����ؼ���  
	if(RegOpenKey(HKEY_CLASSES_ROOT,NULL,&hkRoot) == ERROR_SUCCESS)  
	{  
		//���ӹؼ���  
		if(RegOpenKeyEx(hkRoot, _T("htmlfile\\Shell\\open\\command"), 0, KEY_ALL_ACCESS, &hSubKey) == ERROR_SUCCESS)  
		{  
			RegEnumValue(hSubKey, 0, ValueName.GetBuffer(), &cbValueNAME, NULL, &dwType, DataValue, &cbDataNAME);  
			ValueName.ReleaseBuffer();  

			//���ò�����ֵ  
			ShellChar = (char *)DataValue;  
			if(ShellChar == "\"")  
			{  
				ShellExecute(NULL, _T("open"), csUrl, NULL, NULL, SW_SHOWNORMAL); 
			}  
			else  
			{  
				int index = ShellChar.Find('"');
				if ( index != -1 )
				{
					index = ShellChar.Find('"', index + 1);
					if(  index != 0 )
					{
						ShellChar = ShellChar.Left(index + 1);
					}
				}

				ShellChar = ShellChar + csUrl;  
				WinExec((LPCSTR)ShellChar.GetBuffer(MAX_PATH),SW_SHOW);  
				ShellChar.ReleaseBuffer();  
			}  
			eRet = ULIB_RETURN_TRUE;
		}  
		else  
		{  
			RegCloseKey(hSubKey);  
			RegCloseKey(hkRoot);             
			ShellExecute(NULL, _T("open"),csUrl, NULL, NULL, SW_SHOWNORMAL);  
			eRet = ULIB_RETURN_TRUE;
		}
	}	

	return eRet;
}

/*************************************************
Function:       path_filename
Description:    ��·���л�ȡ�ļ���
Input:          ��string�����ļ�·��
Output:         ��string &�����ļ���
Return:         ��ULIB_RETURN����ULIB_RETURN_TRUE�����
Others:         null
Author:			Liuxin
*************************************************/ 
ULIB_RETURN CCommTools::path_filename(string strPath, string &strfilename)
{
	ULIB_RETURN eRet = ULIB_RETURN_FALSE;

	if ( strPath.empty() )
	{
		return ULIB_RETURN_PARAMETER_NULL;
	}

	boost::filesystem::path path(strPath);
	strfilename = path.filename().string();

	return ULIB_RETURN_TRUE;
}

/*************************************************
Function:       path_extension
Description:    ��ȡ�ļ���չ��
Input:          ��string�����ļ�·��
Output:         ��string &������չ��
Return:         ��ULIB_RETURN����ULIB_RETURN_TRUE�����
Others:         null
Author:			Liuxin
*************************************************/ 
ULIB_RETURN CCommTools::path_extension(string strPath, string &strextension)
{
	ULIB_RETURN eRet = ULIB_RETURN_FALSE;

	if ( strPath.empty() )
	{
		return ULIB_RETURN_PARAMETER_NULL;
	}

	boost::filesystem::path path(strPath);
	strextension = path.extension().string();

	return ULIB_RETURN_TRUE;
}

/*************************************************
Function:       path_stem
Description:    ��ȡ����չ�����ļ���
Input:          ��string�����ļ�·��
Output:         ��string &�����ļ���
Return:         ��ULIB_RETURN����ULIB_RETURN_TRUE�����
Others:         null
Author:			Liuxin
*************************************************/ 
ULIB_RETURN CCommTools::path_stem(string strPath, string &strstem)
{
	ULIB_RETURN eRet = ULIB_RETURN_FALSE;

	if ( strPath.empty() )
	{
		return ULIB_RETURN_PARAMETER_NULL;
	}

	boost::filesystem::path path(strPath);
	strstem = path.stem().string();

	return ULIB_RETURN_TRUE;
}

/*************************************************
Function:       path_withoutfilename
Description:    ��ȡ���ļ�����·��
Input:          ��string�����ļ�·��
Output:         ��string &����·��
Return:         ��ULIB_RETURN����ULIB_RETURN_TRUE�����
Others:         null
Author:			Liuxin
*************************************************/ 
ULIB_RETURN CCommTools::path_withoutfilename(string strPath, string &strDesPath)
{
	ULIB_RETURN eRet = ULIB_RETURN_FALSE;

	if ( strPath.empty() )
	{
		return ULIB_RETURN_PARAMETER_NULL;
	}

	boost::filesystem::path path(strPath);
	strDesPath = path.remove_filename().string();

	return ULIB_RETURN_TRUE;
}

/*************************************************
Function:       file_exist
Description:    ���麯���Ƿ����
Input:          ��string�����ļ�·��
Output:         ��string &������������
Return:         ��ULIB_RETURN����ULIB_RETURN_TRUE������
Others:         null
Author:			Liuxin
*************************************************/ 
ULIB_RETURN CCommTools::file_exist(string strPath, string &strError)
{
	bool bRet = false;
	ULIB_RETURN eRet = ULIB_RETURN_FALSE;

	if ( strPath.empty() )
	{
		return eRet;
	}

	boost::filesystem::path path(strPath);
	if ( &strError == nullptr )
	{
		bRet = boost::filesystem::exists(strPath);
	}
	else	
	{
		boost::system::error_code ec;
		bRet = boost::filesystem::exists(strPath, ec);
		if ( ec )
		{
			strError = ec.message();
		}
	}

	if ( bRet )
	{
		eRet = ULIB_RETURN_TRUE;
	}

	return eRet;
}

/*************************************************
Function:       file_currentpath
Description:    ���麯���Ƿ����
Input:          ��string�����ļ�·��
Output:         ��string &������������
Return:         ��ULIB_RETURN����ULIB_RETURN_TRUE�����
Others:         null
Author:			Liuxin
*************************************************/ 
ULIB_RETURN CCommTools::file_currentpath(string &strcurrentpath, string &strError)
{
	ULIB_RETURN eRet = ULIB_RETURN_FALSE;

	boost::system::error_code ec;
	boost::filesystem::path path = boost::filesystem::current_path(ec);
	if ( ec )
	{
		strError = ec.message();
	}
	else
	{
		strcurrentpath = path.string();
		eRet = ULIB_RETURN_TRUE;
	}

	return eRet;
}

/*************************************************
Function:       file_currentpath
Description:    ���麯���Ƿ����
Input:          ��string�����ļ�·��
Output:         ��string &������������
Return:         ��ULIB_RETURN����ULIB_RETURN_TRUE�����
Others:         null
Author:			Liuxin
*************************************************/ 
ULIB_RETURN CCommTools::file_initialpath(string &strcurrentpath, string &strError)
{
	ULIB_RETURN eRet = ULIB_RETURN_FALSE;

	boost::system::error_code ec;
	boost::filesystem::path path = boost::filesystem::initial_path(ec);
	if ( ec )
	{
		strError = ec.message();
	}
	else
	{
		strcurrentpath = path.string();
		eRet = ULIB_RETURN_TRUE;
	}

	return eRet;
}

/*************************************************
Function:       file_remove
Description:    ���麯���Ƿ����
Input:          ��string�����ļ�·��
Output:         ��string &������������
Return:         ��ULIB_RETURN����ULIB_RETURN_TRUE�����
Others:         null
Author:			Liuxin
*************************************************/
ULIB_RETURN CCommTools::file_remove(string strPath, string &strError)
{
	ULIB_RETURN eRet = ULIB_RETURN_FALSE;

	boost::system::error_code ec;
	bool bRet = boost::filesystem::remove(strPath, ec);
	if ( !bRet )
	{
		strError = ec.message();
	}
	else
	{
		eRet = ULIB_RETURN_TRUE;
	}

	return eRet;
}

ULIB_RETURN CCommTools::file_creat(string strPath, string &strError)
{
	ULIB_RETURN eRet = ULIB_RETURN_FALSE;

	boost::system::error_code ec;
	bool bRet = boost::filesystem::create_directories(strPath, ec);
	if ( !bRet )
	{
		strError = ec.message();
	}
	else
	{
		eRet = ULIB_RETURN_TRUE;
	}

	return eRet;
}


























































