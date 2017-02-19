#include "StdAfx.h"

COwnerPicSta::COwnerPicSta(void)
{
}


COwnerPicSta::~COwnerPicSta(void)
{
}

BEGIN_MESSAGE_MAP(COwnerPicSta, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

//iMethod���ŵķ�ʽ��0:ͼƬԭ����, 1:����ͼƬ
void COwnerPicSta::LoadImage(Mat img, int iMethod)
{
	if ( img.empty() )
	{
		return;
	}

	this->GetClientRect(&m_Rect);
	m_Image = Mat(m_Rect.Height(),m_Rect.Width(),CV_8UC3);

	if (m_Image.size != img.size)
	{
		ResizeImage(img, m_Rect, m_Image, iMethod);
	}
	else
	{
		m_Image = img;
	}

	//OnPaint();
	Invalidate(FALSE);
}

//iMethod���ŵķ�ʽ��0:ͼƬԭ����, 1:����ͼƬ

void  COwnerPicSta::LoadImage(CString csPath, int iMethod)
{
	if ( csPath.IsEmpty() )
	{
		return;
	}

	Mat Image;
#ifdef _UNICODE
	Image = imread((string)CCommTools::GetInstance()->string_W2A(csPath));
#else
	Image = imread((string)csPath);
#endif
	
	if ( Image.data == nullptr)
	{
		CString csTmp;
		csTmp.Format(_T("�����ļ�·��%s����"), csPath);
		MessageBox(csTmp, _T("����ʧ��"), MB_ICONERROR);

		return;
	}

	m_csPath = _T("");
	m_csPath = csPath;
	this->LoadImage(Image, iMethod);
}

//img�����ͼ��rectϣ�������ɵĴ�С������ΪMFC���CRect����dst_img�������ͼ��
//�ú����ȸ�ʽ��dst_img,�ٽ�img����dst_img,��˲���img��dst_img��������ͬһ��ͼ
//method���ŵķ�ʽ��0��ʾ����ͼƬԭ����ȥ��Ӧrect,1��ʾ����ͼƬȥ��Ӧrect
void COwnerPicSta::ResizeImage(Mat &img,CRect rect,Mat &dst_img,int method)
{
	// ��ȡͼƬ�Ŀ�͸�
	int h = img.rows;
	int w = img.cols;
	int nw,nh;

	if (0==method)
	{
		// ���㽫ͼƬ���ŵ�dst_drawing��������ı�������
		float scale1 = (float) ( (float)w /(float)rect.Width() );
		float scale2 = (float) ( (float)h /(float)rect.Height() );

		float scale=(scale1>scale2)?scale1:scale2;

		// ���ź�ͼƬ�Ŀ�͸�
		nw = ((float)w)/scale;
		nh = ((float)h)/scale;

		//���ڸ���Ҳ�������ݽضϣ�Ϊ��ֹnw����Ŀ����С�������߽籣��
		if (nw>rect.Width())
		{
			nw=rect.Width();
		}
		if (nh>rect.Height())
		{
			nh=rect.Height();
		}
	}else if (1==method)
	{
		// ���ź�ͼƬ�Ŀ�͸�
		nw = rect.Width();
		nh = rect.Height();
	}

	// Ϊ�˽����ź��ͼƬ���� dst_drawing �����в�λ�������ͼƬ�� dst_drawing ���Ͻǵ���������ֵ
	int tlx = ( nw < rect.Width())? (((float)(rect.Width()-nw))/2.0f+0.5f):   0;
	int tly = ( nh < rect.Height())? (((float)(rect.Height()-nh))/2.0f+0.5f): 0;

	//���þ����С��ͼ�Ƚϴ�ʱҲ�ܷ�ʱ�䣬�Ӹ��жϿɱ����ظ������þ����С
	if ( dst_img.rows!=rect.Height() || dst_img.cols!=rect.Width() || dst_img.type()!=img.type() )
	{
		dst_img=Mat(rect.Height(),rect.Width(),img.type());
	}
	if (0==method)
	{
		dst_img=0;
	}
	// ���� dst_img �� ROI ��������������ĺ��ͼƬ img
	Rect rect_roi=Rect(tlx,tly,nw,nh);

	// ��ͼƬ img �������ţ������뵽 src_drawing ��
	Mat dst_img_roi=dst_img(rect_roi);

	//��img�ߴ�������dst_img_roi�ߴ�,��ֵ����ѡ��INTER_NEAREST���ŵ������޽�����ʾ�ܽ�ÿ�����ؿ���
	resize(img,dst_img_roi,cv::Size(nw,nh),0.0,0.0,INTER_NEAREST);
}

void COwnerPicSta::OnPaint()
{
	//������Ӧ�����ػ���Ϣ,ɾ���� WM_PAINT��Ϣ�޷�����Ϣ����������������𲻶ϵĴ����ػ�
	CPaintDC dc(this);

	CDC* pDC=this->GetDC();	
	HDC hDC = pDC->GetSafeHdc();

	this->GetClientRect(&m_Rect);

	IplImage* pImage = &IplImage(m_Image);
	CvvImage Cvvimg;
	Cvvimg.CopyOf(pImage);
	Cvvimg.DrawToHDC(hDC, &m_Rect );

	ReleaseDC(pDC);		
}

void COwnerPicSta::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if ( !m_csPath.IsEmpty() )
	{
		Mat tmp = imread((string)CCommTools::GetInstance()->string_W2A(m_csPath));
		
		imshow(CCommTools::GetInstance()->string_W2A(m_csPath), tmp);
	}

	CStatic::OnLButtonDblClk(nFlags, point);
}



cv::Mat COwnerPicSta::GetImage()
{
	return m_Image;
}
