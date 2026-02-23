#pragma once
#include <vector>
#include <afxwin.h>

// Include GDI+
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

struct PanelSegment {
    int nID;
    CString strLabel;
    COLORREF color;
};

class CStatusPanelCtrl : public CStatic
{
public:
    CStatusPanelCtrl();
    virtual ~CStatusPanelCtrl();

    // Configuration
    void SetTitle(CString strTitle);
    void SetVertical(BOOL bVertical);
    void SetCornerRadius(int radius);
    void AddSegment(int nID, CString strLabel, COLORREF color);

    // Runtime
    void SetState(int nID);

protected:
    CString m_strTitle;
    std::vector<PanelSegment> m_segments;
    int m_nActiveId;
    BOOL m_bVertical;
    int m_nCornerRadius;

    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    DECLARE_MESSAGE_MAP()

private:
    // Helper to create smooth rounded paths
    void GetRoundedRectPath(GraphicsPath* pPath, RectF rect, int diameter);
};