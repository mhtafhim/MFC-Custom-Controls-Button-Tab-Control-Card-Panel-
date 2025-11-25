#include "pch.h"
#include "CardPanel.h"

// Ensure GDI+ Library is linked
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;

IMPLEMENT_DYNAMIC(CCardPanel, CWnd)

CCardPanel::CCardPanel()
{
}

CCardPanel::~CCardPanel()
{
}

// ---------------------------------------------------------
// CRITICAL: This registers the name "MFC_CardPanel" with Windows
// ---------------------------------------------------------
BOOL CCardPanel::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    // Check if already registered
    if (!(::GetClassInfo(hInst, _T("MFC_CardPanel"), &wndcls)))
    {
        wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc = ::DefWindowProc;
        wndcls.cbClsExtra = 0;
        wndcls.cbWndExtra = 0;
        wndcls.hInstance = hInst;
        wndcls.hIcon = NULL;
        wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1); // Default Gray
        wndcls.lpszMenuName = NULL;

        // THIS NAME MUST MATCH THE RESOURCE EDITOR EXACTLY
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

// Prevent flicker
BOOL CCardPanel::OnEraseBkgnd(CDC* pDC)
{
    return TRUE; // We handle all painting in OnPaint
}

void CCardPanel::OnPaint()
{
    CPaintDC dc(this);

    CRect rectClient;
    GetClientRect(&rectClient);

    // Double Buffer
    CMemDC memDC(dc, this);
    CDC* pDC = &memDC.GetDC();

    // 1. FILL BACKGROUND 
    // Match this color to your Main Dialog Background (Light Blue)
    pDC->FillSolidRect(rectClient, RGB(255, 255, 255));

    Graphics graphics(pDC->GetSafeHdc());
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    // 2. DEFINE BOX
    float margin = 10.0f;
    RectF rectWhite(
        (REAL)rectClient.left + margin, (REAL)rectClient.top + margin,
        (REAL)rectClient.Width() - (margin * 2), (REAL)rectClient.Height() - (margin * 2));
    float radius = 12.0f;

    // 3. DRAW SHADOW (Simple soft shadow)
    {
        RectF r = rectWhite;
        r.Inflate(4.0f, 4.0f); // Make bigger
        r.Offset(-2.0, -2.0);     // Move down

        GraphicsPath path;
        AddRoundedRectToPath(path, r, radius + 4.0f); // Radius + Inflate
        SolidBrush b(Color(40, 0, 0, 0)); // Transparent Black
        graphics.FillPath(&b, &path);
    }

    // 4. DRAW WHITE CARD
    GraphicsPath path;
    AddRoundedRectToPath(path, rectWhite, radius);

    SolidBrush brushWhite(Color(255, 255, 255, 255));
    graphics.FillPath(&brushWhite, &path);

    Pen penBorder(Color(255, 220, 220, 220), 1.0f);
    graphics.DrawPath(&penBorder, &path);
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
    // HTTRANSPARENT tells Windows: 
    // "I am not here. Pass the click through me to the window behind me 
    //  OR check if there is a sibling window (like a button) at this location."
    return HTTRANSPARENT;
}
