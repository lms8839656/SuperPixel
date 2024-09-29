
// MFCApplication1View.cpp: CMFCApplication1View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"
#include "MainFrm.h"
#include <opencv2/opencv.hpp>
extern "C" {
#include "vl/generic.h"
#include "vl/slic.h"
}
#include "GdiPlus.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <iostream>
ULONG_PTR gdiplusToken;
using namespace Gdiplus;
// CMFCApplication1View
#pragma comment(lib, "Gdiplus.lib")

IMPLEMENT_DYNCREATE(CMFCApplication1View, CFormView)

BEGIN_MESSAGE_MAP(CMFCApplication1View, CFormView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1View::OnBnClickedButton1)
    ON_EN_CHANGE(IDC_SelFile, &CMFCApplication1View::OnEnChangeSelfile)
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1View::OnBnClickedButton2)
    ON_NOTIFY(NM_CLICK, IDC_LIST, &CMFCApplication1View::OnNMClickList)
    ON_BN_CLICKED(IDC_CHECK, &CMFCApplication1View::OnBnClickedCheck)
    ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1View::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_CHECK2, &CMFCApplication1View::OnBnClickedCheck2)
    ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication1View::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication1View::OnBnClickedButton5)
    ON_EN_CHANGE(IDC_REGION, &CMFCApplication1View::OnEnChangeRegion)
END_MESSAGE_MAP()

// CMFCApplication1View 생성/소멸

CMFCApplication1View::CMFCApplication1View()
    noexcept
	: CFormView(IDD_MFCAPPLICATION1_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFCApplication1View::~CMFCApplication1View()
{
    ::GdiplusShutdown(gdiplusToken);
}

void CMFCApplication1View::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BUTTON1, m_image);
    DDX_Control(pDX, IDC_SelFile, m_selfile);


    DDX_Text(pDX, IDC_REGION, m_region);

    DDX_Control(pDX, IDC_LIST, m_list);
    DDX_Control(pDX, IDC_CHECK, m_check);
    DDX_Control(pDX, IDC_CHECK2, m_check2);
    DDX_Control(pDX, IDC_REGULATION, m_regul);

    DDX_Control(pDX, IDC_MFCEDITBROWSE1, ddd);
}

BOOL CMFCApplication1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
    GdiplusStartupInput gdiplusStartupInput;
    if (::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) != Ok) {
        AfxMessageBox(_T("Error : Failed to initialize GDI+ library"));
        return FALSE;
    }

	return CFormView::PreCreateWindow(cs);
}

void CMFCApplication1View::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

    m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    m_list.InsertColumn(0, L"NUM", LVCFMT_CENTER, 50);
    m_list.InsertColumn(1, L"x", LVCFMT_CENTER, 50);
    m_list.InsertColumn(2, L"y", LVCFMT_CENTER, 50);
    m_list.InsertColumn(3, L"area", LVCFMT_CENTER, 50);
    m_list.InsertColumn(4, L"color(R, G, B)", LVCFMT_CENTER, 150);

    check = 0;

    m_region = 30;
    edit_regulation = 1000;

    SetDlgItemInt(IDC_REGION, m_region);
    m_regul.SetWindowText(_T("0.1"));

    rect1 = { 50,70,300,320 };
    rect2 = { 500,70,300,320 };

    cv::Mat nothing = cv::imread("nothing.png", cv::IMREAD_UNCHANGED);
    
    for (int i = 0; i < nothing.rows;i++)
        for (int j = 0; j < nothing.cols; j++) {
            nothing.at<cv::Vec4b>(i, j)[0] = cv::saturate_cast<uchar>(1.0 * 0xff);
            nothing.at<cv::Vec4b>(i, j)[1] = cv::saturate_cast<uchar>(1.0 * 0xff);
            nothing.at<cv::Vec4b>(i, j)[2] = cv::saturate_cast<uchar>(1.0 * 0x0);
            nothing.at<cv::Vec4b>(i, j)[3] = 0x00;
        }
    cv::imwrite("contours.png", nothing);
    
    ImgPath = L"nothing.png";
    
}


// CMFCApplication1View 인쇄

BOOL CMFCApplication1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCApplication1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCApplication1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMFCApplication1View::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 여기에 사용자 지정 인쇄 코드를 추가합니다.
}


// CMFCApplication1View 진단

#ifdef _DEBUG
void CMFCApplication1View::AssertValid() const
{
	CFormView::AssertValid();
}

void CMFCApplication1View::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMFCApplication1Doc* CMFCApplication1View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication1Doc)));
	return (CMFCApplication1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication1View 메시지 처리기

void CMFCApplication1View::OnPaint()
{
    CPaintDC dc(this);

    Graphics ScreenG(dc.GetSafeHdc());

    Bitmap* pBitmap = (Bitmap*)Image::FromFile(ImgPath);
    Bitmap* tBitmap;
    check ? tBitmap = (Bitmap*)Image::FromFile(L"temp2.png") : tBitmap = (Bitmap*)Image::FromFile(L"contours.png");
    


    dc.Rectangle(45, 65, 455, 575);
    dc.Rectangle(495, 65, 905, 575);
    ScreenG.DrawImage(pBitmap, rect1.left, rect1.top, 400, 500);
    ScreenG.DrawImage(tBitmap, rect2.left, rect2.top, 400, 500);


    delete pBitmap;
    delete tBitmap;
}

void CMFCApplication1View::OnBnClickedButton1()
{

    m_region = GetDlgItemInt(IDC_REGION);
    edit_regulation = (float)(10000*GetDlgItemInt(IDC_REGULATION));

    SuperImage();
    Invalidate();
    ShowList();    
}


void CMFCApplication1View::OnEnChangeSelfile()
{
    
    
    m_selfile.GetWindowText(ImgPath);

    // 이미지 Mat형식
   
    size_t getVal;
    wcstombs_s(&getVal, path, (ImgPath.GetLength() + 1), ImgPath, _TRUNCATE);

    //SuperImage();
    Invalidate();
    ShowList();
}

void CMFCApplication1View::ShowList()
{
    m_list.DeleteAllItems();

    CString num, midx, midy, area, color;
    int nitem;

    for (int i = list_num;i > 0;i--) {
        num.Format(_T("%d"), i);
        midx.Format(_T("%d"), list_x[i]);
        midy.Format(_T("%d"), list_y[i]);
        area.Format(_T("%d"), list_area[i]);
        color.Format(_T("%.0f,%.0f,%.0f"), list_R[i], list_G[i], list_B[i]);

        nitem = m_list.InsertItem(0, num);
        m_list.SetItemText(nitem, 1, midx);
        m_list.SetItemText(nitem, 2, midy);
        m_list.SetItemText(nitem, 3, area);
        m_list.SetItemText(nitem, 4, color);
    }
}


void CMFCApplication1View::SuperImage()
{
    // Convert image to one-dimensional array.


    cv::Mat mat = cv::imread(path, cv::IMREAD_COLOR);

    cv::imwrite("org.png", mat);
    cv::imwrite("temp.png", mat);
    if (mat.empty()) {
        std::cout << "cannot read images.\n";
        return;
    }

    list_x = (unsigned int*)malloc(250*250);
    list_y = (unsigned int*)malloc(250 * 250);
    list_area = (unsigned int*)malloc(250 * 250);
    list_R = (float*)malloc(250 * 250);
    list_G = (float*)malloc(250 * 250);
    list_B = (float*)malloc(250 * 250);


    float* image = new float[mat.rows * mat.cols * mat.channels()];
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            image[j + mat.cols * i + mat.cols * mat.rows * 0] = mat.at<cv::Vec3b>(i, j)[0];
            image[j + mat.cols * i + mat.cols * mat.rows * 1] = mat.at<cv::Vec3b>(i, j)[1];
            image[j + mat.cols * i + mat.cols * mat.rows * 2] = mat.at<cv::Vec3b>(i, j)[2];
        }
    }
    // The algorithm will store the final segmentation in a one-dimensional array.
    vl_uint32* segmentation = new vl_uint32[mat.rows * mat.cols];
    vl_size height = mat.rows;
    vl_size width = mat.cols;
    vl_size channels = mat.channels();
    vl_size minRegion = 10;

    vl_slic_segment(segmentation, image, width, height, channels, m_region, edit_regulation, minRegion);

    // Convert segmentation.
    int** labels = new int* [mat.rows];
    for (int i = 0; i < mat.rows; ++i) {
        labels[i] = new int[mat.cols];

        for (int j = 0; j < mat.cols; ++j) {
            labels[i][j] = (int)segmentation[j + mat.cols * i];
        }
    }

    int label = 0;
    int labelTop = -1;
    int labelBottom = -1;
    int labelLeft = -1;
    int labelRight = -1;

    float* image_R = new float[mat.rows * mat.cols];
    float* image_G = new float[mat.rows * mat.cols];
    float* image_B = new float[mat.rows * mat.cols];

    float* pixel = new float[mat.rows * mat.cols];

    int* pixelw = new int[mat.rows * mat.cols];
    int* pixelh = new int[mat.rows * mat.cols];

    float* count_width = new float[mat.rows * mat.cols];
    float* count_height = new float[mat.rows * mat.cols];

    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j)
        {
            image_R[j + mat.cols * i] = 0;
            image_G[j + mat.cols * i] = 0;
            image_B[j + mat.cols * i] = 0;
            pixel[j + mat.cols * i] = 0;
            pixelw[j + mat.cols * i] = 0;
            pixelh[j + mat.cols * i] = 0;
            count_width[j + mat.cols * i] = 0;
            count_height[j + mat.cols * i] = 0;
        }

    }


    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {

            label = labels[i][j];

            labelTop = label;
            if (i > 0) {
                labelTop = labels[i - 1][j];
            }

            labelBottom = label;
            if (i < mat.rows - 1) {
                labelBottom = labels[i + 1][j];
            }

            labelLeft = label;
            if (j > 0) {
                labelLeft = labels[i][j - 1];
            }

            labelRight = label;
            if (j < mat.cols - 1) {
                labelRight = labels[i][j + 1];
            }

            int state_check = m_check.GetCheck();
            int state_check2 = m_check2.GetCheck();
 
            

            pixel[label]++;

            image_B[label] += mat.at<cv::Vec3b>(i, j)[0];
            image_G[label] += mat.at<cv::Vec3b>(i, j)[1];
            image_R[label] += mat.at<cv::Vec3b>(i, j)[2];

            pixelh[label] += i;
            count_height[label]++;
            pixelw[label] += j;
            count_width[label]++;
            

            list_area[label ] = unsigned int(pixel[label]);

            list_R[label +1 ] = float(image_R[label] / pixel[label]);
            list_G[label +1 ] = float(image_G[label] / pixel[label]);
            list_B[label +1 ] = float(image_B[label] / pixel[label]);

            if (state_check2 == 1)
            {
                mat.at<cv::Vec3b>(i, j)[0] = list_B[label + 1];
                mat.at<cv::Vec3b>(i, j)[1] = list_G[label + 1];
                mat.at<cv::Vec3b>(i, j)[2] = list_R[label + 1];
            }

            if (label != labelTop || label != labelBottom || label != labelLeft || label != labelRight) {
                if (tax[label] == 1 && state_check == 1) {
                    mat.at<cv::Vec3b>(i, j)[0] = 255 - GetBValue(color);
                    mat.at<cv::Vec3b>(i, j)[1] = 255 - GetGValue(color);
                    mat.at<cv::Vec3b>(i, j)[2] = 255 - GetRValue(color);
                }
                else if (tax[label] == 0 && state_check == 1) {
                    mat.at<cv::Vec3b>(i, j)[0] = GetBValue(color);
                    mat.at<cv::Vec3b>(i, j)[1] = GetGValue(color);
                    mat.at<cv::Vec3b>(i, j)[2] = GetRValue(color);
                }
            }
           
        }
    }
    list_num = label + 1;

    for (unsigned int i = 0; i < list_num; i++) {
        for (int k = 0; k < mat.rows;k++) {
            for (int l = 0; l < mat.cols;l++) {
                if ((int(pixelh[i] / count_height[i])) == k && (int(pixelh[i] / count_width[i])) == l) {
                    list_x[i + 1] = l;
                    list_y[i + 1] = k;
                }
            }
        }
    }

   cv::imwrite("contours.png", mat);

   delete []image_R;
   delete []image_G;
   delete []image_B;

   delete []pixel;

   delete []pixelw;
   delete []pixelh;

   delete []count_width;
   delete []count_height;

   
   return;
}


void CMFCApplication1View::SuperImage2()
{
    // Convert image to one-dimensional array.


    cv::Mat mat = cv::imread("temp.png", cv::IMREAD_COLOR);


    if (mat.empty()) {
        std::cout << "cannot read images.\n";
        return;
    }

    list_x = (unsigned int*)malloc(250 * 250);
    list_y = (unsigned int*)malloc(250 * 250);
    list_area = (unsigned int*)malloc(250 * 250);
    list_R = (float*)malloc(250 * 250);
    list_G = (float*)malloc(250 * 250);
    list_B = (float*)malloc(250 * 250);


    float* image = new float[mat.rows * mat.cols * mat.channels()];
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            image[j + mat.cols * i + mat.cols * mat.rows * 0] = mat.at<cv::Vec3b>(i, j)[0];
            image[j + mat.cols * i + mat.cols * mat.rows * 1] = mat.at<cv::Vec3b>(i, j)[1];
            image[j + mat.cols * i + mat.cols * mat.rows * 2] = mat.at<cv::Vec3b>(i, j)[2];
        }
    }
    // The algorithm will store the final segmentation in a one-dimensional array.
    vl_uint32* segmentation = new vl_uint32[mat.rows * mat.cols];
    vl_size height = mat.rows;
    vl_size width = mat.cols;
    vl_size channels = mat.channels();
    vl_size minRegion = 10;

    vl_slic_segment(segmentation, image, width, height, channels, m_region, edit_regulation, minRegion);

    // Convert segmentation.
    int** labels = new int* [mat.rows];
    for (int i = 0; i < mat.rows; ++i) {
        labels[i] = new int[mat.cols];

        for (int j = 0; j < mat.cols; ++j) {
            labels[i][j] = (int)segmentation[j + mat.cols * i];
        }
    }

    int label = 0;
    int labelTop = -1;
    int labelBottom = -1;
    int labelLeft = -1;
    int labelRight = -1;

    float* image_R = new float[mat.rows * mat.cols];
    float* image_G = new float[mat.rows * mat.cols];
    float* image_B = new float[mat.rows * mat.cols];

    float* pixel = new float[mat.rows * mat.cols];

    int* pixelw = new int[mat.rows * mat.cols];
    int* pixelh = new int[mat.rows * mat.cols];

    float* count_width = new float[mat.rows * mat.cols];
    float* count_height = new float[mat.rows * mat.cols];

    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j)
        {
            image_R[j + mat.cols * i] = 0;
            image_G[j + mat.cols * i] = 0;
            image_B[j + mat.cols * i] = 0;
            pixel[j + mat.cols * i] = 0;
            pixelw[j + mat.cols * i] = 0;
            pixelh[j + mat.cols * i] = 0;
            count_width[j + mat.cols * i] = 0;
            count_height[j + mat.cols * i] = 0;
        }

    }


    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {

            label = labels[i][j];

            labelTop = label;
            if (i > 0) {
                labelTop = labels[i - 1][j];
            }

            labelBottom = label;
            if (i < mat.rows - 1) {
                labelBottom = labels[i + 1][j];
            }

            labelLeft = label;
            if (j > 0) {
                labelLeft = labels[i][j - 1];
            }

            labelRight = label;
            if (j < mat.cols - 1) {
                labelRight = labels[i][j + 1];
            }

            int state_check = m_check.GetCheck();
            int state_check2 = m_check2.GetCheck();



            pixel[label]++;

            image_B[label] += mat.at<cv::Vec3b>(i, j)[0];
            image_G[label] += mat.at<cv::Vec3b>(i, j)[1];
            image_R[label] += mat.at<cv::Vec3b>(i, j)[2];

            pixelh[label] += i;
            count_height[label]++;
            pixelw[label] += j;
            count_width[label]++;


            list_area[label] = unsigned int(pixel[label]);

            list_R[label + 1] = float(image_R[label] / pixel[label]);
            list_G[label + 1] = float(image_G[label] / pixel[label]);
            list_B[label + 1] = float(image_B[label] / pixel[label]);

            if (state_check2 == 1)
            {
                mat.at<cv::Vec3b>(i, j)[0] = list_B[label + 1];
                mat.at<cv::Vec3b>(i, j)[1] = list_G[label + 1];
                mat.at<cv::Vec3b>(i, j)[2] = list_R[label + 1];
            }

            if (label != labelTop || label != labelBottom || label != labelLeft || label != labelRight) {
                if (tax[label] == 1 && state_check == 1) {
                    mat.at<cv::Vec3b>(i, j)[0] = 255 - GetBValue(color);
                    mat.at<cv::Vec3b>(i, j)[1] = 255 - GetGValue(color);
                    mat.at<cv::Vec3b>(i, j)[2] = 255 - GetRValue(color);
                }
                else if (tax[label] == 0 && state_check == 1) {
                    mat.at<cv::Vec3b>(i, j)[0] = GetBValue(color);
                    mat.at<cv::Vec3b>(i, j)[1] = GetGValue(color);
                    mat.at<cv::Vec3b>(i, j)[2] = GetRValue(color);
                }
            }

        }
    }
    list_num = label + 1;

    for (unsigned int i = 0; i < list_num; i++) {
        for (int k = 0; k < mat.rows; k++) {
            for (int l = 0; l < mat.cols; l++) {
                if ((int(pixelh[i] / count_height[i])) == k && (int(pixelh[i] / count_width[i])) == l) {
                    list_x[i + 1] = l;
                    list_y[i + 1] = k;
                }
            }
        }
    }

    cv::imwrite("temp2.png", mat);

    delete[]image_R;
    delete[]image_G;
    delete[]image_B;

    delete[]pixel;

    delete[]pixelw;
    delete[]pixelh;

    delete[]count_width;
    delete[]count_height;
    return;
}


void CMFCApplication1View::OnMouseMove(UINT nFlags, CPoint point)
{
    CClientDC dc(this);
    COLORREF color = dc.GetPixel(point);
    CString str;

    str.Format(TEXT("(%4d, %4d, %4d)"), GetRValue(color), GetGValue(color), GetBValue(color));
    CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
    pMainFrame->m_wndStatusBar.SetPaneText(1, str);
}

/////////////*외곽선 색상변경*//////////////////////
void CMFCApplication1View::OnBnClickedButton2()
{
    CColorDialog dlg;
    dlg.DoModal();
    color = dlg.GetColor();

    check ? SuperImage2() : SuperImage();
    Invalidate();
}


void CMFCApplication1View::OnNMClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

    int count = m_list.GetItemCount();

    for (int i = count; i >= 0; i--) {
        if (m_list.GetItemState(i, LVIS_SELECTED) != 0) {
            tax[i] = 1;
        }
        else if (m_list.GetItemState(i, LVIS_SELECTED) == 0) {
            tax[i] = 0;
        }
    }

    check ? SuperImage2() : SuperImage();

    Invalidate();
    *pResult = 0;
}

//////////* 외곽선활성*////////////////
void CMFCApplication1View::OnBnClickedCheck()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    m_selfile.GetWindowText(ImgPath);

    size_t getVal;
    wcstombs_s(&getVal, path, (ImgPath.GetLength() + 1), ImgPath, _TRUNCATE);
    check ? SuperImage2() : SuperImage();
    Invalidate();
}

/////////////*색상 반전*/////////////////////
void CMFCApplication1View::OnBnClickedButton3()
{
    CClientDC dc(this);
    
    CImage mm_image;
    CImage invert_image;


    check = checkv + checkh + checki;

    if (check == 0)
        mm_image.Load(L"org.png"); // 외부 이미지 로드
    else
        mm_image.Load(L"temp.png");

    checki = !checki;
    invert_image.Create(mm_image.GetWidth(), mm_image.GetHeight(), mm_image.GetBPP(), 0);

    COLORREF temp_color;

    for (int y = 0; y < mm_image.GetHeight(); y++) { // 이미지의 세로 픽셀값 만큼
        for (int x = 0; x < mm_image.GetWidth(); x++) { // 이미지의 가로 픽셀값 만큼
            temp_color = mm_image.GetPixel(x, y); // 현재 픽셀의 색상을 얻어 저장

            int r = GetRValue(temp_color);
            int g = GetGValue(temp_color);
            int b = GetBValue(temp_color);

            int new_r = 255 - r;
            int new_g = 255 - g;
            int new_b = 255 - b;

            invert_image.SetPixel(x, y, RGB(new_r, new_g, new_b));
            // 반전된 RGB 값으로 새로운 이미지에 SetPixel 해줌
        }
    }
    invert_image.Save(L"temp.png");

    check ? SuperImage2() : SuperImage();

    Graphics ScreenG(dc.GetSafeHdc());
    Bitmap* tBitmap;
    check ? tBitmap = (Bitmap*)Image::FromFile(L"temp2.png") : tBitmap = (Bitmap*)Image::FromFile(L"contours.png");
    ScreenG.DrawImage(tBitmap, rect2.left, rect2.top, 400, 500);


    delete tBitmap;
}

//////////* rgb 평균*////////////////
void CMFCApplication1View::OnBnClickedCheck2()
{
    m_selfile.GetWindowText(ImgPath);

    size_t getVal;
    wcstombs_s(&getVal, path, (ImgPath.GetLength() + 1), ImgPath, _TRUNCATE);

    check ? SuperImage2() : SuperImage();
    Invalidate();
}

////////////////*좌우 거울*///////////////////
void CMFCApplication1View::OnBnClickedButton4()
{
    CClientDC dc(this);
    
    CImage mm_image;
    CImage invert_image;

    check = checkv + checkh + checki;

    if (check == 0)
        mm_image.Load(L"org.png"); // 외부 이미지 로드
    else
        mm_image.Load(L"temp.png");

    checkh = !checkh;
    invert_image.Create(mm_image.GetWidth(), mm_image.GetHeight(), mm_image.GetBPP(), 0);

    COLORREF temp_color;

    for (int y = 0; y < mm_image.GetHeight(); y++) { // 이미지의 세로 픽셀값 만큼
        for (int x = 0; x < mm_image.GetWidth(); x++) { // 이미지의 가로 픽셀값 만큼
            temp_color = mm_image.GetPixel(x, y); // 현재 픽셀의 색상을 얻어 저장

            int R = GetRValue(temp_color);
            int G = GetGValue(temp_color);
            int B = GetBValue(temp_color);

            invert_image.SetPixel(mm_image.GetWidth() - x - 1, y, RGB(R, G, B));
        }
    }
    invert_image.Save(L"temp.png");

    check ? SuperImage2() : SuperImage();

    Graphics ScreenG(dc.GetSafeHdc());
    Bitmap* tBitmap;
    check ? tBitmap = (Bitmap*)Image::FromFile(L"temp2.png") : tBitmap = (Bitmap*)Image::FromFile(L"contours.png");
    ScreenG.DrawImage(tBitmap, rect2.left, rect2.top, 400, 500);


    delete tBitmap;
}

///////////////*상하 거울*////////////////////////
void CMFCApplication1View::OnBnClickedButton5()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CClientDC dc(this);
    
    CImage mm_image;
    CImage invert_image;
    
    check = checkv + checkh + checki;

    if (check == 0)
        mm_image.Load(L"org.png"); // 외부 이미지 로드
    else
        mm_image.Load(L"temp.png");

    checkv = !checkv;

    invert_image.Create(mm_image.GetWidth(), mm_image.GetHeight(), mm_image.GetBPP(), 0);

    COLORREF temp_color;
    for (int x = 0; x < mm_image.GetWidth(); x++) { // 이미지의 가로 픽셀값 만큼
        for (int y = 0; y < mm_image.GetHeight(); y++) { // 이미지의 세로 픽셀값 만큼
            temp_color = mm_image.GetPixel(x, y); // 현재 픽셀의 색상을 얻어 저장

            int R = GetRValue(temp_color);
            int G = GetGValue(temp_color);
            int B = GetBValue(temp_color);

            invert_image.SetPixel(x, mm_image.GetHeight() - y - 1, RGB(R, G, B));
        }
    }
    invert_image.Save(L"temp.png");

    check ? SuperImage2() : SuperImage();

    Graphics ScreenG(dc.GetSafeHdc());
    Bitmap* tBitmap;
    check ? tBitmap = (Bitmap*)Image::FromFile(L"temp2.png") : tBitmap = (Bitmap*)Image::FromFile(L"contours.png");
    ScreenG.DrawImage(tBitmap, rect2.left, rect2.top, 400, 500);
    

    
    delete tBitmap;

}


void CMFCApplication1View::OnEnChangeRegion()
{
    // TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
    // CFormView::OnInitDialog() 함수를 재지정 
    //하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
    // 이 알림 메시지를 보내지 않습니다.

    // TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
