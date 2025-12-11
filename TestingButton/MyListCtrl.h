#pragma once
#include "MyHeaderCtrl.h" // Include your header class

class CMyListCtrl : public CListCtrl
{
    CMyHeaderCtrl m_HeaderCtrl; // The custom header instance

protected:
    virtual void PreSubclassWindow();

public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);


    // ADD THESE TWO:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnNcPaint();


    // ... previous declarations ...
};