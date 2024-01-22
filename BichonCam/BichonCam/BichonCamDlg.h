
// BichonCamDlg.h : header file
//

#pragma once

#include "opencv2/opencv.hpp"

#define CAMERA_EVENT_FLAG 1

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
	enum BUTTON_STATE {
		NONE,
		FACE_DETECT,
		EYE_SENSOR,
	};

	void CreateBitmapInfo(BITMAPINFO** bmInfo, int width, int height, int bpp);
	void DrawImage();
	void DetectFace();
	CStatic m_cameraControl;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void MLDetectFace(bool eyeSensor);
	afx_msg void OnBnClickedDetectFaceButton();
	afx_msg void OnBnClickedEyeSensorButton();
	afx_msg void OnBnClickedClearButton();

	BUTTON_STATE buttonState = NONE;
};
