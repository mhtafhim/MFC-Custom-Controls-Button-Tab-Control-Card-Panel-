#include "pch.h" // Use stdafx.h for older VS
#include "InfoIcon.h"

ULONG_PTR CInfoIcon::m_gdiplusToken = 0;
int CInfoIcon::m_gdiplusRefCount = 0;

CInfoIcon::CInfoIcon()
{
    // Initialize GDI+
    if (m_gdiplusRefCount == 0)
    {
        GdiplusStartupInput gdiplusStartupInput;
        GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
    }
    m_gdiplusRefCount++;
}

CInfoIcon::~CInfoIcon()
{
    m_gdiplusRefCount--;
    if (m_gdiplusRefCount == 0)
        GdiplusShutdown(m_gdiplusToken);
}

BEGIN_MESSAGE_MAP(CInfoIcon, CStatic)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CInfoIcon::PreSubclassWindow()
{
    CStatic::PreSubclassWindow();
    ModifyStyle(0, SS_NOTIFY); // Allow mouse hover detection

    // Create Tooltip
    m_ToolTip.Create(this, TTS_ALWAYSTIP | TTS_BALLOON | TTS_NOPREFIX);
    m_ToolTip.SetMaxTipWidth(300);
    m_ToolTip.AddTool(this, _T("Info"));
}

BOOL CInfoIcon::PreTranslateMessage(MSG* pMsg)
{
    if (m_ToolTip.GetSafeHwnd())
        m_ToolTip.RelayEvent(pMsg);
    return CStatic::PreTranslateMessage(pMsg);
}

void CInfoIcon::SetTooltipText(CString sText)
{
    m_sTooltipText = sText;
    if (m_ToolTip.GetSafeHwnd())
        m_ToolTip.UpdateTipText(sText, this);
}

// --- RESIZE FUNCTION ---
// This resizes the actual window control. 
// The OnPaint logic will automatically scale the drawing to match.
void CInfoIcon::SetIconSize(int nSize)
{
    if (GetSafeHwnd())
    {
        SetWindowPos(NULL, 0, 0, nSize, nSize, SWP_NOMOVE | SWP_NOZORDER);
        Invalidate(); // Redraw immediately
    }
}

BOOL CInfoIcon::OnEraseBkgnd(CDC* pDC)
{
    // Handle transparency (prevent flickering)
    CRect rect;
    GetClientRect(rect);
    CWnd* pParent = GetParent();
    if (pParent)
    {
        pParent->ScreenToClient(rect);
        // Trick: Ask parent to draw its background here first
    }
    return TRUE;
}

void CInfoIcon::OnPaint()
{
    CPaintDC dc(this);

    // 1. Setup GDI+
    Graphics graphics(dc.GetSafeHdc());
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
    graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality); // Important for precise centering

    // 2. Get Drawing Area
    CRect rectClient;
    GetClientRect(&rectClient);

    // Force a square shape based on the smaller dimension
    int nSize = min(rectClient.Width(), rectClient.Height());

    // Calculate the drawing square centered in the control
    RectF rectDraw(
        (REAL)(rectClient.Width() - nSize) / 2.0f,
        (REAL)(rectClient.Height() - nSize) / 2.0f,
        (REAL)nSize,
        (REAL)nSize
    );

    // 3. Define Colors (Matches your image)
    // Dark Gray for Text, slightly lighter for Border
    Color colorBorder(255, 105, 105, 105);
    Color colorText(255, 60, 60, 60);

    // 4. Draw Circle
    // Dynamic border thickness (Size / 10 is thicker, matches your image better)
    float borderThick = (float)nSize / 10.0f;
    if (borderThick < 2.0f) borderThick = 2.0f;

    Pen circlePen(colorBorder, borderThick);

    // Deflate rect so border fits inside
    RectF rectCircle = rectDraw;
    float fInset = borderThick / 2.0f;
    rectCircle.Inflate(-fInset, -fInset);

    graphics.DrawEllipse(&circlePen, rectCircle);

    // --- NEW LOGIC FOR PERFECT VERTICAL CENTERING ---

    // 5. Create the "i" as a Vector Path
    GraphicsPath path;
    FontFamily fontFamily(L"Times New Roman");
    int fontStyle = FontStyleBold | FontStyleItalic;

    // Calculate font size (roughly 60% of circle looks best for Times New Roman)
    float emSize = nSize * 0.60f;

    // Add the string "i" to the path at coordinates (0,0)
    path.AddString(L"i", -1, &fontFamily, fontStyle, emSize, PointF(0, 0), NULL);

    // 6. Measure the ACTUAL ink size (ignores invisible font padding)
    RectF bounds;
    path.GetBounds(&bounds, NULL, NULL);

    // 7. Calculate the Offset needed to center the ink
    // Center of Circle = rectCircle.X + rectCircle.Width/2
    // Center of 'i'    = bounds.X + bounds.Width/2

    float circleCenterX = rectDraw.X + rectDraw.Width / 2.0f;
    float circleCenterY = rectDraw.Y + rectDraw.Height / 2.0f;

    float textCenterX = bounds.X + bounds.Width / 2.0f;
    float textCenterY = bounds.Y + bounds.Height / 2.0f;

    float moveX = circleCenterX - textCenterX;
    float moveY = circleCenterY - textCenterY;

    // 8. Move the "i" to the center
    Matrix matrix;
    matrix.Translate(moveX, moveY);
    path.Transform(&matrix);

    // 9. Draw the filled "i"
    SolidBrush textBrush(colorText);
    graphics.FillPath(&textBrush, &path);
}