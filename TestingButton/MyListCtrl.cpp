#include "pch.h"
#include "MyListCtrl.h"
#include <uxtheme.h>            // Required for SetWindowTheme
#pragma comment(lib, "uxtheme.lib") // Link the library automatically

BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMyListCtrl::OnCustomDraw)
    //ON_WM_SIZE()
    //ON_WM_NCPAINT()
END_MESSAGE_MAP()

void CMyListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    // --- PART A: Initialize the Header (The Fix) ---
    // We check this every time. If m_headerCtrl is not yet attached, we attach it now.
    if (m_headerCtrl.GetSafeHwnd() == NULL)
    {
        CHeaderCtrl* pHeader = GetHeaderCtrl();
        if (pHeader && pHeader->GetSafeHwnd())
        {
            // 1. Disable Windows Themes for the header 
            // (This fixes the "White Background" issue)
            ::SetWindowTheme(pHeader->GetSafeHwnd(), L"", L"");

            // 2. Subclass the header to our CMyHeaderCtrl
            m_headerCtrl.SubclassWindow(pHeader->GetSafeHwnd());

            // 3. Force a redraw of the header immediately
            m_headerCtrl.Invalidate();
        }
    }

    // --- PART B: Draw the List Rows (Your existing Zebra code) ---
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
    *pResult = CDRF_DODEFAULT;

    switch (pLVCD->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
        *pResult = CDRF_NOTIFYITEMDRAW;
        break;

    case CDDS_ITEMPREPAINT:
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
        break;

    case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
    {
        int nRow = (int)pLVCD->nmcd.dwItemSpec;

        // Zebra Striping logic
        if (nRow % 2 == 0) pLVCD->clrTextBk = RGB(200, 230, 247);
        else pLVCD->clrTextBk = RGB(215, 236, 247);

        *pResult = CDRF_NEWFONT;
    }
    break;
    }
}


void CMyListCtrl::PreSubclassWindow()
{
    CListCtrl::PreSubclassWindow();

    // 1. Remove standard square borders (ClientEdge and Border)
    ModifyStyle(WS_BORDER, 0);
    ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 0);

    SetExtendedStyle(GetExtendedStyle() | LVS_EX_DOUBLEBUFFER);
    ModifyStyle(0, WS_CLIPCHILDREN);
}

// Add this function manually if you haven't yet
BOOL CMyListCtrl::OnEraseBkgnd(CDC* pDC)
{
    // Return TRUE. This stops the white "flash" before the blue rows are drawn.
    // Since we are using Double Buffering (below), we don't need default erasing.
    return TRUE;
}


BOOL CMyListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    NMHDR* pNMHDR = (NMHDR*)lParam;

    // Check if the notification is coming from the Header Control
    if (pNMHDR)
    {
        // HDN_TRACK indicates the user is currently dragging/resizing a column
        // We handle both ANSI (A) and Unicode (W) versions to be safe
        if (pNMHDR->code == HDN_TRACKW || pNMHDR->code == HDN_TRACKA ||
            pNMHDR->code == HDN_ITEMCHANGINGW || pNMHDR->code == HDN_ITEMCHANGINGA)
        {
            // FORCE A FULL REPAINT
            // "FALSE" means: "Don't just repaint the data, erase the background too."
            // This prevents the 'smearing' artifact completely.
            Invalidate(FALSE);

            // Force the window to update immediately, not later
            UpdateWindow();
        }
    }

    return CListCtrl::OnNotify(wParam, lParam, pResult);
}


//void CMyListCtrl::OnSize(UINT nType, int cx, int cy)
//{
//    CListCtrl::OnSize(nType, cx, cy);
//
//    // Define the corner roundness (e.g., 16 pixels)
//    int nCornerRadius = 16;
//
//    CRect rect;
//    GetWindowRect(&rect);
//
//    // Create a Rounded Rectangular Region
//    // We start at (0,0) and go to (Width, Height)
//    CRgn rgn;
//    rgn.CreateRoundRectRgn(0, 0, rect.Width() + 1, rect.Height() + 1, nCornerRadius, nCornerRadius);
//
//    // Apply the Region to the Window
//    // The "TRUE" flag tells Windows to redraw immediately
//    SetWindowRgn(static_cast<HRGN>(rgn.GetSafeHandle()), TRUE);
//
//    // Note: SetWindowRgn takes ownership of the GDI handle, so we detach it 
//    // to prevent the C++ object from destroying it.
//    rgn.Detach();
//}



//void CMyListCtrl::OnNcPaint()
//{
//    // 1. Let the default paint happen (for scrollbars, etc.)
//    CListCtrl::OnNcPaint();
//
//    // 2. Draw our custom rounded border on top
//    CWindowDC dc(this); // Get DC for the whole window (including borders)
//
//    CRect rect;
//    GetWindowRect(&rect);
//
//    // Normalize rect to (0,0)
//    rect.OffsetRect(-rect.left, -rect.top);
//
//    // Define border color (e.g., Dark Gray like standard Windows)
//    CPen pen(PS_SOLID, 1, RGB(100, 100, 100));
//    CPen* pOldPen = dc.SelectObject(&pen);
//
//    // Use a NULL brush so we don't fill the inside, just the outline
//    dc.SelectStockObject(NULL_BRUSH);
//
//    // Draw the RoundRect border matching the region in OnSize
//    // Note: We subtract 1 from right/bottom to keep it inside the region
//    int nCornerRadius = 16;
//    dc.RoundRect(rect.left, rect.top, rect.right, rect.bottom, nCornerRadius, nCornerRadius);
//
//    dc.SelectObject(pOldPen);
//}

