
// BichonFilterAppDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "BichonFilterApp.h"
#include "BichonFilterAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

enum FILTERTYPE
{
	NONE,
	EMBOSSING,
	BLURRING,
	GAUSSIAN_BLUR,
	UNSHARP_MASK,
	NOISE_GAUSSIAN,
	BILATERAL,
	MEDIAN
};

Point g_prevPoint;
Point g_currPoint;

Mat g_colorImage;
Mat g_grayImage;

Mat g_outputImage;

BITMAPINFO* g_bitmapInfo;
BITMAPINFO* g_grayBitmapInfo;

FILTERTYPE g_filterType = NONE;

int sigmaValue = 1;
int g_stddevValue = 30;
int g_ksizeValue = 3;

void MouseEvent(int event, int x, int y, int flags, void*)
{
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		g_prevPoint = Point(x, y);
		break;
	case EVENT_LBUTTONUP:
		g_currPoint = Point(x, y);
		Mat newImg = g_colorImage(Rect(g_prevPoint.x, g_prevPoint.y, g_currPoint.x - g_prevPoint.x, g_currPoint.y - g_prevPoint.y));
		if (g_prevPoint.x == g_currPoint.x && g_prevPoint.y == g_currPoint.y) { break; }
		g_outputImage = newImg;
		break;
	}
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
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

// CBichonFilterAppDlg dialog

CBichonFilterAppDlg::CBichonFilterAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BICHONFILTERAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBichonFilterAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SIGMA_SLIDER, m_sigmaSlider);
	DDX_Control(pDX, IDC_SIGMA_TEXT, m_sigmaText);
	DDX_Control(pDX, IDC_KSIZE_SLIDER, m_ksizeSlider);
	DDX_Control(pDX, IDC_KSIZE_TEXT, m_ksizeText);
	DDX_Control(pDX, IDC_STDDEV_TEXT, m_stddevText);
	DDX_Control(pDX, IDC_STDDEV_SLIDER, m_stddevSlider);
}

BEGIN_MESSAGE_MAP(CBichonFilterAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOAD_BUTTON, &CBichonFilterAppDlg::OnBnClickedLoadButton)
	ON_BN_CLICKED(IDC_LOAD_OUTPUT, &CBichonFilterAppDlg::OnBnClickedLoadOutput)
	ON_BN_CLICKED(IDC_CLEAR_OUTPUT, &CBichonFilterAppDlg::OnBnClickedClearOutput)
	ON_COMMAND(ID_FILTER_EMBOSSING, &CBichonFilterAppDlg::OnFilterEmbossing)
	ON_COMMAND(ID_FILTER_BLURRING, &CBichonFilterAppDlg::OnFilterBlurring)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_STDDEV_SLIDER, &CBichonFilterAppDlg::OnNMCustomdrawStddevSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_KSIZE_SLIDER, &CBichonFilterAppDlg::OnNMCustomdrawKsizeSlider)
	ON_COMMAND(ID_FILTER_GAUSSIANBLUR, &CBichonFilterAppDlg::OnFilterGaussianblur)
	ON_COMMAND(ID_FILTER_UNSHARPMASK, &CBichonFilterAppDlg::OnFilterUnsharpmask)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SIGMA_SLIDER, &CBichonFilterAppDlg::OnNMCustomdrawSigmaSlider)
	ON_COMMAND(ID_FILTER_NOISEGAUSSIAN, &CBichonFilterAppDlg::OnFilterNoisegaussian)
	ON_COMMAND(ID_FILTER_BILATERAL, &CBichonFilterAppDlg::OnFilterBilateral)
	ON_COMMAND(ID_FILTER_MEDIAN, &CBichonFilterAppDlg::OnFilterMedian)
END_MESSAGE_MAP()


// CBichonFilterAppDlg message handlers

BOOL CBichonFilterAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_sigmaSlider.SetRange(1, 5);
	m_ksizeSlider.SetRange(1, 3);
	m_stddevSlider.SetRange(1, 3);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBichonFilterAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBichonFilterAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBichonFilterAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBichonFilterAppDlg::OnBnClickedLoadButton()
{
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_READONLY, _T("image file(*.jpg;*.bmp;*.png;)|*.jpg;*.bmp;*.png;|All Files(*.*)|*.*||"));

	if (fileDlg.DoModal() == IDOK)
	{
		CString path = fileDlg.GetPathName();
		CT2CA pszString(path);
		std::string strPath(pszString);

		g_colorImage = imread(strPath, IMREAD_UNCHANGED);
		g_grayImage = imread(strPath, IMREAD_GRAYSCALE);

		CreateBitmapInfo(&g_bitmapInfo, g_colorImage.cols, g_colorImage.rows, g_colorImage.channels() * 8);
		CreateBitmapInfo(&g_grayBitmapInfo, g_grayImage.cols, g_grayImage.rows, g_grayImage.channels() * 8);

		DrawImage();
	}
}

void CBichonFilterAppDlg::CreateBitmapInfo(BITMAPINFO** bmInfo, int width, int height, int bpp)
{
	if (*bmInfo != NULL)
	{
		delete[] *bmInfo;
		*bmInfo = NULL;
	}
	
	if (bpp == 8)
		(*bmInfo) = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else
		(*bmInfo) = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];

	(*bmInfo)->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	(*bmInfo)->bmiHeader.biBitCount = bpp;
	(*bmInfo)->bmiHeader.biPlanes = 1;
	(*bmInfo)->bmiHeader.biCompression = BI_RGB;
	(*bmInfo)->bmiHeader.biSizeImage = 0;
	(*bmInfo)->bmiHeader.biXPelsPerMeter = 0;
	(*bmInfo)->bmiHeader.biYPelsPerMeter = 0;
	(*bmInfo)->bmiHeader.biClrUsed = 0;
	(*bmInfo)->bmiHeader.biClrImportant = 0;

	if (bpp == 8)
	{
		for (int i = 0; i < 256; ++i)
		{
			(*bmInfo)->bmiColors[i].rgbBlue = (BYTE)i;
			(*bmInfo)->bmiColors[i].rgbGreen = (BYTE)i;
			(*bmInfo)->bmiColors[i].rgbRed = (BYTE)i;
			(*bmInfo)->bmiColors[i].rgbReserved = 0;
		}
	}

	(*bmInfo)->bmiHeader.biWidth = width;
	(*bmInfo)->bmiHeader.biHeight = -height;
}

void CBichonFilterAppDlg::DrawImage()
{
	CClientDC dc(GetDlgItem(IDC_INP_PIC));
	CRect rect;
	GetDlgItem(IDC_INP_PIC)->GetClientRect(&rect);

	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(
		dc.GetSafeHdc(),
		0,
		0,
		rect.Width(),
		rect.Height(),
		0,
		0,
		g_colorImage.cols,
		g_colorImage.rows,
		g_colorImage.data,
		g_bitmapInfo,
		DIB_RGB_COLORS,
		SRCCOPY);

}

void CBichonFilterAppDlg::DrawOutputImage(BITMAPINFO* bminfo)
{
	CClientDC dc(GetDlgItem(IDC_OUTPUT_PIC));
	CRect rect;
	GetDlgItem(IDC_OUTPUT_PIC)->GetClientRect(&rect);
	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(
		dc.GetSafeHdc(),
		0,
		0,
		rect.Width(),
		rect.Height(),
		0,
		0,
		g_outputImage.cols,
		g_outputImage.rows,
		g_outputImage.data,
		bminfo,
		DIB_RGB_COLORS,
		SRCCOPY);
}


void CBichonFilterAppDlg::OnBnClickedLoadOutput()
{
}

void CBichonFilterAppDlg::OnBnClickedClearOutput()
{
	// TODO: Add your control notification handler code here
}

void CBichonFilterAppDlg::OnFilterEmbossing()
{
	g_filterType = EMBOSSING;

	float data[] = { -1, -1, 0, -1, 0, 1, 0, 1, 1 };
	Mat emboss(g_ksizeValue, g_ksizeValue, CV_32FC1, data);
	filter2D(g_colorImage, g_outputImage, -1, emboss, Point(-1, -1), 128);
	DrawOutputImage(g_bitmapInfo);
}

void CBichonFilterAppDlg::OnFilterBlurring()
{
	g_filterType = BLURRING;
	blur(g_colorImage, g_outputImage, Size(g_ksizeValue, g_ksizeValue));
	DrawOutputImage(g_bitmapInfo);
}

void CBichonFilterAppDlg::OnFilterGaussianblur()
{
	g_filterType = GAUSSIAN_BLUR;

	GaussianBlur(g_colorImage, g_outputImage, Size(), (double)sigmaValue);
	DrawOutputImage(g_bitmapInfo);
}

void CBichonFilterAppDlg::OnFilterUnsharpmask()
{
	g_filterType = UNSHARP_MASK;

	GaussianBlur(g_colorImage, g_outputImage, Size(), (double)sigmaValue);

	float alpha = 1.0f;
	g_outputImage = (1 + alpha) * g_colorImage - alpha * g_outputImage;
	DrawOutputImage(g_bitmapInfo);
}

void CBichonFilterAppDlg::OnNMCustomdrawSigmaSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	sigmaValue = m_sigmaSlider.GetPos();
	std::wstring windowText = L"Sigma value : " + std::to_wstring(sigmaValue);
	m_sigmaText.SetWindowTextW(windowText.c_str());

	switch (g_filterType)
	{
	case UNSHARP_MASK:
		OnFilterUnsharpmask();
		break;
	case GAUSSIAN_BLUR:
		OnFilterGaussianblur();
		break;
	default:
		break;
	}

	*pResult = 0;
}

void CBichonFilterAppDlg::OnFilterNoisegaussian()
{
	g_filterType = NOISE_GAUSSIAN;

	Mat noise(g_grayImage.size(), CV_32SC1);
	randn(noise, 0, g_stddevValue);
	add(g_grayImage, noise, g_outputImage, Mat(), CV_8U);
	DrawOutputImage(g_grayBitmapInfo);
}

void CBichonFilterAppDlg::OnFilterBilateral()
{
	g_filterType = BILATERAL;

	bilateralFilter(g_colorImage, g_outputImage, -1, 10, 5);
	DrawOutputImage(g_bitmapInfo);
}


void CBichonFilterAppDlg::OnFilterMedian()
{
	g_filterType = MEDIAN;

	Mat tempImage = g_grayImage.clone();

	int num = (int)(tempImage.total() * 0.1);

	for (int i = 0; i < num; ++i) 
	{
		int x = rand() % tempImage.cols;
		int y = rand() % tempImage.rows;
		tempImage.at<uchar>(y, x) = (i % 2) * 255;
	}

	imshow("Temp Image", tempImage);

	medianBlur(tempImage, g_outputImage, g_ksizeValue);

	DrawOutputImage(g_grayBitmapInfo);
}

void CBichonFilterAppDlg::OnNMCustomdrawKsizeSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	g_ksizeValue = (m_ksizeSlider.GetPos() + 1) + m_ksizeSlider.GetPos();
	std::wstring windowText = L"KSize : " + std::to_wstring(g_ksizeValue);
	m_ksizeText.SetWindowTextW(windowText.c_str());

	switch (g_filterType)
	{
	case BLURRING:
		OnFilterBlurring();
		break;
	case MEDIAN:
		OnFilterMedian();
		break;
	case EMBOSSING:
		OnFilterEmbossing();
	default:
		break;
	}

	*pResult = 0;
}

void CBichonFilterAppDlg::OnNMCustomdrawStddevSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	g_stddevValue = m_stddevSlider.GetPos() * 10;
	std::wstring windowText = L"stddev : " + std::to_wstring(g_stddevValue);
	m_stddevText.SetWindowTextW(windowText.c_str());

	switch (g_filterType)
	{
	case NOISE_GAUSSIAN:
		OnFilterNoisegaussian();
		break;
	default:
		break;
	}

	*pResult = 0;
}
