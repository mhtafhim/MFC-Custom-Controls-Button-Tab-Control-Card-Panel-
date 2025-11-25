#pragma once
#include <afxwin.h>
#include <gdiplus.h>

class CCardPanel : public CWnd
{
    DECLARE_DYNAMIC(CCardPanel)

public:
    CCardPanel();
    virtual ~CCardPanel();

    // CALL THIS IN InitInstance()!
    static BOOL RegisterWindowClass();

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    afx_msg LRESULT OnNcHitTest(CPoint point);


private:
    // Helper for rounded corners
    void AddRoundedRectToPath(Gdiplus::GraphicsPath& path, Gdiplus::RectF rect, float radius);
};