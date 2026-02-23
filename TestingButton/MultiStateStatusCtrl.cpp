#include "pch.h"
#include "MultiStateStatusCtrl.h"

BEGIN_MESSAGE_MAP(CMultiStateStatusCtrl, CStatic)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CMultiStateStatusCtrl::CMultiStateStatusCtrl()
{
    m_nActiveId = -1; // No active state by default
    m_brushBg.CreateSolidBrush(RGB(50, 50, 50)); // Dark container background
}

CMultiStateStatusCtrl::~CMultiStateStatusCtrl()
{
    m_brushBg.DeleteObject();
    if (m_font.GetSafeHandle()) m_font.DeleteObject();
}

void CMultiStateStatusCtrl::AddSegment(int nID, CString strLabel, COLORREF color)
{
    StateSegment seg;
    seg.nID = nID;
    seg.strLabel = strLabel;
    seg.color = color;
    m_segments.push_back(seg);
}

void CMultiStateStatusCtrl::ResetSegments()
{
    m_segments.clear();
    Invalidate();
}

void CMultiStateStatusCtrl::SetActiveId(int nID)
{
    if (m_nActiveId != nID)
    {
        m_nActiveId = nID;
        Invalidate();   // Mark for repaint
        UpdateWindow(); // Force immediate update
    }
}

COLORREF CMultiStateStatusCtrl::GetDimmedColor(COLORREF color)
{
    // Reduce brightness by 80% to create the "off" look
    return RGB(GetRValue(color) / 3, GetGValue(color) / 3, GetBValue(color) / 3);
}

BOOL CMultiStateStatusCtrl::OnEraseBkgnd(CDC* pDC)
{
    return TRUE; // Prevent flickering
}

void CMultiStateStatusCtrl::OnPaint()
{
    CPaintDC dc(this);

    // Double Buffering Setup
    CRect rectClient;
    GetClientRect(&rectClient);

    CDC memDC;
    memDC.CreateCompatibleDC(&dc);
    CBitmap memBitmap;
    memBitmap.CreateCompatibleBitmap(&dc, rectClient.Width(), rectClient.Height());
    CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

    // 1. Draw Background
    memDC.FillRect(rectClient, &m_brushBg);

    // 2. Setup Font (Bold Arial)
    if (!m_font.GetSafeHandle()) {
        m_font.CreatePointFont(90, _T("Arial Bold"));
    }
    CFont* pOldFont = memDC.SelectObject(&m_font);

    // 3. Draw Segments
    size_t count = m_segments.size();
    if (count > 0)
    {
        int totalGapWidth = (int)(count + 1) * 4; // 4px gap
        int availableWidth = rectClient.Width() - totalGapWidth;
        int segmentWidth = availableWidth / (int)count;
        int gap = 4;

        CRect rcSegment = rectClient;
        rcSegment.left += gap;
        rcSegment.right = rcSegment.left + segmentWidth;
        rcSegment.top += gap;
        rcSegment.bottom -= gap;

        for (size_t i = 0; i < count; i++)
        {
            StateSegment& seg = m_segments[i];
            bool isActive = (seg.nID == m_nActiveId);

            // Determine Colors
            COLORREF fillColor = isActive ? seg.color : GetDimmedColor(seg.color);
            COLORREF textColor = isActive ? RGB(0, 0, 0) : RGB(100, 100, 100);

            // Fill Segment
            memDC.FillSolidRect(rcSegment, fillColor);

            // Draw Glow Border if active
            if (isActive)
            {
                // Draw a 2px white border
                CPen penBorder(PS_SOLID, 2, RGB(255, 255, 255));
                CPen* pOldPen = memDC.SelectObject(&penBorder);
                memDC.SelectStockObject(NULL_BRUSH);
                memDC.Rectangle(rcSegment);
                memDC.SelectObject(pOldPen);
            }

            // Draw Text
            memDC.SetBkMode(TRANSPARENT);
            memDC.SetTextColor(textColor);
            memDC.DrawText(seg.strLabel, rcSegment, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            // Move Rect to next position
            rcSegment.OffsetRect(segmentWidth + gap, 0);

            // Adjust last segment to fit perfectly to the edge minus gap
            if (i == count - 2) {
                rcSegment.right = rectClient.right - gap;
            }
        }
    }

    // Blit to screen
    dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0, 0, SRCCOPY);

    // Cleanup
    memDC.SelectObject(pOldBitmap);
    memDC.SelectObject(pOldFont);
}