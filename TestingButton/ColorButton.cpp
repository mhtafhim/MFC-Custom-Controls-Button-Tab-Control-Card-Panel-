#include "pch.h"
#include "ColorButton.h"

// Constructor
CColorButton::CColorButton()
{
    // Default Colors (Gray/Black)
    m_bgColor = RGB(200, 200, 200);
    m_hoverColor = RGB(220, 220, 220);
    m_textColor = RGB(0, 0, 0);

    // CMFCButton Settings
    m_bTransparent = FALSE;      // We want to draw the background
    m_bDrawFocus = FALSE;        // Optional: Hide the dotted focus rectangle
    m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS; // We draw our own borders
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

    // We need a temporary DC to calculate font height
    CClientDC dc(this);

    LOGFONT lf;
    memset(&lf, 0, sizeof(LOGFONT));

    // Calculate height based on DPI
    lf.lfHeight = -MulDiv(fontSize, dc.GetDeviceCaps(LOGPIXELSY), 72);
    lf.lfWeight = isBold ? FW_BOLD : FW_NORMAL;
    _tcscpy_s(lf.lfFaceName, fontName);

    m_font.CreateFontIndirect(&lf);

    // Update the button font
    SetFont(&m_font);

    if (GetSafeHwnd()) Invalidate();
}

BEGIN_MESSAGE_MAP(CColorButton, CMFCButton)
    // No manual mouse handlers needed; CMFCButton handles tracking internally
END_MESSAGE_MAP()



// The Main Drawing Logic
// CMFCButton uses OnDraw instead of DrawItem for easier customization
void CColorButton::OnDraw(CDC* pDC, const CRect& rect, UINT uiState)
{
    // Copy rect to modifiable variable
    CRect drawRect = rect;

    // --- A. Determine Background Color ---
    COLORREF currentBg = m_bgColor;

    // CMFCButton provides easy state checkers
    if (IsPressed())
    {
        // Darken the normal color slightly
        currentBg = RGB(GetRValue(m_bgColor) * 0.8,
            GetGValue(m_bgColor) * 0.8,
            GetBValue(m_bgColor) * 0.8);
    }
    else if (IsHighlighted()) // Replaces m_bHovering
    {
        currentBg = m_hoverColor;
    }

    // --- B. Draw Background (Rounded Rectangle) ---
    CPen pen(PS_SOLID, 1, currentBg);
    CBrush brush(currentBg);

    CBrush* pOldBrush = pDC->SelectObject(&brush);
    CPen* pOldPen = pDC->SelectObject(&pen);

    // (15,15) is the roundness of the corners
    pDC->RoundRect(drawRect, CPoint(15, 15));

    // --- C. Draw Text ---
    CString strText;
    GetWindowText(strText);

    pDC->SetTextColor(m_textColor);
    pDC->SetBkMode(TRANSPARENT);

    // Select Custom Font if set
    CFont* pOldFont = NULL;
    if (m_font.GetSafeHandle())
    {
        pOldFont = pDC->SelectObject(&m_font);
    }
    else
    {
        // Fallback to default button font if custom one isn't set
        pOldFont = pDC->SelectObject(GetFont());
    }

    // If pressed, move text 1 pixel down/right for "click" feel
    if (IsPressed())
    {
        drawRect.OffsetRect(1, 1);
    }

    pDC->DrawText(strText, drawRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // --- D. Cleanup ---
    if (pOldFont) pDC->SelectObject(pOldFont);
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);

    // Note: Do not call DC.Detach() here, CMFCButton passes a pointer to an existing DC
}