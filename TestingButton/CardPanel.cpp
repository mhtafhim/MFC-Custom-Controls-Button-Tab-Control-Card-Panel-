#include "pch.h"
#include "CardPanel.h"

// Ensure GDI+ Library is linked
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;

IMPLEMENT_DYNAMIC(CCardPanel, CWnd)

CCardPanel::CCardPanel()
{
    // Default title is empty
    m_strTitle = _T("");
}

CCardPanel::~CCardPanel()
{
}

// ... [Keep RegisterWindowClass as it is] ...
BOOL CCardPanel::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, _T("MFC_CardPanel"), &wndcls)))
    {
        wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc = ::DefWindowProc;
        wndcls.cbClsExtra = 0;
        wndcls.cbWndExtra = 0;
        wndcls.hInstance = hInst;
        wndcls.hIcon = NULL;
        wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
        wndcls.lpszMenuName = NULL;
        wndcls.lpszClassName = _T("MFC_CardPanel");
        return AfxRegisterClass(&wndcls);
    }
    return TRUE;
}

BEGIN_MESSAGE_MAP(CCardPanel, CWnd)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_NCHITTEST()
END_MESSAGE_MAP()

// --- NEW: Implementation of SetTitle ---
void CCardPanel::SetTitle(CString strTitle)
{
    m_strTitle = strTitle;
    if (GetSafeHwnd())
    {
        Invalidate(); // Trigger a repaint
        UpdateWindow();
    }
}

BOOL CCardPanel::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CCardPanel::OnPaint()
{
    CPaintDC dc(this);

    CRect rectClient;
    GetClientRect(&rectClient);

    CMemDC memDC(dc, this);
    CDC* pDC = &memDC.GetDC();

    // 1. Background (Light Blue-ish)
    pDC->FillSolidRect(rectClient, RGB(244, 247, 249));

    Graphics graphics(pDC->GetSafeHdc());
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit); // Better text quality

    // 2. Define Box Dimensions
    float margin = 10.0f;
    RectF rectWhite(
        (REAL)rectClient.left + margin, (REAL)rectClient.top + margin,
        (REAL)rectClient.Width() - (margin * 2), (REAL)rectClient.Height() - (margin * 2));
    float radius = 10.0f;

    // 3. Draw Shadow
    {
        RectF r = rectWhite;
        r.Offset(0, 2.0f); // Slight offset down

        GraphicsPath path;
        AddRoundedRectToPath(path, r, radius);

        // Very soft shadow color
        SolidBrush b(Color(20, 0, 0, 0));
        graphics.FillPath(&b, &path);
    }

    // 4. Draw White Card Background
    GraphicsPath path;
    AddRoundedRectToPath(path, rectWhite, radius);

    SolidBrush brushWhite(Color(255, 255, 255, 255));
    graphics.FillPath(&brushWhite, &path);

    Pen penBorder(Color(255, 225, 225, 225), 1.0f);
    graphics.DrawPath(&penBorder, &path);

    // 5. DRAW TITLE AND LINE (If title exists)
    if (!m_strTitle.IsEmpty())
    {
        // A. Define Font (Segoe UI, Bold, ~11pt)
        FontFamily fontFamily(L"Segoe UI");
        Gdiplus::Font font(&fontFamily, 15, FontStyleBold, UnitPixel);

        // B. Define Text Color (Dark Slate Blue: RGB 44, 62, 80)
        SolidBrush textBrush(Color(255, 44, 62, 80));

        // C. Define Position (Padding inside the white box)
        RectF textRect = rectWhite;
        textRect.Inflate(-15, -12); // Padding: Left 15, Top 12

        // D. Draw String
        StringFormat format;
        format.SetAlignment(StringAlignmentNear);
        format.SetLineAlignment(StringAlignmentNear);

        // Convert CString to WCHAR for GDI+
        graphics.DrawString(m_strTitle, -1, &font, textRect, &format, &textBrush);

        // E. Draw Divider Line
        // Calculate Y position: Title Top + Height + Padding
        float lineY = rectWhite.Y + 40.0f;

        // Light Gray Line
        Pen penLine(Color(255, 230, 230, 230), 1.0f);

        // Draw line from left (with padding) to right
        graphics.DrawLine(&penLine,
            rectWhite.X + 10.0f, lineY,
            rectWhite.GetRight() - 10.0f, lineY);
    }
}

void CCardPanel::AddRoundedRectToPath(GraphicsPath& path, RectF rect, float radius)
{
    float d = radius * 2.0f;
    path.AddArc(rect.X, rect.Y, d, d, 180, 90);
    path.AddArc(rect.X + rect.Width - d, rect.Y, d, d, 270, 90);
    path.AddArc(rect.X + rect.Width - d, rect.Y + rect.Height - d, d, d, 0, 90);
    path.AddArc(rect.X, rect.Y + rect.Height - d, d, d, 90, 90);
    path.CloseFigure();
}

LRESULT CCardPanel::OnNcHitTest(CPoint point)
{
    // Allows clicks to pass through to underlying window or overlaying controls
    return HTTRANSPARENT;
}