#include "pch.h"
#include "MyHeaderCtrl.h"



BEGIN_MESSAGE_MAP(CMyHeaderCtrl, CHeaderCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMyHeaderCtrl::OnCustomDraw)
END_MESSAGE_MAP()



void CMyHeaderCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMCUSTOMDRAW pCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

    switch (pCD->dwDrawStage)
    {
    case CDDS_PREPAINT:
        // Tell Windows we want to handle notification for each item
        *pResult = CDRF_NOTIFYITEMDRAW;
        break;

    case CDDS_ITEMPREPAINT:
    {
        CDC* pDC = CDC::FromHandle(pCD->hdc);
        CRect rect = pCD->rc;
        int nItem = (int)pCD->dwItemSpec;

        // 1. Define Colors
        COLORREF clrBackground = RGB(0, 135, 212); // Blue background   0, 135, 212
        COLORREF clrText = RGB(0, 0, 0);     // black text 

        // 2. Fill Background
        pDC->FillSolidRect(&rect, clrBackground);

        // 3. Get Item Text and Format
        HDITEM hdi;
        wchar_t buffer[256] = { 0 };
        hdi.mask = HDI_TEXT | HDI_FORMAT;
        hdi.pszText = buffer;
        hdi.cchTextMax = 256;
        GetItem(nItem, &hdi);

        UINT uFormat = DT_VCENTER | DT_SINGLELINE;
        if (hdi.fmt & HDF_CENTER) uFormat |= DT_CENTER;
        else if (hdi.fmt & HDF_RIGHT) uFormat |= DT_RIGHT;
        else uFormat |= DT_LEFT;

        // Add some padding
        rect.DeflateRect(4, 0);

        // 4. Draw Text
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(clrText);
        pDC->DrawText(hdi.pszText, &rect, uFormat);

        // 5. Tell Windows we drew it ourselves, don't do default drawing
        *pResult = CDRF_SKIPDEFAULT;
    }
    break;

    default:
        *pResult = CDRF_DODEFAULT;
        break;
    }
}

