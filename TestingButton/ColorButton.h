#pragma once
#include <afxwin.h>

class CColorButton : public CButton
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

    // State tracking
    bool m_bHovering;
    bool m_bTracking;

    // Overrides
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

    // Message Handlers
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();

    DECLARE_MESSAGE_MAP()
};