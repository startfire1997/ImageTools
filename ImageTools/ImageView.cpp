// CImageView.cpp: 实现文件
//

#include "pch.h"
#include "ImageTools.h"
#include "ImageView.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "OpenGL32.lib")

// CImageView

IMPLEMENT_DYNCREATE(CImageView, CView)

CImageView::CImageView()
{

}

CImageView::~CImageView()
{
}

BEGIN_MESSAGE_MAP(CImageView, CView)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CImageView 绘图

void CImageView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
    HDC dc = ::GetDC(m_hWnd);

    glClear(GL_COLOR_BUFFER_BIT);

    if (m_imageLoaded) {
        glBindTexture(GL_TEXTURE_2D, m_imageTextureId);
        glBegin(GL_QUADS);
        glTexCoord2f(0.f, 0.f);
        glVertex3i(0, 0, 0);

        glTexCoord2f(0.f, 1.f);
        glVertex3i(0, m_imageHeight, 0);

        glTexCoord2f(1.f, 1.f);
        glVertex3i(m_imageWidth, m_imageHeight, 0);

        glTexCoord2f(1.f, 0.f);
        glVertex3i(m_imageWidth, 0, 0);
        glEnd();
    }
    SwapBuffers(dc);
    ::ReleaseDC(m_hWnd, dc);
}

// CImageView 诊断

#ifdef _DEBUG
void CImageView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CImageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CImageView 消息处理程序

// 设置格式？
BOOL CImageView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: 在此添加专用代码和/或调用基类
    cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CS_OWNDC;

    return CView::PreCreateWindow(cs);
}

int CImageView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  在此添加您专用的创建代码
    m_hdc = ::GetDC(m_hWnd);//hdc设备上下文,m_hWnd窗口句柄

    //设置像素格式
    PIXELFORMATDESCRIPTOR pfd =     //像素格式
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | //绘制到窗口
        PFD_SUPPORT_OPENGL |//支持opengl
        PFD_DOUBLEBUFFER,//采用双缓冲
        PFD_TYPE_RGBA,//像素类型 RGBA
        32, //像素位数 4*8- 32
        0, 0, 0, 0, 0, 0,//
        0,
        0,
        0,
        0, 0, 0, 0,
        0,// 深度缓冲区位数, 显示二维图片，不需要深度缓冲区
        0,//模板缓冲
        0,
        PFD_MAIN_PLANE,//
        0,
        0, 0, 0
    };

    int pixelformat = ChoosePixelFormat(m_hdc, &pfd); //匹配像素格式的索引
    SetPixelFormat(m_hdc, pixelformat, &pfd); //设置像素格式

    m_hglrc = wglCreateContext(m_hdc);//hglrc ：opengl设备上下文
    wglMakeCurrent(m_hdc, m_hglrc);//hglrc绑定hdc; 绘制到当前设备上下文

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenTextures(1, &m_imageTextureId);
    glBindTexture(GL_TEXTURE_2D, m_imageTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glClearColor(0, 0, 0, 0);
    return 0;
}


void CImageView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    glViewport(0, 0, cx, cy);

    glMatrixMode(GL_PROJECTION);	// set projection matrix current matrix
    glLoadIdentity();
    glOrtho(0, cx, cy, 0, 0, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0, 0, -10.0f);
    glScalef(m_ratio, m_ratio, 1.f);
}


void CImageView::OnDestroy()
{
    CView::OnDestroy();

    // TODO: 在此处添加消息处理程序代码
    wglDeleteContext(m_hglrc);
}

BOOL CImageView::OnEraseBkgnd(CDC* pDC)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    return TRUE;
    return CView::OnEraseBkgnd(pDC);
}

BOOL CImageView::SetImageParam(long width, long height, float ratio)
{
    m_imageWidth = width;
    m_imageHeight = height;
    m_ratio = ratio;
    m_imageLoaded = false;
    return 0;
}

BOOL CImageView::SetImageData(PixelData* rgbaBuf)
{
    glBindTexture(GL_TEXTURE_2D, m_imageTextureId);
    if (m_imageLoaded)
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_imageWidth, m_imageHeight, GL_RGBA, GL_UNSIGNED_BYTE, rgbaBuf);
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_imageWidth, m_imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaBuf);
        m_imageLoaded = true;
    }
    Invalidate(NULL);
    return 0;
}
