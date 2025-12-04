#include "pch.h"
#include "ColorSliderCtrl.h"

CColorSliderCtrl::CColorSliderCtrl()
{
    m_colBg = RGB(230, 255, 255);
    m_colChannel = RGB(200, 200, 200);
    m_colThumb = RGB(0, 120, 215);
    m_colThumbBorder = RGB(0, 80, 180);
    m_bDragging = FALSE;
}

CColorSliderCtrl::~CColorSliderCtrl()
{
}

BEGIN_MESSAGE_MAP(CColorSliderCtrl, CSliderCtrl)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CColorSliderCtrl::SetColors(COLORREF bg, COLORREF channel, COLORREF thumb, COLORREF thumbBorder)
{
    m_colBg = bg;
    m_colChannel = channel;
    m_colThumb = thumb;
    m_colThumbBorder = thumbBorder;
    Invalidate();
}

BOOL CColorSliderCtrl::OnEraseBkgnd(CDC* pDC)
{
    // Return TRUE to stop Windows from erasing background (prevents flickering)
    return TRUE;
}

// Helper: Calculate where the thumb is currently drawn
CRect CColorSliderCtrl::GetThumbRect()
{
    CRect rectClient;
    GetClientRect(rectClient);

    int minVal, maxVal;
    GetRange(minVal, maxVal);
    int currPos = GetPos();

    if (maxVal <= minVal) maxVal = minVal + 1;

    BOOL bIsVertical = (GetStyle() & TBS_VERT);
    double ratio = (double)(currPos - minVal) / (double)(maxVal - minVal);

    CRect rectThumb;

    if (bIsVertical)
    {
        int centerX = rectClient.Width() / 2;
        int usableHeight = rectClient.Height() - m_nThumbHeight;

        // In Vert sliders, usually Top is Min, Bottom is Max.
        int pixelOffset = (int)(ratio * usableHeight);

        rectThumb.SetRect(
            centerX - m_nThumbWidth / 2,
            rectClient.top + pixelOffset,
            centerX + m_nThumbWidth / 2,
            rectClient.top + pixelOffset + m_nThumbHeight
        );
    }
    else
    {
        int centerY = rectClient.Height() / 2;
        int usableWidth = rectClient.Width() - m_nThumbWidth;
        int pixelOffset = (int)(ratio * usableWidth);

        rectThumb.SetRect(
            rectClient.left + pixelOffset,
            centerY - m_nThumbHeight / 2,
            rectClient.left + pixelOffset + m_nThumbWidth,
            centerY + m_nThumbHeight / 2
        );
    }
    return rectThumb;
}

// Helper: Convert Mouse Point to Slider Value (0-100 etc)
int CColorSliderCtrl::PointToPos(CPoint point)
{
    CRect rectClient;
    GetClientRect(rectClient);

    int minVal, maxVal;
    GetRange(minVal, maxVal);

    BOOL bIsVertical = (GetStyle() & TBS_VERT);
    double ratio = 0.0;

    if (bIsVertical)
    {
        int usableHeight = rectClient.Height() - m_nThumbHeight;
        if (usableHeight <= 0) return minVal;

        // Clamp mouse to the usable area
        int relY = point.y - (m_nThumbHeight / 2);
        if (relY < 0) relY = 0;
        if (relY > usableHeight) relY = usableHeight;

        ratio = (double)relY / (double)usableHeight;
    }
    else
    {
        int usableWidth = rectClient.Width() - m_nThumbWidth;
        if (usableWidth <= 0) return minVal;

        int relX = point.x - (m_nThumbWidth / 2);
        if (relX < 0) relX = 0;
        if (relX > usableWidth) relX = usableWidth;

        ratio = (double)relX / (double)usableWidth;
    }

    return minVal + (int)(ratio * (maxVal - minVal));
}

void CColorSliderCtrl::OnPaint()
{
    CPaintDC dc(this);

    // Double Buffering setup
    CRect rectClient;
    GetClientRect(rectClient);

    CDC memDC;
    memDC.CreateCompatibleDC(&dc);
    CBitmap memBitmap;
    memBitmap.CreateCompatibleBitmap(&dc, rectClient.Width(), rectClient.Height());
    CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

    // 1. Draw Background
    memDC.FillSolidRect(rectClient, m_colBg);

    // 2. Draw Channel
    BOOL bIsVertical = (GetStyle() & TBS_VERT);
    CRect rectChannel;

    if (bIsVertical)
    {
        int centerX = rectClient.Width() / 2;
        rectChannel.SetRect(centerX - m_nChannelThickness / 2, rectClient.top + 5,
            centerX + m_nChannelThickness / 2, rectClient.bottom - 5);
    }
    else
    {
        int centerY = rectClient.Height() / 2;
        rectChannel.SetRect(rectClient.left + 5, centerY - m_nChannelThickness / 2,
            rectClient.right - 5, centerY + m_nChannelThickness / 2);
    }
    memDC.FillSolidRect(rectChannel, m_colChannel);

    // 3. Draw Thumb (Using central logic)
    CRect rectThumb = GetThumbRect();

    CPen penThumb(PS_SOLID, 1, m_colThumbBorder);
    CBrush brushThumb(m_colThumb);
    CPen* pOldPen = memDC.SelectObject(&penThumb);
    CBrush* pOldBrush = memDC.SelectObject(&brushThumb);

    memDC.Rectangle(rectThumb);

    memDC.SelectObject(pOldPen);
    memDC.SelectObject(pOldBrush);

    // 4. Blit to screen
    dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0, 0, SRCCOPY);

    memDC.SelectObject(pOldBitmap);
}

// --- MOUSE HANDLING ---

void CColorSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    CRect rectThumb = GetThumbRect();

    // Only start dragging if user clicks ON the thumb or channel
    // For better UX, let's allow clicking anywhere to jump
    SetCapture();
    m_bDragging = TRUE;

    // Move immediately to click spot
    int newPos = PointToPos(point);
    SetPos(newPos);
    Invalidate(); // Redraw
    UpdateWindow();

    // Notify Parent that we changed value
    CWnd* pParent = GetParent();
    if (pParent)
    {
        if (GetStyle() & TBS_VERT)
            pParent->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, newPos), (LPARAM)m_hWnd);
        else
            pParent->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, newPos), (LPARAM)m_hWnd);
    }
}

void CColorSliderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bDragging)
    {
        int newPos = PointToPos(point);
        int oldPos = GetPos();

        if (newPos != oldPos)
        {
            SetPos(newPos);
            Invalidate();
            UpdateWindow(); // Force immediate redraw for smooth look

            // Notify Parent
            CWnd* pParent = GetParent();
            if (pParent)
            {
                if (GetStyle() & TBS_VERT)
                    pParent->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, newPos), (LPARAM)m_hWnd);
                else
                    pParent->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, newPos), (LPARAM)m_hWnd);
            }
        }
    }
    CSliderCtrl::OnMouseMove(nFlags, point);
}

void CColorSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bDragging)
    {
        m_bDragging = FALSE;
        ReleaseCapture();

        // Send final notification (SB_THUMBPOSITION or SB_ENDSCROLL)
        CWnd* pParent = GetParent();
        if (pParent)
        {
            int pos = GetPos();
            if (GetStyle() & TBS_VERT)
                pParent->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_ENDSCROLL, pos), (LPARAM)m_hWnd);
            else
                pParent->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_ENDSCROLL, pos), (LPARAM)m_hWnd);
        }
    }
    CSliderCtrl::OnLButtonUp(nFlags, point);
}