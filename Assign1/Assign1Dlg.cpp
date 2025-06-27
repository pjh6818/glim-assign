
// Assign1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Assign1.h"
#include "Assign1Dlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CAssign1Dlg 대화 상자



CAssign1Dlg::CAssign1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASSIGN1_DIALOG, pParent),
	m_draggingPoint(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAssign1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAssign1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CAssign1Dlg::OnBnClickedButtonReset)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_EN_CHANGE(IDC_EDIT_RADIUS, &CAssign1Dlg::OnEnChangeEditRadius)
	ON_EN_CHANGE(IDC_EDIT_THICKNESS, &CAssign1Dlg::OnEnChangeEditThickness)
	ON_BN_CLICKED(IDC_BUTTON_RANDOM, &CAssign1Dlg::OnBnClickedButtonRandom)
	ON_MESSAGE(MESSAGE_DRAW_RANDOM, &CAssign1Dlg::DrawRandom)
END_MESSAGE_MAP()


// CAssign1Dlg 메시지 처리기

BOOL CAssign1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	if (!Initialize())
	{
		AfxMessageBox(_T("이미지 생성 실패!"));
		return FALSE;
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CAssign1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CAssign1Dlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	if (!m_image.IsNull())
		m_image.Draw(dc.GetSafeHdc(), 0, 0);
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CAssign1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool isInCircle(int x, int y, int nCenterX, int nCenterY, int nRadius)
{
	double dX = x - nCenterX;
	double dY = y - nCenterY;
	double dDist = dX * dX + dY * dY;

	if (dDist < nRadius * nRadius)
		return true;

	return false;
}

void CAssign1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!(point.x >= 0 && point.x < m_image.GetWidth() && point.y >= 0 && point.y < m_image.GetHeight()))
	{
		CDialogEx::OnLButtonDown(nFlags, point);
		return;
	}

	for (CPoint& pt : m_points)
	{
		if (isInCircle(point.x, point.y, pt.x, pt.y, m_nPointRadius * 2))
		{
			m_bDragging = true;
			m_draggingPoint = &pt;

			CDialogEx::OnLButtonDown(nFlags, point);
			return;
		}
	}

	if (m_points.size() < 3)
	{
		m_points.push_back(point);

		unsigned char* fm = (unsigned char*)m_image.GetBits();

		DrawCircle(fm, point.x, point.y, m_nPointRadius, 0);

		if (m_points.size() == 3)
		{
			DrawCircleAcross(fm, 0);

			for (CPoint pt : m_points)
				DrawCircle(fm, pt.x, pt.y, m_nPointRadius, 0);
		}

		UpdateDisplay();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CAssign1Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDragging && m_draggingPoint)
	{
		unsigned char* fm = (unsigned char*)m_image.GetBits();

		DrawCircle(fm, m_draggingPoint->x, m_draggingPoint->y, m_nPointRadius, 255);
		DrawCircle(fm, point.x, point.y, m_nPointRadius, 0);

		m_draggingPoint->x = point.x;
		m_draggingPoint->y = point.y;

		if (m_points.size() == 3)
		{
			memset(fm, 255, m_image.GetWidth() * m_image.GetHeight());

			DrawCircleAcross(fm, 0);

			for (CPoint pt : m_points)
				DrawCircle(fm, pt.x, pt.y, m_nPointRadius, 0);
		}

		UpdateDisplay();
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CAssign1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bDragging = false;
	m_draggingPoint = NULL;

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CAssign1Dlg::OnBnClickedButtonReset()
{
	Reset();

	UpdateDisplay();
}

void CAssign1Dlg::OnEnChangeEditRadius()
{
	BOOL success;
	int nValue = GetDlgItemInt(IDC_EDIT_RADIUS, &success);

	if (success)
	{
		m_nPointRadius = max(1, min(50, nValue)); // 사용자 설정 반지름 값을 1~50 범위가 되도록 설정

		if (nValue < 1)
			SetDlgItemInt(IDC_EDIT_RADIUS, 1);

		if (nValue > 50)
			SetDlgItemInt(IDC_EDIT_RADIUS, 50);

		Redraw();
	}
}

void CAssign1Dlg::OnEnChangeEditThickness()
{
	BOOL success;
	int nValue = GetDlgItemInt(IDC_EDIT_THICKNESS, &success);

	if (success)
	{
		m_nLineThickness = max(1, min(m_nPointRadius, nValue)); // 사용자 설정 선두께 값을 1~반지름 범위가 되도록 설정

		if (nValue < 1)
			SetDlgItemInt(IDC_EDIT_THICKNESS, 1);

		if (nValue > m_nPointRadius)
			SetDlgItemInt(IDC_EDIT_THICKNESS, m_nPointRadius);

		Redraw();
	}
}

LRESULT CAssign1Dlg::DrawRandom(WPARAM wParam, LPARAM lParam)
{
	if (!m_isWorkingThread)
		return 0;

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> distW(0, m_image.GetWidth()), distH(0, m_image.GetHeight());
	
	for (int i = 0; i < m_points.size(); i++)
	{
		int randW = distW(mt);
		int randH = distH(mt);

		m_points[i].x = randW;
		m_points[i].y = randH;
	}
	
	Redraw();

	return 0;
}

UINT CAssign1Dlg::ThreadProc(LPVOID param)
{
	CAssign1Dlg* pThis = (CAssign1Dlg*)param;

	for (int i = 0; i < 10; i++)
	{
		pThis->PostMessage(MESSAGE_DRAW_RANDOM, NULL, NULL);

		Sleep(500);
	}

	pThis->m_isWorkingThread = false;

	return 0;
}

void CAssign1Dlg::OnBnClickedButtonRandom()
{
	if (m_points.size() < 3)
	{
		AfxMessageBox(L"정원을 만든 후 랜덤 이동을 눌러주세요.");
		return;
	}

	if (m_isWorkingThread)
	{
		AfxMessageBox(L"랜덤 이동이 이미 실행 중 입니다.");
		return;
	}

	if (!m_isWorkingThread)
	{
		m_pThread = AfxBeginThread(ThreadProc, this);
	}

	m_isWorkingThread = !m_isWorkingThread;
}

bool CAssign1Dlg::Initialize()
{
	int nWidth = 640;
	int nHeight = 480;
	int nBpp = 8;

	if (!m_image.Create(nWidth, -nHeight, nBpp))
		return false;

	if (nBpp == 8) {
		static RGBQUAD rgb[256];

		for (int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_image.SetColorTable(0, 256, rgb);
	}

	unsigned char* fm = (unsigned char*)m_image.GetBits();
	memset(fm, 0xff, nWidth * nHeight);

	m_nPointRadius = 10;
	m_nLineThickness = 3;

	m_bDragging = false;


	SetDlgItemInt(IDC_EDIT_RADIUS, m_nPointRadius);
	SetDlgItemInt(IDC_EDIT_THICKNESS, m_nLineThickness);

	return true;
}

void CAssign1Dlg::UpdateDisplay()
{
	CClientDC dc(this);
	m_image.Draw(dc, 0, 0);
}

void CAssign1Dlg::DrawCircle(unsigned char* fm, int x, int y, int radius, int gray)
{
	int nPitch = m_image.GetPitch();
	int nLastIdx = m_image.GetWidth() * m_image.GetHeight();

	for (int j = y - radius; j < y + radius; j++) {
		for (int i = x - radius; i < x + radius; i++) {
			if (j < 0 || j > m_image.GetHeight())
				continue;

			if (i < 0 || i > m_image.GetWidth())
				continue;

			int idx = j * nPitch + i;

			if (0 <= idx && idx < nLastIdx && isInCircle(i, j, x, y, radius))
				fm[idx] = gray;
		}
	}
}

typedef std::pair<CPoint, double> Circle;

std::pair<bool, Circle> calculateCircleFromThreem_points(CPoint p1, CPoint p2, CPoint p3)
{
	const double epsilon = 1e-9;

	double a = (double)(p2.y - p1.y) / (p2.x - p1.x);
	double b = (double)(p3.y - p2.y) / (p3.x - p2.x);
	double gradDiff = std::abs(a - b);

	if (gradDiff < epsilon)
		return { false, {} };
	
	double A = 2 * (p2.x - p1.x);
	double B = 2 * (p2.y - p1.y);
	double C = p2.x * p2.x + p2.y * p2.y - p1.x * p1.x - p1.y * p1.y;

	double D = 2 * (p3.x - p2.x);
	double E = 2 * (p3.y - p2.y);
	double F = p3.x * p3.x + p3.y * p3.y - p2.x * p2.x - p2.y * p2.y;

	double determinant = A * E - B * D;

	if (std::abs(determinant) < epsilon)
		return { false, {} };

	CPoint center;
	center.x = (C * E - F * B) / determinant;
	center.y = (A * F - D * C) / determinant;

	double radius;

	double dx = p1.x - center.x;
	double dy = p1.y - center.y;
	radius = std::sqrt(dx * dx + dy * dy);

	return { true, { center, radius } };
}

void CAssign1Dlg::DrawCircleAcross(unsigned char* fm, int gray)
{
	if (m_points.size() < 3)
		return;

	auto [success, result] = calculateCircleFromThreem_points(m_points[0], m_points[1], m_points[2]);

	if (!success)
	{
		AfxMessageBox(L"세 점이 일직선이 되어 원을 그릴 수 없습니다.");

		Reset();
		
		return;
	}

	int radius = (int)(result.second + 0.5);
	CPoint center = result.first;

	int outerRadius, innerRadius;

	if (m_nLineThickness % 2 == 0)
		innerRadius = radius - m_nLineThickness / 2;
	else
		innerRadius = radius - m_nLineThickness / 2 - 1;
	
	DrawCircle(fm, center.x, center.y, radius + m_nLineThickness / 2, 0);
	DrawCircle(fm, center.x, center.y, innerRadius, 255);
}

void CAssign1Dlg::Redraw()
{
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	memset(fm, 255, m_image.GetWidth() * m_image.GetHeight());

	if (m_points.size() == 3)
		DrawCircleAcross(fm, 0);

	for (CPoint pt : m_points)
		DrawCircle(fm, pt.x, pt.y, m_nPointRadius, 0);
	
	UpdateDisplay();
}

void CAssign1Dlg::Reset()
{
	m_points.clear();

	memset((unsigned char*)m_image.GetBits(), 255, m_image.GetWidth() * m_image.GetHeight());
}
