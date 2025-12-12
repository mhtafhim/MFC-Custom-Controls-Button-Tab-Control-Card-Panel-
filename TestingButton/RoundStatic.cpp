#include "pch.h" // or #include "stdafx.h" depending on your VS version
#include "RoundStatic.h"

CRoundStatic::CRoundStatic()
{
    // Default Colors
    m_BackColor = RGB(200, 200, 200); // Light Gray
    m_TextColor = RGB(0, 0, 0);       // Black
    m_CornerRadius = 25;              // Radius of the corners
}

CRoundStatic::~CRoundStatic()
{
}

BEGIN_MESSAGE_MAP(CRoundStatic, CStatic)
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CRoundStatic::SetBackColor(COLORREF color)
{
    m_BackColor = color;
    Invalidate(); // Redraw
}

void CRoundStatic::SetTextColor(COLORREF color)
{
    m_TextColor = color;
    Invalidate();
}

void CRoundStatic::SetCornerRadius(int radius)
{
    m_CornerRadius = radius;
    Invalidate();
}

void CRoundStatic::OnPaint()
{
    CPaintDC dc(this); // Device context for painting

    // Get the area of the control
    CRect rect;
    GetClientRect(&rect);

    // 1. Prepare to draw graphics (Double Buffering to prevent flicker)
    CMemDC memDC(dc, this);
    CDC& pDC = memDC.GetDC();

    // Fill parent background (optional, cleans up corners)
     pDC.FillSolidRect(rect, RGB(230,255,255)); 

    // 2. Create Brush and Pen for the rounded rectangle
    CBrush brush(m_BackColor);
    CPen pen(PS_SOLID, 1, m_BackColor); // Pen same color as brush for no border

    CBrush* pOldBrush = pDC.SelectObject(&brush);
    CPen* pOldPen = pDC.SelectObject(&pen);

    // 3. Draw the Rounded Rectangle
    // The CPoint defines the ellipse width/height used for corners
    pDC.RoundRect(rect, CPoint(m_CornerRadius, m_CornerRadius));

    // 4. Draw the Text
    CString strText;
    GetWindowText(strText);

    if (!strText.IsEmpty())
    {
        pDC.SetBkMode(TRANSPARENT);
        pDC.SetTextColor(m_TextColor);

        // Select the current font of the control
        CFont* pFont = GetFont();
        CFont* pOldFont = NULL;
        if (pFont)
            pOldFont = pDC.SelectObject(pFont);

        // Draw centered text
        pDC.DrawText(strText, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        if (pOldFont)
            pDC.SelectObject(pOldFont);
    }

    // Cleanup
    pDC.SelectObject(pOldBrush);
    pDC.SelectObject(pOldPen);
}