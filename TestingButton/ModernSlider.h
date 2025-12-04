#pragma once
#include <afxwin.h>
#include <afxcmn.h>

class CModernSlider : public CSliderCtrl
{
    DECLARE_DYNAMIC(CModernSlider)

public:
    CModernSlider();
    virtual ~CModernSlider();

    // Configuration
    void SetPrimaryColor(COLORREF color); // Color of the filled part and thumb
    void SetBackgroundColor(COLORREF color); // Color of the dialog background
    void SetChannelColor(COLORREF color); // Color of the empty track
    void SetTransparent(BOOL bTransparent = TRUE);

protected:
    // Colors
    COLORREF m_colPrimary;
    COLORREF m_colChannel;
    COLORREF m_colBackground;
    COLORREF m_colThumbBorder;

    BOOL m_bTransparent;
    BOOL m_bIsHovering;
    BOOL m_bIsDragging;

    // Drawing Helpers
    void DrawHorizontal(CDC* pDC, CRect rectClient);
    void DrawVertical(CDC* pDC, CRect rectClient);
    void DrawChannel(CDC* pDC, CRect rect, BOOL bVertical);
    void DrawThumb(CDC* pDC, CPoint ptCenter);

    // Overrides
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};