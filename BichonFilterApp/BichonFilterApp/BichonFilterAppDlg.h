
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
	afx_msg void OnBnClickedLoadOutput();
	afx_msg void OnBnClickedClearOutput();
	afx_msg void OnFilterEmbossing();
	afx_msg void OnFilterBlurring();
	afx_msg void OnFilterGaussianblur();
	afx_msg void OnFilterUnsharpmask();

public:
	void CreateBitmapInfo(BITMAPINFO** bmInfo, int width, int height, int bpp);
	void DrawImage();
	void DrawOutputImage(BITMAPINFO* bminfo);

	CSliderCtrl m_sigmaSlider;
	afx_msg void OnNMCustomdrawSigmaSlider(NMHDR* pNMHDR, LRESULT* pResult);
	CStatic m_sigmaText;
	afx_msg void OnFilterNoisegaussian();
	afx_msg void OnFilterBilateral();
	afx_msg void OnFilterMedian();
	CSliderCtrl m_ksizeSlider;
	CStatic m_ksizeText;
	afx_msg void OnNMCustomdrawKsizeSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawStddevSlider(NMHDR* pNMHDR, LRESULT* pResult);
	CStatic m_stddevText;
	CSliderCtrl m_stddevSlider;
};
