
// ImageToolsDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ImageTools.h"
#include "ImageToolsDlg.h"
#include "afxdialogex.h"
#include "ImageView.h"
#include <io.h>
#include <fcntl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CImageToolsDlg 对话框



CImageToolsDlg::CImageToolsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGETOOLS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImageToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CImageToolsDlg 消息处理程序

BOOL CImageToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CRect windRect;
	GetClientRect(windRect);
    m_imageView = new CImageView();
    m_imageView->Create(NULL, NULL, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, windRect, this, 0);

	TestFunc();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CImageToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CImageToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CImageToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CImageToolsDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnDropFiles(hDropInfo);
}

void CImageToolsDlg::SetWindowSizeParam(long width, long height, float ratio)
{
	if (m_width == width && m_height == height && m_ratio == ratio) {
		return;
	}
    m_ratio = ratio;
	m_width = width;
	m_height = height;

    m_imageView->SetImageParam(m_width, m_height, m_ratio);
    m_imageView->SetWindowPos(NULL, 0, 0, m_width * m_ratio, m_height * m_ratio, SWP_NOMOVE);
    SetWindowPos(NULL, 0, 0, m_width * m_ratio, m_height * m_ratio, SWP_NOMOVE);
}

void CImageToolsDlg::TestFunc()
{
	ImageData newData;
	m_format = IMAGE_FORMAT::YUV420NV21;
	if (m_format == IMAGE_FORMAT::YUV420) {
        newData.m_filePath = "images\\test-yuv420p.yuv";
	}
	else if (m_format == IMAGE_FORMAT::YUV420NV12) {
        newData.m_filePath = "images\\test-nv12.yuv";
    }
    else if (m_format == IMAGE_FORMAT::YUV420NV21) {
        newData.m_filePath = "images\\test-nv21.yuv";
    }
	else if (m_format == IMAGE_FORMAT::YUV444) {
		newData.m_filePath = "images\\test-yuv444p.yuv";
	}
	PixelData* yBuf = nullptr;
	PixelData* uBuf = nullptr;
	PixelData* vBuf = nullptr;
    PixelData* uvBuf = nullptr;

	// regex parse width and height
	{
		newData.m_width = 2560;
		newData.m_height = 1440;
		yBuf = new PixelData[newData.m_width * newData.m_height * 2];
		uBuf = new PixelData[newData.m_width * newData.m_height * 2];
		vBuf = new PixelData[newData.m_width * newData.m_height * 2];
        uvBuf = new PixelData[newData.m_width * newData.m_height * 4];
    }

	// allcate mem of rgba buffer
	{
		newData.m_bufData = new PixelData[newData.m_width * newData.m_height * 4];
	}

	// read yuv data to buffer
	if (m_format == IMAGE_FORMAT::YUV420 ||
		m_format == IMAGE_FORMAT::YUV420NV12 ||
		m_format == IMAGE_FORMAT::YUV420NV21){
		// yuv420; (Y: wxh)(U: w/2 x h/2)(V: w/2 x h/2)
        // yuv420nv12; (Y: wxh)(U0V0U1V1: w/2 x h/2)
        // yuv420nv21; (Y: wxh)(V0U0V1U1: w/2 x h/2)
        int ySize = newData.m_width * newData.m_height;
		int uvSize = ((newData.m_width + 1) >> 1) * ((newData.m_height + 1) >> 1);

		int ret = _wsopen_s(&newData.m_fd, newData.m_filePath.wstring().c_str(),
			(O_RDONLY | O_BINARY), _SH_DENYNO, 0);
		ASSERT(ret != -1);

		if (m_format == IMAGE_FORMAT::YUV420NV12 ||
			m_format == IMAGE_FORMAT::YUV420NV21) {
            ret = _read(newData.m_fd, yBuf, ySize);
            ASSERT(ret != -1);
            ret = _read(newData.m_fd, uvBuf, ySize / 2);
            ASSERT(ret != -1);
        }
		else {
			ret = _read(newData.m_fd, yBuf, ySize);
			ASSERT(ret != -1);
			ret = _read(newData.m_fd, uBuf, uvSize);
			ASSERT(ret != -1);
			ret = _read(newData.m_fd, vBuf, uvSize);
			ASSERT(ret != -1);
		}
	}
	else if (m_format == IMAGE_FORMAT::YUV444) {

	}

#define clip(var) ((var >= 255) ? 255 : (var <= 0) ? 0 : var)
	// convert yuv420p to rgba
    if (m_format == IMAGE_FORMAT::YUV420 ||
        m_format == IMAGE_FORMAT::YUV420NV12 ||
        m_format == IMAGE_FORMAT::YUV420NV21) {
		int width = newData.m_width;
		int height = newData.m_height;
		int rIdx, cIdx;
		int yVal, uVal, vVal;
		int stride_uv = (newData.m_width + 1) >> 1;
		unsigned char* line = newData.m_bufData;
		unsigned char* cur;

		for (cIdx = 0; cIdx < height; cIdx++) {
			cur = line;
			for (rIdx = 0; rIdx < width; rIdx++) {
				yVal = yBuf[cIdx * width + rIdx]; // -16;

                // 提前减了128，所以后面不用再减去128了
                if (m_format == IMAGE_FORMAT::YUV420NV12) {
                    uVal = uvBuf[(cIdx >> 1) * width + (rIdx >> 1 << 1)] - 128;
                    vVal = uvBuf[(cIdx >> 1) * width + (rIdx >> 1 << 1) + 1] - 128;
				}
				else if (m_format == IMAGE_FORMAT::YUV420NV21) {
                    uVal = uvBuf[(cIdx >> 1) * width + (rIdx >> 1 << 1) + 1] - 128;
                    vVal = uvBuf[(cIdx >> 1) * width + (rIdx >> 1 << 1)] - 128;
				}
				else {
                    uVal = uBuf[(cIdx >> 1) * stride_uv + (rIdx >> 1)] - 128;
                    vVal = vBuf[(cIdx >> 1) * stride_uv + (rIdx >> 1)] - 128;
				}

				// 转换公式：https://www.jianshu.com/p/259ae6f1e5e0
				(*cur) = clip(yVal + ((360 * (vVal)) >> 8));
				cur++;
				(*cur) = clip(yVal - (((88 * (uVal) + 184 * (vVal))) >> 8));
				cur++;
				(*cur) = clip(yVal + ((455 * (uVal)) >> 8));
				cur += 2;
			}
			line += (width << 2);
		}
	}

	{
		SetWindowSizeParam(newData.m_width, newData.m_height, 0.5);
        m_imageView->SetImageData(newData.m_bufData);
    }
	//
	{
		_close(newData.m_fd);
	}
}
