
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
}

BEGIN_MESSAGE_MAP(CBichonFilterAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOAD_BUTTON, &CBichonFilterAppDlg::OnBnClickedLoadButton)
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

	// TODO: Add extra initialization here

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

		m_matImage = imread(strPath, IMREAD_UNCHANGED);
		CreateBitmapInfo(m_matImage.cols, m_matImage.rows, m_matImage.channels() * 8);
		DrawImage();
	}
}

void CBichonFilterAppDlg::CreateBitmapInfo(int width, int height, int bpp)
{
	if (m_bitmapInfo != NULL)
	{
		delete m_bitmapInfo;
		m_bitmapInfo = NULL;
	}

	if (bpp == 8)
		m_bitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else
		m_bitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];

	m_bitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bitmapInfo->bmiHeader.biPlanes = 1;
	m_bitmapInfo->bmiHeader.biBitCount = bpp;
	m_bitmapInfo->bmiHeader.biCompression = BI_RGB;
	m_bitmapInfo->bmiHeader.biSizeImage = 0;
	m_bitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	m_bitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	m_bitmapInfo->bmiHeader.biClrUsed = 0;
	m_bitmapInfo->bmiHeader.biClrImportant = 0;

	if (bpp == 8)
	{
		for (int i = 0; i < 256; ++i)
		{
			m_bitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
			m_bitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
			m_bitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
			m_bitmapInfo->bmiColors[i].rgbReserved = 0;
		}
	}

	m_bitmapInfo->bmiHeader.biWidth = width;
	m_bitmapInfo->bmiHeader.biHeight = -height;
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
		m_matImage.cols,
		m_matImage.rows,
		m_matImage.data,
		m_bitmapInfo,
		DIB_RGB_COLORS,
		SRCCOPY);
}