
// Assign1Dlg.h: 헤더 파일
//

#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>


#define MESSAGE_DRAW_RANDOM WM_USER

// CAssign1Dlg 대화 상자
class CAssign1Dlg : public CDialogEx
{
// 생성입니다.
public:
	CAssign1Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ASSIGN1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEnChangeEditRadius();
	afx_msg void OnEnChangeEditThickness();
	afx_msg void OnBnClickedButtonRandom();

private:
	CImage m_image;
	std::vector<CPoint> m_points;
	int m_nPointRadius;
	int m_nLineThickness;

	bool m_bDragging;
	CPoint* m_draggingPoint;

	CWinThread* m_pThread;
	bool m_isWorkingThread;

private:
	bool Initialize();
	void UpdateDisplay();
	void DrawCircle(unsigned char* fm, int i, int j, int radius, int gray);
	void DrawCircleAcross(unsigned char* fm, int gray);
	void Redraw();
	void Reset();

	LRESULT DrawRandom(WPARAM wParam, LPARAM lParam);
	static UINT ThreadProc(LPVOID param);
};
