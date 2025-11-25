#include "pch.h"
#include "MyListCtrl.h"


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMyListCtrl::OnCustomDraw)
END_MESSAGE_MAP()

void CMyListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

    *pResult = CDRF_DODEFAULT;

    switch (pLVCD->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
        // Tell Windows we want to receive notification for every item
        *pResult = CDRF_NOTIFYITEMDRAW;
        break;

    case CDDS_ITEMPREPAINT:
        // Tell Windows we want to receive notification for every sub-item (cell)
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
        break;

    case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
    {
        int nRow = (int)pLVCD->nmcd.dwItemSpec;
        int nCol = pLVCD->iSubItem;

        // --- LOGIC TO CHANGE COLORS ---

        // Example 1: Zebra Striping (Alternate Row Colors)
        if (nRow % 2 == 0) {
            pLVCD->clrTextBk = RGB(200, 230, 247); // Light Gray Background  200, 230, 247
        }
        else {
            pLVCD->clrTextBk = RGB(162, 214, 245); // White Background  162, 214, 245
        }

        // Example 2: Change Specific Column Style (e.g., Column 1 is Red Text)
        if (nCol == 1) {
            pLVCD->clrText = RGB(0, 0, 0);
        }
        else {
            pLVCD->clrText = RGB(0, 0, 0);
        }

        // Example 3: Change Font Style (Bold)
        // Note: You need to manage the CFont object lifetime elsewhere (e.g., member variable)
        // SelectObject(pLVCD->nmcd.hdc, m_boldFont.GetSafeHandle());

        *pResult = CDRF_NEWFONT; // Tell Windows we changed the font/colors
    }
    break;
    }

}




void CMyListCtrl::PreSubclassWindow()
{
    CListCtrl::PreSubclassWindow();

    // Get the existing header
    CHeaderCtrl* pHeader = GetHeaderCtrl();

    if (pHeader)
    {
        // Subclass it with our custom class
        m_HeaderCtrl.SubclassWindow(pHeader->GetSafeHwnd());

        // IMPORTANT: Disable Windows Themes for the header 
        // so our custom colors actually show up.
       // ::SetWindowTheme(m_HeaderCtrl.GetSafeHwnd(), L"", L"");
    }
}