#pragma once

class __declspec(dllexport) CCmdStation : public CBaseCmdStation
{
public:
	CCmdStation(void);
	~CCmdStation(void);

	virtual void RecvData(unsigned char inData);

	virtual void ResetRcvBuf();

	virtual void SetRecvBuf();

	virtual void SetRecvLine();

protected:
	UCHAR*		m_pRecvBuf;				//�������ݻ�����

	unsigned int m_nRcvIndex;		//�ѽ����ֽڼ���

	UINT		m_nFrameLength;	//��ǰָ���

	BOOL		m_bFrameStart;
};