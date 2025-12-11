#include "pch.h"
#include "MyListCtrl.h"


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMyListCtrl::OnCustomDraw)
    ON_WM_SIZE()
    ON_WM_NCPAINT()

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


    // 1. Remove standard square borders (ClientEdge and Border)
    ModifyStyle(WS_BORDER, 0);
    ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 0);

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


void CMyListCtrl::OnSize(UINT nType, int cx, int cy)
{
    CListCtrl::OnSize(nType, cx, cy);

    // Define the corner roundness (e.g., 16 pixels)
    int nCornerRadius = 16;

    CRect rect;
    GetWindowRect(&rect);

    // Create a Rounded Rectangular Region
    // We start at (0,0) and go to (Width, Height)
    CRgn rgn;
    rgn.CreateRoundRectRgn(0, 0, rect.Width() + 1, rect.Height() + 1, nCornerRadius, nCornerRadius);

    // Apply the Region to the Window
    // The "TRUE" flag tells Windows to redraw immediately
    SetWindowRgn(static_cast<HRGN>(rgn.GetSafeHandle()), TRUE);

    // Note: SetWindowRgn takes ownership of the GDI handle, so we detach it 
    // to prevent the C++ object from destroying it.
    rgn.Detach();
}


void CMyListCtrl::OnNcPaint()
{
    // 1. Let the default paint happen (for scrollbars, etc.)
    CListCtrl::OnNcPaint();

    // 2. Draw our custom rounded border on top
    CWindowDC dc(this); // Get DC for the whole window (including borders)

    CRect rect;
    GetWindowRect(&rect);

    // Normalize rect to (0,0)
    rect.OffsetRect(-rect.left, -rect.top);

    // Define border color (e.g., Dark Gray like standard Windows)
    CPen pen(PS_SOLID, 1, RGB(100, 100, 100));
    CPen* pOldPen = dc.SelectObject(&pen);

    // Use a NULL brush so we don't fill the inside, just the outline
    dc.SelectStockObject(NULL_BRUSH);

    // Draw the RoundRect border matching the region in OnSize
    // Note: We subtract 1 from right/bottom to keep it inside the region
    int nCornerRadius = 16;
    dc.RoundRect(rect.left, rect.top, rect.right, rect.bottom, nCornerRadius, nCornerRadius);

    dc.SelectObject(pOldPen);
}

