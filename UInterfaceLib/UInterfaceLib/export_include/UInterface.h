#pragma  once

//define
#define lib_max(a,b)            (((a) > (b)) ? (a) : (b))
#define lib_min(a,b)            (((a) < (b)) ? (a) : (b))

#define  VIDEO_WIDTH		640
#define  VIDEO_HEIGHT		480

#define FORMAT_HEX			1
#define FORMAT_ASC			2
#define FORMAT_HEX_SPACE	3
#define FORMAT_ASC_SPACE	4

enum ULIB_RETURN
{
	ULIB_RETURN_FALSE			= -1,
	ULIB_RETURN_TRUE			= 0,
	ULIB_RETURN_PARAMETER_NULL 	= 2,
};

#define ULIB_FALSE		-1
#define ULIB_TRUE		0

#define ULIB_DB_TYPE_SQLITE						1
#define ULIB_DB_TYPE_MYSQL						2
#define ULIB_DB_TYPE_SQLSERVICE					3
#define ULIB_DB_TYPE_MONGODB					4

#include <string>
using namespace std;
#include <vector>
#include <map>

#include "UInterface/UOwnCtrlDefine.h"
#include "UInterface/UDataBase.h"
#include "UInterface/UCommonToolsDefine.h"
#pragma comment(lib, "UInterface/UInterfaceLib.lib")


/*************************************************
Author��			Liuxin
Description��	�������
Input:			��CString�������·��
Return��			��CString����������Ϣ
**************************************************/
extern "C" __declspec(dllimport) CString  UpdateCheck(CString csFilePath);


/************************************************* 
Function:				InitMessageDlg
Description:		��ʼ����Ϣ��ʾ�Ի���
Input:					void 
Return:					int���ɹ��󷵻�1������Ϊ����
Others:					NULL
*************************************************/  
extern "C" __declspec(dllimport) int  InitMessageDlg();

/************************************************* 
Function:				ShowWindow
Description:		������ʾ����
Input:					int�� SW_HIDE,SW_NORMAL,SW_SHOW,����Ի���ShowWindow����
Return:					int���ɹ��󷵻�1������Ϊ����
Others:					NULL
*************************************************/  
extern "C" __declspec(dllimport) int  ShowMessageWindow(int iShowMode);

/************************************************* 
Function:				ShowMessage
Description:		��Ϣ��ʾ����
Input:					CString��Ҫ��ʾ����Ϣ����  
								int����ʾģʽ������������ɫ
										SHOW_MESSAGE_COLOR_SYS��ϵͳ��Ϣ
										SHOW_MESSAGE_COLOR_SEND �� ������Ϣ
										SHOW_MESSAGE_COLOR_RECV��������Ϣ
Return:					int���ɹ��󷵻�1������Ϊ����
Others:					NULL
*************************************************/  
#define SHOW_MESSAGE_COLOR_SYS		0
#define SHOW_MESSAGE_COLOR_SEND		1
#define SHOW_MESSAGE_COLOR_RECV		2
extern "C" __declspec(dllimport) int  ShowMessage(CString csMessage, int iMode);

/************************************************* 
Function:				ShowMessage
Description:		��Ϣ�����ƶ�
Input:					CRect��Ҫ��ʾ�ķ�Χ  
Return:					int���ɹ��󷵻�1������Ϊ����
Others:					NULL
*************************************************/  
extern "C" __declspec(dllimport) int  MoveMessageWindow(CRect rect);