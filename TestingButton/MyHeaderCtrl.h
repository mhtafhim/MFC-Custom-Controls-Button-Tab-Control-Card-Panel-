#pragma once
class CMyHeaderCtrl : public CHeaderCtrl
{
public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
};