#pragma once
/************************************************* 
Copyright:forour
Author: 
Date:2015-06-28
Description:�Ի�ؼ���
**************************************************/  

#pragma once

class COwnerButton :
	public CButton
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

	BOOL m_bOver;

	// ����λͼ
	void DrawBitmap(HDC hDC, HBITMAP hBitmap, int w, int h);

	// ��ʼ���Ի水ť
	BOOL InitButton(HINSTANCE hInstance, int nUpID, int nDownID, int nOverID);

	// �ͷ��Ի水ť
	void ReleaseButton();

	// �Ի水ť������״̬��Ӧ��λͼ���
	HBITMAP m_hUpBitmap, m_hDownBitmap, m_hOverBitmap; // Up, Down, Over

	CRect GetTextRect(int iLen);
};