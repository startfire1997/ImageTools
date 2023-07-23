#pragma once

// CImageView 视图

class CImageView : public CView
{
	DECLARE_DYNCREATE(CImageView)

public:
	CImageView();           // 动态创建所使用的受保护的构造函数
	virtual ~CImageView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
    BOOL SetImageParam(long width, long height, float ratio);
    BOOL SetImageData(PixelData* rgbaBuf);

private:
    HDC m_hdc;
    HGLRC m_hglrc;
    uint32_t m_imageTextureId;
    float m_ratio = 1.0f;
    bool m_imageLoaded = false;
    long m_imageWidth = 0;
    long m_imageHeight = 0;
};


