#pragma once
#include <afxwin.h>
#include <afxbutton.h> // Required for CMFCButton

class CColorButton : public CMFCButton
{
public:
    CColorButton();
    virtual ~CColorButton();

    // Setup colors: Normal Background, Hover Background, Text Color
    void SetColor(COLORREF bg, COLORREF hover, COLORREF text);

    // Setup Font: Size (e.g., 14), Bold (true/false), Font Name (e.g., "Arial")
    void SetTextProperties(int fontSize, bool isBold, CString fontName = _T("Arial"));

protected:
    // Internal variables
    COLORREF m_bgColor;
    COLORREF m_hoverColor;
    COLORREF m_textColor;
    CFont    m_font;

    // CMFCButton specific override for drawing
    virtual void OnDraw(CDC* pDC, const CRect& rect, UINT uiState);

    DECLARE_MESSAGE_MAP()
};