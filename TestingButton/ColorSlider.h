#pragma once
#include <afxcmn.h>

class CColorSlider : public CSliderCtrl
{
    DECLARE_DYNAMIC(CColorSlider)

public:
    CColorSlider();
    virtual ~CColorSlider();

    // Sets a solid background color
    void SetBackgroundColor(COLORREF color);

    // Sets the background to transparent
    void SetTransparent(BOOL bTransparent = TRUE);

protected:
    CBrush m_brushBack;
    COLORREF m_colorBack;
    BOOL m_bTransparent;

    // Overrides
    virtual void PreSubclassWindow();

    // Message Map Functions
    DECLARE_MESSAGE_MAP()

    // Reflected handler: The control handles its own color, not the parent
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

    // Mouse handlers to fix drawing artifacts ("smearing") when transparent
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};