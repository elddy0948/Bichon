
// BichonCamDlg.h : header file
//

#pragma once

#include "opencv2/opencv.hpp"

using namespace cv;

// CBichonCamDlg dialog
class CBichonCamDlg : public CDialogEx
{
// Construction
public:
	CBichonCamDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BICHONCAM_DIALOG };
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
	void CreateBitmapInfo(BITMAPINFO** bmInfo, int width, int height, int bpp);
	void DrawImage();
	CStatic m_cameraControl;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
