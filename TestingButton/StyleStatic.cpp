#include "pch.h"
#include "StyleStatic.h"

CStyleStatic::CStyleStatic()
{
    m_textColor = RGB(0, 0, 0);       // Black Text
    m_bkColor = GetSysColor(COLOR_3DFACE);
    m_bkBrush.CreateSolidBrush(m_bkColor);

    m_bTransparent = true;
    m_bBold = true;  // Default to bold
    m_nFontSize = 0;     // Default to original size
}

CStyleStatic::~CStyleStatic()
{
    m_bkBrush.DeleteObject();
    m_font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CStyleStatic, CStatic)
    ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

void CStyleStatic::PreSubclassWindow()
{
    CStatic::PreSubclassWindow();
    ReconstructFont();
}

void CStyleStatic::ReconstructFont()
{
    CFont* pCurrentFont = GetFont();
    if (!pCurrentFont && GetParent())
        pCurrentFont = GetParent()->GetFont();

    if (!pCurrentFont) return;

    LOGFONT lf;
    pCurrentFont->GetLogFont(&lf);

    // Apply BOLD based on member variable
    lf.lfWeight = m_bBold ? FW_BOLD : FW_NORMAL;

    // Apply SIZE based on member variable
    if (m_nFontSize > 0)
    {
        HDC hDC = ::GetDC(NULL);
        lf.lfHeight = -MulDiv(m_nFontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
        ::ReleaseDC(NULL, hDC);
    }

    if (m_font.GetSafeHandle())
        m_font.DeleteObject();

    m_font.CreateFontIndirect(&lf);
    SetFont(&m_font);
}

// --- The New Combined Function ---
void CStyleStatic::SetFontOptions(int nPointSize, bool bBold)
{
    m_nFontSize = nPointSize;
    m_bBold = bBold;

    ReconstructFont();
    Invalidate(); // Redraw immediately
}

void CStyleStatic::SetTextColor(COLORREF color)
{
    m_textColor = color;
    Invalidate();
}

void CStyleStatic::SetBkColor(COLORREF color)
{
    m_bkColor = color;
    m_bkBrush.DeleteObject();
    m_bkBrush.CreateSolidBrush(m_bkColor);
    m_bTransparent = false;
    Invalidate();
}

void CStyleStatic::SetTransparent(bool bTransparent)
{
    m_bTransparent = bTransparent;
    Invalidate();
}



HBRUSH CStyleStatic::CtlColor(CDC* pDC, UINT nCtlColor)
{
    pDC->SetTextColor(m_textColor);

    if (m_bTransparent)
    {
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else
    {
        pDC->SetBkMode(OPAQUE);
        pDC->SetBkColor(m_bkColor);
        return (HBRUSH)m_bkBrush.GetSafeHandle();
    }
}