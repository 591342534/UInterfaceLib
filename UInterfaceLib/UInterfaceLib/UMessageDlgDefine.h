#pragma once
/************************************************* 
Copyright:forour
Author: 
Date:2015-06-28
Description:��Ϣ���ڲ����ӿ� 
**************************************************/  


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
extern "C" __declspec(dllimport) int ShowMessageWindow(int iShowMode);

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
extern "C" __declspec(dllimport) int ShowMessage(CString csMessage, int iMode);

/************************************************* 
Function:				ShowMessage
Description:		��Ϣ�����ƶ�
Input:					CRect��Ҫ��ʾ�ķ�Χ  
Return:					int���ɹ��󷵻�1������Ϊ����
Others:					NULL
*************************************************/  
extern "C" __declspec(dllimport) int MoveMessageWindow(CRect rect);