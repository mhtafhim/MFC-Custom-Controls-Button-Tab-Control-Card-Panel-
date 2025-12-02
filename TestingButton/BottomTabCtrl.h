#pragma once
#include <afxcmn.h>
#include <afxwin.h>



class CBottomTabCtrl : public CTabCtrl
{
public:
    CBottomTabCtrl();
    virtual ~CBottomTabCtrl();

    // Configuration methods
    void SetTabHeight(int nHeight);

protected:
    CFont m_Font;              // Custom large font
    int m_nTabHeight;          // Height of the tabs
    int m_nHoverIndex;         // Which tab is being hovered over?

    // Colors
    COLORREF m_colActiveText;
    COLORREF m_colInactiveText;
    COLORREF m_colHoverText;
    COLORREF m_colUnderline;
    COLORREF m_colBkg;

    virtual void PreSubclassWindow();

    // Core drawing logic
    void DrawItemManual(CDC* pDC, int nIndex, CRect rect);

    // Overrides
    virtual void AdjustRect(BOOL bLarger, LPRECT lpRect);

    DECLARE_MESSAGE_MAP()
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg void OnPaint();
};