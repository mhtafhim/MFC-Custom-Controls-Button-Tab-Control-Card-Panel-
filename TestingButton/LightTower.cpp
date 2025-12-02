#include "pch.h"
#include "LightTower.h"

CLightTower::CLightTower()
{
    m_CurrentMode = TOWER_OFF;
}

CLightTower::~CLightTower()
{
}

BEGIN_MESSAGE_MAP(CLightTower, CStatic)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CLightTower::SetMode(TowerMode mode)
{
    if (m_CurrentMode != mode)
    {
        m_CurrentMode = mode;
        Invalidate(); // Redraw immediately
    }
}

BOOL CLightTower::OnEraseBkgnd(CDC* pDC)
{
    return TRUE; // Reduce flicker
}

void CLightTower::OnPaint()
{
    CPaintDC dc(this);

    // Get Dimensions
    CRect rectClient;
    GetClientRect(&rectClient);

    // Double Buffering Setup
    CDC memDC;
    memDC.CreateCompatibleDC(&dc);
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(&dc, rectClient.Width(), rectClient.Height());
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    // --- 1. Define Colors ---
    COLORREF cRed = RGB(255, 0, 0);
    COLORREF cAmber = RGB(255, 191, 0);
    COLORREF cGreen = RGB(0, 200, 0);
    COLORREF cBlue = RGB(0, 0, 255);

    // --- 2. Calculate Height for 4 Segments ---
    int segmentHeight = rectClient.Height() / 4;

    // --- 3. Draw Segments Stacked Vertically ---

    // Segment 1 (Top): RED
    CRect r1 = rectClient;
    r1.bottom = r1.top + segmentHeight;
    DrawSegment(&memDC, r1, cRed, (m_CurrentMode == TOWER_RED));

    // Segment 2: AMBER
    CRect r2 = rectClient;
    r2.top = r1.bottom;
    r2.bottom = r2.top + segmentHeight;
    DrawSegment(&memDC, r2, cAmber, (m_CurrentMode == TOWER_AMBER));

    // Segment 3: GREEN
    CRect r3 = rectClient;
    r3.top = r2.bottom;
    r3.bottom = r3.top + segmentHeight;
    DrawSegment(&memDC, r3, cGreen, (m_CurrentMode == TOWER_GREEN));

    // Segment 4 (Bottom): BLUE
    CRect r4 = rectClient;
    r4.top = r3.bottom;
    r4.bottom = rectClient.bottom; // Ensure it fills the remaining space
    DrawSegment(&memDC, r4, cBlue, (m_CurrentMode == TOWER_BLUE));

    // --- 4. Draw Outer Border ---
    memDC.FrameRect(&rectClient, &CBrush(RGB(50, 50, 50)));

    // Copy to Screen
    dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0, 0, SRCCOPY);
    memDC.SelectObject(pOldBitmap);
}

void CLightTower::DrawSegment(CDC* pDC, CRect rect, COLORREF baseColor, bool bOn)
{
    COLORREF finalColor;

    if (bOn)
    {
        // Full Plain Color (Active)
        finalColor = baseColor;
    }
    else
    {
        // Dimmed/Washed out Color (Inactive)
        // We mix the base color with a lot of white/grey to make it look "Off"
        BYTE r = GetRValue(baseColor);
        BYTE g = GetGValue(baseColor);
        BYTE b = GetBValue(baseColor);
        finalColor = RGB((r + 600) / 5.5, (g + 600) / 5.5, (b + 600) / 5.5);
    }

    // Fill Solid Rectangle (No Gradient)
    pDC->FillSolidRect(rect, finalColor);

    // Draw Divider Line (Dark Gray)
    pDC->FrameRect(rect, &CBrush(RGB(80, 80, 80)));
}