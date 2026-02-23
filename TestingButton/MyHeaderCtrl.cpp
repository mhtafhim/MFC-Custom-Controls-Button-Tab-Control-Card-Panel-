#include "pch.h"
#include "MyHeaderCtrl.h"

BEGIN_MESSAGE_MAP(CMyHeaderCtrl, CHeaderCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMyHeaderCtrl::OnCustomDraw)
    // Add this to prevent flickering (Optional but recommended)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// Add this function manually to MyHeaderCtrl.cpp if not present
// You also need to add afx_msg BOOL OnEraseBkgnd(CDC* pDC); in .h
BOOL CMyHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
    // Return TRUE to tell Windows "I handled the background, don't auto-erase it"
    // This reduces flickering during resize.
    return TRUE;
}

void CMyHeaderCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMCUSTOMDRAW pCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

    *pResult = CDRF_DODEFAULT;

    if (pCD->dwDrawStage == CDDS_PREPAINT)
    {
        // --- FIX FOR BROKEN ARTIFACTS ---
        // Paint the ENTIRE header background area here.
        // This covers the empty space to the right of the columns.
        CDC* pDC = CDC::FromHandle(pCD->hdc);
        CRect rectClient;
        GetClientRect(&rectClient);

        // Fill the whole header strip with your background color
        pDC->FillSolidRect(&rectClient, RGB(255, 255, 255)); // Same color as your columns

        *pResult = CDRF_NOTIFYITEMDRAW;
    }
    else if (pCD->dwDrawStage == CDDS_ITEMPREPAINT)
    {
        CDC* pDC = CDC::FromHandle(pCD->hdc);
        CRect rect = pCD->rc;
        int nCol = (int)pCD->dwItemSpec;

        // 1. Draw Column Background
        // We redraw the background for the specific item to ensure clean edges
        pDC->FillSolidRect(&rect, RGB(162, 214, 245));

        // 2. Draw Text
        HDITEM hdItem;
        TCHAR szBuffer[256] = _T("");
        hdItem.mask = HDI_TEXT | HDI_FORMAT;
        hdItem.pszText = szBuffer;
        hdItem.cchTextMax = 255;
        GetItem(nCol, &hdItem);

        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(RGB(0, 0, 0));

        UINT uFormat = DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX;
        if (hdItem.fmt & HDF_CENTER) uFormat |= DT_CENTER;
        else if (hdItem.fmt & HDF_RIGHT) uFormat |= DT_RIGHT;
        else uFormat |= DT_LEFT;

        // Add padding
        if (uFormat & DT_LEFT) rect.left += 60;
        else if (uFormat & DT_RIGHT) rect.right -= 6;

        pDC->DrawText(szBuffer, &rect, uFormat);

        // 3. Draw Separator Line (Optional, makes it look like a real table)
        CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
        CPen* pOld = pDC->SelectObject(&pen);
        pDC->MoveTo(rect.right - 1, rect.top);
        pDC->LineTo(rect.right - 1, rect.bottom);
        pDC->SelectObject(pOld);

        *pResult = CDRF_SKIPDEFAULT;
    }
}