#pragma once

typedef unsigned __int16    INT16U;

//����ָ��Ļ����б��Ա
struct CMDBUF 
{
	UCHAR * pCmdBuf;	//ָ������ָ��
	UINT nLen;			//ָ���
};

class CBaseCmdStation
{
public:
	CBaseCmdStation(){};
	virtual ~CBaseCmdStation(){};

public:
	virtual void RecvData(unsigned char inData) = 0;
	virtual void ResetRcvBuf() = 0;
	virtual void SetRecvBuf() = 0;
	virtual void SetRecvLine() = 0;
};

class __declspec(dllexport) CSerialCom
{
public:
	CSerialCom();
	virtual ~CSerialCom();	

public:
	BOOL IsRunning();

	BOOL Create(int iCom, int iBaudrate);
	
	void Close();

	void Send(const void *pBuffer, const int iLength);		//ͨ�������б����ָ��ͣ���׼�ӿڣ�

		/*!	@func	CSerialCom::GetSerialPortParam
	|*	@brief	��ȡ�������Ӳ���
	|*	@param	comNo	���ں�
	|*	@param	baud	������
	|*	@return null
	\**/
	void GetSerialPortParam(int & comNo, int & baud);

	void SetCmdStation(CBaseCmdStation * pCmdStation);

	void InitCom(int iComNo[]);

protected:
	void SendData(int nodeID, int memAddr, int data);

	/*!	@func	CSerialCom::ParseByte
	|*	@brief	���յ�����
	|*	@param	inChar	���յ���һ���ֽ�
	|*	@return null
	\**/
	virtual void ParseByte(BYTE* inChar);

protected:
	//��������
	int			m_iCom;												
	int			m_iBaudrate;
	HANDLE		m_hSingleEvent;
	BYTE		m_bytesize;
	BYTE		m_parity;
	BYTE		m_stopbits;
	CRITICAL_SECTION	m_cComCS;							//�ٽ�����������

protected:
	CPtrList		m_cmdlist;									//����ָ��Ļ����б�
	CMDBUF   *	m_pTempCmd;									//����ָ����Ա��ʱָ��
	HANDLE		m_hCom;										//�Ѵ򿪵Ĵ��ھ��
	BOOL			m_bIsRuning;									//����״̬���
	BOOL			m_bSending;									//���ͱ��

	CBaseCmdStation *	m_pParser;

private:
	static UINT ComMonitor(LPVOID pParam);
	static UINT SendingThread(LPVOID pParam);
	void SetComProp(DCB* inProp);
	void ComSend(const void *pBuffer, const int iLength);	//ֱ�ӷ���ָ��

protected:
	CWinThread	*	m_pSendThread;		//�����߳�
	CWinThread	*	m_pRecvThread;		//�����߳�
};
