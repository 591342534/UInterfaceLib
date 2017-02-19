#pragma once

/************************************************* 
Copyright:forour
Author: 
Date:2015-06-28
Description:�Ի�ؼ���
**************************************************/  

/************************************************* 
Function:				COwnerButton
Description:		�Ի水ť��
Input:					
Return:					
Others:					
*************************************************/  
class COwnerButton : public CButton
{
	DECLARE_DYNAMIC(COwnerButton)

public:
	COwnerButton(void);
	~COwnerButton(void);

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();

public:
	// �Ի水ť
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	// ��Ӧ����ƶ�ʱ��Ϣ

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	// �Զ�������Ƴ���ťʱ��Ϣ
	afx_msg LRESULT OnMouseOut(WPARAM wParam, LPARAM lParam);

	// ����λͼ
	void DrawBitmap(HDC hDC, HBITMAP hBitmap, int w, int h);

	// ��ʼ���Ի水ť
	BOOL InitButton(HINSTANCE hInstance, int nUpID, int nDownID, int nOverID);

	// �ͷ��Ի水ť
	void ReleaseButton();

	CRect GetTextRect(int iLen);

protected:
	// �Ի水ť������״̬��Ӧ��λͼ���
	HBITMAP m_hUpBitmap, m_hDownBitmap, m_hOverBitmap; // Up, Down, Over
	BOOL m_bOver;
};

/************************************************* 
Function:				COwnerPicSta
Description:		�Ի澲̬����
Input:					
Return:					
Others:					
*************************************************/  
class COwnerPicSta : public CStatic
{
public:
	COwnerPicSta(void);
	~COwnerPicSta(void);
	void LoadImage(cv::Mat img, int iMethod = 1);
	void  LoadImage(CString csPath, int iMethod = 1);
	cv::Mat GetImage();

protected:
	cv::Mat m_Image;
	CRect m_Rect;
	CString m_csPath;
};