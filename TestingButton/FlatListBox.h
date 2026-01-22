#pragma once
class CFlatListBox : public CListBox
{
protected:
    virtual void PreSubclassWindow();
    afx_msg void OnNcPaint(); // Crucial to stop Windows drawing borders
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    DECLARE_MESSAGE_MAP()
};