#pragma once
#include <afxwin.h>

class CStyleStatic : public CStatic
{
public:
    CStyleStatic();
    virtual ~CStyleStatic();

    // --- Public Functions ---

    // Updated: Sets size AND bold status in one call
    void SetFontOptions(int nPointSize, bool bBold);

    // Keep these if you want to change properties individually later
    void SetTextColor(COLORREF color);
    void SetBkColor(COLORREF color);
    void SetTransparent(bool bTransparent);

protected:
    // Attributes
    CFont    m_font;
    COLORREF m_textColor;
    COLORREF m_bkColor;
    CBrush   m_bkBrush;
    bool     m_bTransparent;

    // Font styling state
    bool     m_bBold;
    int      m_nFontSize;

    // Helper to rebuild font 
    void ReconstructFont();

    // MFC Overrides
    virtual void PreSubclassWindow();

    // Message Map
    DECLARE_MESSAGE_MAP()
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
};