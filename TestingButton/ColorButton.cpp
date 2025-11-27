#include "pch.h"
#include "ColorButton.h"

// Constructor
CColorButton::CColorButton()
{
    // Default Colors (Gray/Black)
    m_bgColor = RGB(200, 200, 200);
    m_hoverColor = RGB(220, 220, 220);
    m_textColor = RGB(0, 0, 0);

    m_bHovering = false;
    m_bTracking = false;
}

CColorButton::~CColorButton()
{
    m_font.DeleteObject();
}

// Helper to set colors and force redraw
void CColorButton::SetColor(COLORREF bg, COLORREF hover, COLORREF text)
{
    m_bgColor = bg;
    m_hoverColor = hover;
    m_textColor = text;

    if (GetSafeHwnd()) Invalidate();
}

// Helper to create the custom font
void CColorButton::SetTextProperties(int fontSize, bool isBold, CString fontName)
{
    m_font.DeleteObject();

    CClientDC dc(this);

    LOGFONT lf;
    memset(&lf, 0, sizeof(LOGFONT));

    // Calculate height based on DPI
    lf.lfHeight = -MulDiv(fontSize, dc.GetDeviceCaps(LOGPIXELSY), 72);
    lf.lfWeight = isBold ? FW_BOLD : FW_NORMAL;
    _tcscpy_s(lf.lfFaceName, fontName);

    m_font.CreateFontIndirect(&lf);

    if (GetSafeHwnd()) Invalidate();
}

BEGIN_MESSAGE_MAP(CColorButton, CButton)
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

// 1. Detect Mouse Entering
void CColorButton::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bTracking)
    {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.hwndTrack = m_hWnd;
        tme.dwFlags = TME_LEAVE;

        if (_TrackMouseEvent(&tme))
        {
            m_bTracking = true;
            m_bHovering = true;
            Invalidate(); // Redraw for hover effect
        }
    }
    CButton::OnMouseMove(nFlags, point);
}

// 2. Detect Mouse Leaving
void CColorButton::OnMouseLeave()
{
    m_bTracking = false;
    m_bHovering = false;
    Invalidate(); // Redraw to remove hover effect

    CButton::OnMouseLeave();
}

// 3. The Main Drawing Logic
void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC dc;
    dc.Attach(lpDrawItemStruct->hDC);

    CRect rect = lpDrawItemStruct->rcItem;
    UINT state = lpDrawItemStruct->itemState;

    // --- A. Determine Background Color ---
    COLORREF currentBg = m_bgColor;

    if (state & ODS_SELECTED) // Button Pressed
    {
        // Darken the normal color slightly
        currentBg = RGB(GetRValue(m_bgColor) * 0.8,
            GetGValue(m_bgColor) * 0.8,
            GetBValue(m_bgColor) * 0.8);
    }
    else if (m_bHovering) // Mouse Hover
    {
        currentBg = m_hoverColor;
    }

    // --- B. Draw Background (Rounded Rectangle) ---
    CPen pen(PS_SOLID, 1, currentBg);
    CBrush brush(currentBg);

    CBrush* pOldBrush = dc.SelectObject(&brush);
    CPen* pOldPen = dc.SelectObject(&pen);

    // (15,15) is the roundness of the corners
    dc.RoundRect(rect, CPoint(15, 15));

    // --- C. Draw Text ---
    CString strText;
    GetWindowText(strText);

    dc.SetTextColor(m_textColor);
    dc.SetBkMode(TRANSPARENT);

    // Select Custom Font if set
    CFont* pOldFont = NULL;
    if (m_font.GetSafeHandle())
    {
        pOldFont = dc.SelectObject(&m_font);
    }

    // If pressed, move text 1 pixel down/right for "click" feel
    if (state & ODS_SELECTED)
    {
        rect.OffsetRect(1, 1);
    }

    dc.DrawText(strText, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // --- D. Cleanup ---
    if (pOldFont) dc.SelectObject(pOldFont);
    dc.SelectObject(pOldBrush);
    dc.SelectObject(pOldPen);
    dc.Detach();
}