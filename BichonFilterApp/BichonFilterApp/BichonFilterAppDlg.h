
// BichonFilterAppDlg.h : header file
//

#pragma once

/* opencv */
#include "opencv2//opencv.hpp"

using namespace cv;

// CBichonFilterAppDlg dialog
class CBichonFilterAppDlg : public CDialogEx
{
// Construction
public:
	CBichonFilterAppDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BICHONFILTERAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoadButton();

public:
	Mat m_matImage;
	BITMAPINFO* m_bitmapInfo;

	void CreateBitmapInfo(int width, int height, int bpp);
	void DrawImage();
};
