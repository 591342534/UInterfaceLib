// SerialCom.cpp: implementation of the CSerialCom class.
// 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//���ڽ����߳�
UINT CSerialCom::ComMonitor(LPVOID pParam)
{
	OVERLAPPED stOverlap = {0};
	stOverlap.hEvent = NULL;
	DWORD dwMaskFlag;
	DWORD dwCommEvent;
	DWORD dwByteToRead;
	DWORD dwRes;
	BYTE rechar[1]={0xff};
	bool bWaitingOnStatusHandle = false;
	CSerialCom * pCom = (CSerialCom *)pParam;

	dwMaskFlag = EV_RXCHAR | EV_CTS;
	if ( !::SetCommMask(pCom->m_hCom,dwMaskFlag) )
	{
		return 0;
	}
	stOverlap.hEvent = CreateEvent(NULL, true, false, NULL);
	if ( stOverlap.hEvent == NULL )
	{
		return 0;
	}

	while ( pCom->m_bIsRuning )
	{
		if ( !WaitCommEvent(pCom->m_hCom, &dwCommEvent, &stOverlap) )
		{
			if ( ::GetLastError() == ERROR_IO_PENDING )
			{
				bWaitingOnStatusHandle = true;
			}
            else
				break;
		}
		else
		{
			bWaitingOnStatusHandle = false;
		}

		if ( bWaitingOnStatusHandle ) 
		{
			dwRes = WaitForSingleObject(stOverlap.hEvent, 3000);
			while ( (pCom->m_bIsRuning) && (dwRes==WAIT_TIMEOUT) )
			{
				dwRes = WaitForSingleObject(stOverlap.hEvent, 3000);
			}

			GetCommMask(pCom->m_hCom,&dwCommEvent);
			if ( (dwCommEvent & EV_RXCHAR) == EV_RXCHAR )
			{
				do
				{
					COMSTAT stcomstat;
					DWORD dwError = 0;

					ClearCommError(pCom->m_hCom, &dwError, &stcomstat);
					if ( stcomstat.cbInQue == 0 )
					{
						if (pCom->m_pParser!=NULL)
						{
							pCom->m_pParser->SetRecvBuf();
						}
						break;
					}
						
					ReadFile(pCom->m_hCom, rechar, 1, &dwByteToRead, &stOverlap);
					
					if ( !pCom->m_bIsRuning )
					{
						break;
					}

					pCom->ParseByte(rechar);
				}
				while (true);
			}
			if (dwRes==WAIT_OBJECT_0)
			{
				if (!GetOverlappedResult(pCom->m_hCom, &stOverlap, &dwByteToRead, false))
				{
					do
					{
						COMSTAT stcomstat;
						DWORD dwError = 0;
						
						ClearCommError(pCom->m_hCom,&dwError,&stcomstat);
						if ( stcomstat.cbInQue==0 )
						{
							//if (pCom->m_pParser!=NULL)
							//{
							//	pCom->m_pParser->SetRecvBuf();
							//}
							break;
						}
							
						ReadFile(pCom->m_hCom,rechar,1,&dwByteToRead,&stOverlap);

						if ( !pCom->m_bIsRuning )
						{
							break;
						}

						if ( dwByteToRead>0 )
						{
							pCom->ParseByte(rechar);
						}
						else
						{
							CloseHandle(stOverlap.hEvent);
							return 0;
						}

					}
					while (true);
				}
			}
		}
	}
	CloseHandle(stOverlap.hEvent);
	return 0;
}

//���ڷ����߳�
UINT CSerialCom::SendingThread(LPVOID pParam)
{
	CSerialCom *pCom = (CSerialCom *) pParam;
	POSITION pos;

	while ( pCom->m_bIsRuning )
	{
		EnterCriticalSection(&pCom->m_cComCS);
		pos = pCom->m_cmdlist.GetHeadPosition();
		if ( pos != NULL )
		{
			CMDBUF * pCbuf = (CMDBUF *)pCom->m_cmdlist.GetNext(pos);

			//������ͻغ�δ��������ȴ�
/*	
			while (pCom->m_bSending == TRUE)
			{
				Sleep(1);
				cnt ++;
				if (cnt > 1024)
					break;
			}
			cnt = 0;
//*/
			//���ͻ����б����ָ��
			if ( pCbuf != NULL )
			{
				pCom->ComSend(pCbuf->pCmdBuf,pCbuf->nLen);

				//�ͷŻ����б����ѷ���ָ��ռ�õ���Դ
				pCom->m_cmdlist.RemoveHead();
				delete []pCbuf->pCmdBuf;
				delete pCbuf;
			}
		}
		LeaveCriticalSection(&pCom->m_cComCS);
		Sleep(5);
	}

	return 0;
}


CSerialCom::CSerialCom()
{
	//Ĭ�ϴ�������
	this->m_iBaudrate = CBR_115200;
	this->m_bytesize   = 8;
	this->m_stopbits   =ONESTOPBIT;
	this->m_parity=NOPARITY;
	this->m_hCom = NULL;
	this->m_bSending = FALSE;
	this->m_bIsRuning = FALSE;
	this->m_pSendThread = NULL;
	this->m_pRecvThread = NULL;

	::InitializeCriticalSection(&this->m_cComCS);
}

CSerialCom::~CSerialCom()
{
	this->m_bIsRuning = FALSE;

	//�ͷŽ��պͷ����߳�
	if ( this->m_pSendThread!=NULL )
	{
		::WaitForSingleObject(m_pSendThread->m_hThread,INFINITE);
		this->m_pSendThread = NULL;
	}

	if ( this->m_pRecvThread!=NULL )
	{
		::WaitForSingleObject(this->m_pRecvThread->m_hThread,INFINITE);
		this->m_pRecvThread = NULL;
	}

	//��������б��������
	::EnterCriticalSection(&m_cComCS);
	POSITION pos = m_cmdlist.GetHeadPosition();
	while ( pos != NULL )
	{
		this->m_pTempCmd = (CMDBUF *)m_cmdlist.GetNext(pos);
		delete []this->m_pTempCmd->pCmdBuf;
		delete this->m_pTempCmd;
	}
	this->m_cmdlist.RemoveAll();
	::LeaveCriticalSection(&this->m_cComCS);
	Sleep(50);
}

void CSerialCom::Send(const void *pBuffer, const int iLength)
{
	if (!m_bIsRuning)
	{
		return;
	}

	EnterCriticalSection(&m_cComCS);
	m_pTempCmd = new CMDBUF;
	m_pTempCmd->pCmdBuf = new UCHAR[iLength];
	if (m_pTempCmd->pCmdBuf==NULL)
	{
		return;
	}
	memcpy(m_pTempCmd->pCmdBuf,pBuffer,iLength);
	m_pTempCmd->nLen = iLength;

	m_cmdlist.AddTail(m_pTempCmd);
	LeaveCriticalSection(&m_cComCS);
}

void CSerialCom::ComSend(const void *pBuffer, const int iLength)
{
	if (m_hCom==NULL)
		return;

	m_bSending = TRUE;

	OVERLAPPED m_os={0};
	DWORD dwByteWrite;

	if (!WriteFile(m_hCom,pBuffer,iLength,&dwByteWrite,&m_os))
	{

	}
	Sleep(5);						//��ʱ10ms

	m_bSending = FALSE;

	// 	AfterSend(pBuffer,iLength);

	DWORD dwNumberOfBytesTransferred;

	GetOverlappedResult(m_hCom,&m_os,&dwNumberOfBytesTransferred,TRUE);
}

void CSerialCom::Close()
{
	this->m_bIsRuning = FALSE;

	//�ͷŽ��պͷ����߳�
	if ( this->m_pSendThread != NULL )
	{
		::WaitForSingleObject(m_pSendThread->m_hThread, 0);
		this->m_pSendThread = NULL;
	}

	if (m_pRecvThread!=NULL)
	{
		::WaitForSingleObject(m_pRecvThread->m_hThread, 0);
		m_pRecvThread = NULL;
	}

	if ((m_hCom!=NULL)&&(m_hCom!=INVALID_HANDLE_VALUE))
	{
		CloseHandle(m_hCom);
	}

	m_hCom=NULL;

	//��������б��������
	EnterCriticalSection(&m_cComCS);
	POSITION pos = m_cmdlist.GetHeadPosition();
	while (pos != NULL)
	{
		m_pTempCmd = (CMDBUF *)m_cmdlist.GetNext(pos);
		delete []m_pTempCmd->pCmdBuf;
		delete m_pTempCmd;
	}
	m_cmdlist.RemoveAll();
	LeaveCriticalSection(&m_cComCS);
	Sleep(50);
}

void CSerialCom::SetComProp(DCB* inProp)
{
	if (inProp == NULL)
		return;

	m_iBaudrate=inProp->BaudRate;
	m_bytesize=inProp->ByteSize;
	m_stopbits=inProp->StopBits;
	m_parity=inProp->Parity;
}


BOOL CSerialCom::Create(int iCom, int iBaudrate)
{
	this->m_iBaudrate = iBaudrate;
	this->m_iCom = iCom;

	if ( this->m_bIsRuning )
	{
		if ( iCom == m_iCom )
		{	//�ظ���ͬһ����
			return FALSE;
		} 
		else
		{	//δ�ر�ԭ���Ĵ���
			CloseHandle(m_hCom);
		}		
	}

	//��������
	CString csCom;
	if ( iCom < 10 )
		csCom.Format(_T("COM%d"), iCom);
	else
		csCom.Format(_T("\\\\.\\COM%d"), iCom);

	//�ô��ں��ַ����򿪴��ڲ����ش򿪵Ĵ��ھ��
	this->m_hCom = CreateFile(csCom,
																GENERIC_READ | GENERIC_WRITE,
																0,
																NULL,
																OPEN_EXISTING,
																FILE_FLAG_OVERLAPPED,
																NULL);
	if ( this->m_hCom == INVALID_HANDLE_VALUE )		//�򿪴���ʧ�ܵĴ���
	{
		this->m_hCom=NULL;
		return FALSE;
	}

	DCB stdcb;					//�豸���ö��������ǶԴ��ڵ�һ������

	COMMTIMEOUTS stCommTimeouts;
	stCommTimeouts.ReadIntervalTimeout = 1000;
	stCommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	stCommTimeouts.ReadTotalTimeoutConstant = 1000;
	stCommTimeouts.WriteTotalTimeoutMultiplier = 1000;
	stCommTimeouts.WriteTotalTimeoutConstant = 1000;
	if ( !SetCommTimeouts(this->m_hCom, &stCommTimeouts) )
	{
		CloseHandle(m_hCom);
		this->m_hCom=NULL;
		return FALSE;
	}

	GetCommState(this->m_hCom, &stdcb);			//�����ö������õ��򿪵Ĵ���
	stdcb.BaudRate = this->m_iBaudrate;				//���ô��ڲ�����
	stdcb.ByteSize = this->m_bytesize;						//���ô�����ֽ���
	stdcb.StopBits = this->m_stopbits;			//����ֹͣλ
	stdcb.Parity = this->m_parity;				//����У�鷽ʽ

	//�����ö������õ��򿪵Ĵ��ڣ������ʧ�ܣ��������رմ���
	if ( !SetCommState(m_hCom,&stdcb) )		
	{
		CloseHandle(m_hCom);
		this->m_hCom=NULL;
		return FALSE;
	}
	this->m_bIsRuning = TRUE;

	//�������ڽ����߳�
	this->m_pRecvThread = AfxBeginThread(ComMonitor,(LPVOID)this,THREAD_PRIORITY_NORMAL);
	//�������ڷ����߳�
	this->m_pSendThread = AfxBeginThread(SendingThread,(LPVOID)this,THREAD_PRIORITY_NORMAL);

	return TRUE;
}


//�жϴ����Ƿ��������
BOOL	CSerialCom::IsRunning()
{
	return m_bIsRuning;
}


/*!	@func	CSerialCom::GetSerialPortParam
|*	@brief	��ȡ�������Ӳ���
|*	@param	comNo	���ں�
|*	@param	baud	������
|*	@return null
\**/
void CSerialCom::GetSerialPortParam(int & comNo, int & baud)
{
	comNo = m_iCom;
	baud  = this->m_iBaudrate;
}

void CSerialCom::SetCmdStation(CBaseCmdStation * pCmdStation)
{
	m_pParser = pCmdStation;
}

void CSerialCom::ParseByte(BYTE* inChar)
{
	if (m_pParser!=NULL)
	{
		m_pParser->RecvData(*inChar);
	}
}

void CSerialCom::InitCom(int iComNo[])
{
	//�ô��ں��ַ����򿪴��ڲ����ش򿪵Ĵ��ھ��
	CString csCom;
	int iCom = 0;
	for ( int i = 1; i < 20; i++ )
	{
		csCom.Format(_T("COM%d"), i);
		this->m_hCom = CreateFile(csCom,
			0,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED,
			NULL);

		if ( this->m_hCom != INVALID_HANDLE_VALUE )
		{
			CloseHandle(m_hCom);
			m_hCom = NULL;
			iComNo[iCom] = i;
			iCom++;
		}
	}

	for ( int i = 10; i < 20; i++ )
	{
		csCom.Format(_T("\\\\.\\COM%d"), i);
		this->m_hCom = CreateFile(csCom,
			0,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED,
			NULL);

		if ( this->m_hCom != INVALID_HANDLE_VALUE )
		{
			CloseHandle(m_hCom);
			m_hCom = NULL;
			iComNo[iCom] = i;
			iCom++;
		}
	}

	m_hCom=NULL;
}

