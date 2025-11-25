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

    // ... previous declarations ...
};