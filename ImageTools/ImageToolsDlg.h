
// ImageToolsDlg.h: 头文件
//

#pragma once
class CImageView;

// CImageToolsDlg 对话框
class CImageToolsDlg : public CDialogEx
{
// 构造
public:
	CImageToolsDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGETOOLS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDropFiles(HDROP hDropInfo);

private: // func
    void SetWindowSizeParam(long width, long height, float ratio);

private:
	// window
    CImageView* m_imageView = nullptr;
    long m_width = 0;
    long m_height = 0;
    float m_ratio = 1.0f;

	// data
	std::vector<std::filesystem::path> m_fileList;
	std::vector<ImageData*> m_imageDataList; // leasted opened seven image
	ImageData* m_curImage = nullptr;
	IMAGE_FORMAT m_format = IMAGE_FORMAT::YUV444;
	void TestFunc();
};
