#pragma once
#include <afxwin.h>

class CColorSliderCtrl : public CSliderCtrl
{
public:
    CColorSliderCtrl();
    virtual ~CColorSliderCtrl();

    // Customization
    void SetColors(COLORREF bg, COLORREF channel, COLORREF thumb, COLORREF thumbBorder);

protected:
    // Colors
    COLORREF m_colBg;
    COLORREF m_colChannel;
    COLORREF m_colThumb;
    COLORREF m_colThumbBorder;

    // Dimensions
    const int m_nThumbWidth = 24;      // Width of the rectangle
    const int m_nThumbHeight = 12;     // Height of the rectangle
    const int m_nChannelThickness = 4; // Thickness of the line

    // State
    BOOL m_bDragging;

    // Helpers
    CRect GetThumbRect();
    int PointToPos(CPoint point);

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    // Mouse Interaction Handlers
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};