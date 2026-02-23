#pragma once
#include <afxwin.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;

class CInfoIcon : public CStatic
{
public:
    CInfoIcon();
    virtual ~CInfoIcon();

    // 1. Set the popup text
    void SetTooltipText(CString sText);

    // 2. Resize the WHOLE logo (Circle + Text) programmatically
    //    Example: SetIconSize(32) makes it 32x32 pixels.
    void SetIconSize(int nSize);

protected:
    CToolTipCtrl m_ToolTip;
    CString m_sTooltipText;

    // GDI+ Setup
    static ULONG_PTR m_gdiplusToken;
    static int m_gdiplusRefCount;

    virtual void PreSubclassWindow();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    DECLARE_MESSAGE_MAP()
};