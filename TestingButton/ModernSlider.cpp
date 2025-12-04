#include "pch.h"
#include "ModernSlider.h"

IMPLEMENT_DYNAMIC(CModernSlider, CSliderCtrl)

CModernSlider::CModernSlider()
{
    // Default "Modern" Scheme (Windows 10 Blue-ish)
    m_colPrimary = RGB(0, 120, 215);      // Active Blue
    m_colChannel = RGB(200, 200, 200);    // Light Gray
    m_colBackground = GetSysColor(COLOR_BTNFACE);
    m_colThumbBorder = RGB(0, 100, 180);

    m_bTransparent = TRUE;
    m_bIsHovering = FALSE;
    m_bIsDragging = FALSE;
}

CModernSlider::~CModernSlider()
{
}

BEGIN_MESSAGE_MAP(CModernSlider, CSliderCtrl)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// --- Configuration ---
void CModernSlider::SetPrimaryColor(COLORREF color) { m_colPrimary = color; Invalidate(); }
void CModernSlider::SetChannelColor(COLORREF color) { m_colChannel = color; Invalidate(); }
void CModernSlider::SetBackgroundColor(COLORREF color) { m_colBackground = color; m_bTransparent = FALSE; Invalidate(); }
void CModernSlider::SetTransparent(BOOL bTransparent) { m_bTransparent = bTransparent; Invalidate(); }

// --- Message Handlers ---

BOOL CModernSlider::OnEraseBkgnd(CDC* pDC)
{
    // Prevent flickering by handling background in OnPaint
    return TRUE;
}

void CModernSlider::OnPaint()
{
    CPaintDC dc(this); // Device context for painting

    // Get Geometry
    CRect rectClient;
    GetClientRect(rectClient);

    // Double Buffering setup (MemDC) to prevent flicker
    CDC memDC;
    memDC.CreateCompatibleDC(&dc);
    CBitmap memBitmap;
    memBitmap.CreateCompatibleBitmap(&dc, rectClient.Width(), rectClient.Height());
    CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

    // 1. Draw Background
    if (m_bTransparent)
    {
        // To simulate transparency, we ask the parent to draw its background into our MemDC
        // This makes the slider look like it's floating on the dialog image/color
        if (GetParent())
        {
            CPoint pt(0, 0);
            MapWindowPoints(GetParent(), &pt, 1);
            pt = -pt; // Offset
            memDC.SetViewportOrg(pt);
            GetParent()->SendMessage(WM_PRINTCLIENT, (WPARAM)memDC.GetSafeHdc(), PRF_CLIENT);
            memDC.SetViewportOrg(0, 0); // Reset
        }
    }
    else
    {
        memDC.FillSolidRect(rectClient, m_colBackground);
    }

    // 2. Draw Slider based on orientation
    if ((GetStyle() & TBS_VERT))
        DrawVertical(&memDC, rectClient);
    else
        DrawHorizontal(&memDC, rectClient);

    // Copy buffer to screen
    dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0, 0, SRCCOPY);

    // Cleanup
    memDC.SelectObject(pOldBitmap);
}

void CModernSlider::DrawChannel(CDC* pDC, CRect rect, BOOL bVertical)
{
    CPen penChannel(PS_SOLID, 1, m_colChannel);
    CBrush brushChannel(m_colChannel);

    CBrush* pOldBrush = pDC->SelectObject(&brushChannel);
    CPen* pOldPen = pDC->SelectObject(&penChannel);

    // Draw a rounded rectangle for the track
    pDC->RoundRect(rect, CPoint(rect.Width(), rect.Height())); // Fully rounded caps

    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
}

void CModernSlider::DrawThumb(CDC* pDC, CPoint ptCenter)
{
    int radius = 8; // Thumb size
    if (m_bIsHovering || m_bIsDragging) radius = 9; // Enlarge on hover

    CRect rectThumb(ptCenter.x - radius, ptCenter.y - radius,
        ptCenter.x + radius, ptCenter.y + radius);

    // Outer Circle (Border)
    CPen penBorder(PS_SOLID, 1, m_colThumbBorder);
    CBrush brushFill(m_bIsDragging ? RGB(255, 255, 255) : m_colPrimary);

    // Hover effect: If dragging, white center, colored border. If normal, filled color.
    if (m_bIsDragging) brushFill.DeleteObject(), brushFill.CreateSolidBrush(RGB(255, 255, 255));

    CBrush* pOldBrush = pDC->SelectObject(&brushFill);
    CPen* pOldPen = pDC->SelectObject(&penBorder);

    pDC->Ellipse(rectThumb);

    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
}

void CModernSlider::DrawVertical(CDC* pDC, CRect rectClient)
{
    int min, max;
    GetRange(min, max);
    int pos = GetPos();

    // Prevent divide by zero
    if (max <= min) max = min + 1;

    // Dimensions
    int channelWidth = 4;
    int padding = 12; // Space for thumb at top/bottom

    int centerX = rectClient.Width() / 2;
    int trackTop = rectClient.top + padding;
    int trackBottom = rectClient.bottom - padding;
    int trackHeight = trackBottom - trackTop;

    // 1. Draw Inactive Channel (Gray)
    CRect rectChannel(centerX - channelWidth / 2, trackTop, centerX + channelWidth / 2, trackBottom);
    DrawChannel(pDC, rectChannel, TRUE);

    // 2. Calculate Thumb Position
    // MFC Sliders are usually Min at Top, Max at Bottom, unless inverted.
    // Standard UI usually expects Min at Bottom for volume/levels.
    // Let's stick to standard Windows behavior: Top is Min, Bottom is Max.
    double ratio = (double)(pos - min) / (double)(max - min);
    int pixelY = trackTop + (int)(ratio * trackHeight);

    // 3. Draw Active Channel (Filled Color)
    // From Top to Thumb
    CRect rectActive(centerX - channelWidth / 2, trackTop, centerX + channelWidth / 2, pixelY);

    CBrush brushActive(m_colPrimary);
    pDC->FillRect(rectActive, &brushActive);

    // 4. Draw Thumb
    DrawThumb(pDC, CPoint(centerX, pixelY));
}

void CModernSlider::DrawHorizontal(CDC* pDC, CRect rectClient)
{
    int min, max;
    GetRange(min, max);
    int pos = GetPos();
    if (max <= min) max = min + 1;

    int channelHeight = 4;
    int padding = 12;

    int centerY = rectClient.Height() / 2;
    int trackLeft = rectClient.left + padding;
    int trackRight = rectClient.right - padding;
    int trackWidth = trackRight - trackLeft;

    // 1. Channel
    CRect rectChannel(trackLeft, centerY - channelHeight / 2, trackRight, centerY + channelHeight / 2);
    DrawChannel(pDC, rectChannel, FALSE);

    // 2. Thumb Pos
    double ratio = (double)(pos - min) / (double)(max - min);
    int pixelX = trackLeft + (int)(ratio * trackWidth);

    // 3. Active Fill (Left to Thumb)
    CRect rectActive(trackLeft, centerY - channelHeight / 2, pixelX, centerY + channelHeight / 2);
    CBrush brushActive(m_colPrimary);
    pDC->FillRect(rectActive, &brushActive);

    // 4. Thumb
    DrawThumb(pDC, CPoint(pixelX, centerY));
}

// --- Interaction Logic ---

void CModernSlider::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bIsHovering)
    {
        // Start tracking mouse leave
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = m_hWnd;
        TrackMouseEvent(&tme);

        m_bIsHovering = TRUE;
        Invalidate(); // Redraw for hover effect
    }

    CSliderCtrl::OnMouseMove(nFlags, point);
}

void CModernSlider::OnMouseLeave()
{
    m_bIsHovering = FALSE;
    Invalidate();
    CSliderCtrl::OnMouseLeave();
}

void CModernSlider::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bIsDragging = TRUE;
    Invalidate();
    CSliderCtrl::OnLButtonDown(nFlags, point);
}

void CModernSlider::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_bIsDragging = FALSE;
    Invalidate();
    CSliderCtrl::OnLButtonUp(nFlags, point);
}