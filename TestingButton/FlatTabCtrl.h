#pragma once
#include <afxcmn.h>

class CFlatTabCtrl : public CTabCtrl
{
public:
    CFlatTabCtrl();
    virtual ~CFlatTabCtrl();

protected:
    // Parameters for customization
    COLORREF m_colActiveText;
    COLORREF m_colInactiveText;
    COLORREF m_colUnderline;
    COLORREF m_colBackground;
    int m_nUnderlineHeight;

    // Overrides
    virtual void PreSubclassWindow();
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

    DECLARE_MESSAGE_MAP()
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};