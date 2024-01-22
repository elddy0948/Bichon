
// BichonCamDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "BichonCam.h"
#include "BichonCamDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

VideoCapture capture;
Mat g_image;
BITMAPINFO* g_bmInfo;
dnn::Net g_net;

const String model = "res10_300x300_ssd_iter_140000_fp16.caffemodel";
const String config = "deploy.prototxt";

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

// CBichonCamDlg dialog

CBichonCamDlg::CBichonCamDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BICHONCAM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBichonCamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAMERA_CONTROL, m_cameraControl);
}

BEGIN_MESSAGE_MAP(CBichonCamDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_DETECT_FACE_BUTTON, &CBichonCamDlg::OnBnClickedDetectFaceButton)
	ON_BN_CLICKED(IDC_EYE_SENSOR_BUTTON, &CBichonCamDlg::OnBnClickedEyeSensorButton)
	ON_BN_CLICKED(IDC_CLEAR_BUTTON, &CBichonCamDlg::OnBnClickedClearButton)
END_MESSAGE_MAP()


// CBichonCamDlg message handlers

BOOL CBichonCamDlg::OnInitDialog()
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
	capture = VideoCapture(1);

	g_net = dnn::readNet(model, config);

	if (g_net.empty())
	{
		std::cerr << "Net open failed!" << std::endl;
		return -1;
	}

	SetTimer(1, 10, NULL);

	if (!capture.isOpened())
	{
		MessageBox(_T("Can't open camera.\n"));
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBichonCamDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBichonCamDlg::OnPaint()
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
HCURSOR CBichonCamDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBichonCamDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case CAMERA_EVENT_FLAG:
		capture >> g_image;
		CreateBitmapInfo(&g_bmInfo, g_image.cols, g_image.rows, g_image.channels() * 8);
		switch (buttonState)
		{
		case NONE:
			DrawImage();
			break;
		case FACE_DETECT:
			MLDetectFace(false);
			break;
		case EYE_SENSOR:
			MLDetectFace(true);
			break;
		}
		break;
	default:
		break;
	}
	
	CDialogEx::OnTimer(nIDEvent);
}

void CBichonCamDlg::CreateBitmapInfo(BITMAPINFO** bmInfo, int width, int height, int bpp)
{
	if (*bmInfo != NULL)
	{
		delete[] * bmInfo;
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

void CBichonCamDlg::DrawImage()
{
	CClientDC dc(GetDlgItem(IDC_CAMERA_CONTROL));
	CRect rect;
	GetDlgItem(IDC_CAMERA_CONTROL)->GetClientRect(&rect);

	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(
		dc.GetSafeHdc(),
		0,
		0,
		rect.Width(),
		rect.Height(),
		0,
		0,
		g_image.cols,
		g_image.rows,
		g_image.data,
		g_bmInfo,
		DIB_RGB_COLORS,
		SRCCOPY);
}

void CBichonCamDlg::MLDetectFace(bool eyeSensor)
{
	CascadeClassifier eyeClassifier("haarcascade_eye.xml");

	Mat blob = dnn::blobFromImage(g_image, 1, Size(300, 300), Scalar(104, 177, 123));
	g_net.setInput(blob);
	Mat res = g_net.forward();

	Mat detect(res.size[2], res.size[3], CV_32FC1, res.ptr<float>());

	for (int i = 0; i < detect.rows; ++i)
	{
		float confidence = detect.at<float>(i, 2);
		if (confidence < 0.5)
			break;

		int x1 = cvRound(detect.at<float>(i, 3) * g_image.cols);
		int y1 = cvRound(detect.at<float>(i, 4) * g_image.rows);
		int x2 = cvRound(detect.at<float>(i, 5) * g_image.cols);
		int y2 = cvRound(detect.at<float>(i, 6) * g_image.rows);

		if (eyeSensor)
		{
			Mat faceROI;
			faceROI = g_image(Rect(Point(x1, y1), Point(x2, y2)));
			std::vector<Rect> eyes;
			eyeClassifier.detectMultiScale(faceROI, eyes);

			int minX = 0, minY = 0, maxX = 0, maxY = 0;

			if (eyes.size() > 0)
			{
				minX = eyes[0].x;
				minY = eyes[0].y;
			}

			for (Rect eye : eyes)
			{
				if (minX > eye.x)
					minX = eye.x;
				if (minY > eye.y)
					minY = eye.y;
				if (maxX < eye.x)
					maxX = eye.x;
				if (maxY < eye.y)
					maxY = eye.y;
			}

			rectangle(faceROI, Rect(minX, minY, 200, 40), Scalar(0, 0, 200), -1);
		}
		else
		{
			rectangle(g_image, Rect(Point(x1, y1), Point(x2, y2)), Scalar(0, 255, 0), 2);
		}

		String label = format("Face : %4.3f", confidence);
		putText(g_image, label, Point(x1, y1 - 1), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 255, 0));
	}

	DrawImage();
}

void CBichonCamDlg::OnBnClickedDetectFaceButton()
{
	buttonState = FACE_DETECT;
}

void CBichonCamDlg::OnBnClickedEyeSensorButton()
{
	buttonState = EYE_SENSOR;
}

void CBichonCamDlg::OnBnClickedClearButton()
{
	buttonState = NONE;
}
