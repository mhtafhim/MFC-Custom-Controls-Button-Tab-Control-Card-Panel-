#include "pch.h"
#include "FlatListBox.h"

BEGIN_MESSAGE_MAP(CFlatListBox, CListBox)
    ON_WM_NCPAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CFlatListBox::PreSubclassWindow()
{
    // Remove standard and extended 3D borders
    ModifyStyle(WS_BORDER | WS_THICKFRAME, 0);
    ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE | WS_EX_DLGMODALFRAME, 0);

    // Force redraw
    SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

    CListBox::PreSubclassWindow();
}

void CFlatListBox::OnNcPaint()
{
    // DELIBERATELY EMPTY. 
    // This stops Windows from drawing the non-client area (borders).
}

BOOL CFlatListBox::OnEraseBkgnd(CDC* pDC)
{
    // Ensure background is pure white to blend with the card
    CRect rect;
    GetClientRect(rect);
    pDC->FillSolidRect(rect, RGB(255, 255, 255));
    return TRUE;
}