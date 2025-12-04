#pragma once
#include "afxwin.h"

class CCustomRadioButton : public CButton
{
public:
    CCustomRadioButton();
    virtual ~CCustomRadioButton();

    // Setters for customization
    void SetTransparent(BOOL bTransparent);
    void SetBackgroundColor(COLORREF color);
    void SetTextColor(COLORREF color);

protected:
    // State variables
    BOOL m_bTransparent;
    COLORREF m_colorBack;
    COLORREF m_colorText;
    CBrush m_brushBack;

    // Message Map Functions
    DECLARE_MESSAGE_MAP()
    // This assumes the message is reflected from the parent to the control
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
};