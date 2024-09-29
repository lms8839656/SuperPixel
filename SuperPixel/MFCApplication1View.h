
// MFCApplication1View.h: CMFCApplication1View 클래스의 인터페이스
//

#pragma once


class CMFCApplication1View : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CMFCApplication1View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication1View)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_MFCAPPLICATION1_FORM };
#endif

// 특성입니다.
public:
	CMFCApplication1Doc* GetDocument() const;
	
// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMFCApplication1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CButton m_image;
	CMFCEditBrowseCtrl m_selfile;
	afx_msg void OnEnChangeSelfile();
	CStatic m_before;
	CStatic m_after;

	CString ImgPath;
	CRect rect1;
	CRect rect2;
	char path[256];
	char per[256];

	int r;
	int g;
	int b;
	
	int check;
	int checkv;
	int checkh;
	int checki;

	unsigned int list_num;
	unsigned int* list_x;
	unsigned int* list_y;
	unsigned int* list_area;
	float* list_B, * list_G, * list_R;
	int list_selected = -1;
	
	void SuperImage();
	void SuperImage2();
	void ShowList();

	int label;

	afx_msg void OnPaint();
	int m_region;
	float edit_regulation;
	CListCtrl m_list;
	COLORREF color;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnNMClickList(NMHDR* pNMHDR, LRESULT* pResult);
	char tax[256] = { 0 };
	CButton m_check;
	afx_msg void OnBnClickedCheck();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedCheck2();
	CButton m_check2;
	afx_msg void OnBnClickedButton4();
	CEdit m_regul;
	afx_msg void OnBnClickedButton5();
	CMFCEditBrowseCtrl ddd;
};

#ifndef _DEBUG  // MFCApplication1View.cpp의 디버그 버전
inline CMFCApplication1Doc* CMFCApplication1View::GetDocument() const
   { return reinterpret_cast<CMFCApplication1Doc*>(m_pDocument); }
#endif

