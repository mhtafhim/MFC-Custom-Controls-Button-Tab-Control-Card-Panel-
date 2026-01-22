#pragma once
#include <afxwin.h>
#include <gdiplus.h>

class CCardPanel : public CWnd
{
    DECLARE_DYNAMIC(CCardPanel)

public:
    CCardPanel();
    virtual ~CCardPanel();

    static BOOL RegisterWindowClass();

    // --- NEW: Method to set the title ---
    void SetTitle(CString strTitle);

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg LRESULT OnNcHitTest(CPoint point);

private:
    void AddRoundedRectToPath(Gdiplus::GraphicsPath& path, Gdiplus::RectF rect, float radius);

    // --- NEW: Variable to hold the text ---
    CString m_strTitle;
};