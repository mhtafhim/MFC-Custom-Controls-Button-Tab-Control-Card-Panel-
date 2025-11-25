#include "pch.h"
#include "FlatTabCtrl.h"

CFlatTabCtrl::CFlatTabCtrl()
{
    // Match the colors from your image
    m_colActiveText = RGB(0, 120, 215);   // Windows Blue
    m_colInactiveText = RGB(100, 100, 100); // Gray
    m_colUnderline = RGB(0, 120, 215);    // Blue Bar
    m_colBackground = RGB(255, 255, 255); // White background
    m_nUnderlineHeight = 3;               // Thickness of the bar
}

CFlatTabCtrl::~CFlatTabCtrl()
{
}

BEGIN_MESSAGE_MAP(CFlatTabCtrl, CTabCtrl)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CFlatTabCtrl::PreSubclassWindow()
{
    // Force Owner Draw Fixed mode so we can draw it ourselves
    ModifyStyle(0, TCS_OWNERDRAWFIXED);
    CTabCtrl::PreSubclassWindow();
}

BOOL CFlatTabCtrl::OnEraseBkgnd(CDC* pDC)
{
    // Prevent flickering by handling background erasure
    CRect rect;
    GetClientRect(rect);
    pDC->FillSolidRect(rect, m_colBackground);
    return TRUE;
}

void CFlatTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rect = lpDrawItemStruct->rcItem;
    int nTabIndex = lpDrawItemStruct->itemID;

    // 1. Determine if this tab is currently selected
    bool isSelected = (nTabIndex == GetCurSel());

    // 2. Prepare text buffer
    wchar_t label[64];
    TCITEM tci;
    tci.mask = TCIF_TEXT;
    tci.pszText = label;
    tci.cchTextMax = sizeof(label);
    GetItem(nTabIndex, &tci);

    // 3. Set Colors based on state
    COLORREF textColor = isSelected ? m_colActiveText : m_colInactiveText;

    // 4. Set Font (Use the control's current font)
    CFont* pFont = GetFont();
    CFont* pOldFont = pDC->SelectObject(pFont);

    // 5. Draw the Background of the specific tab item
    pDC->FillSolidRect(rect, m_colBackground);

    // 6. Draw the Text
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(textColor);

    // Center text vertically and horizontally
    pDC->DrawText(CString(label), rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // 7. Draw the Underline (Only if selected)
    if (isSelected)
    {
        CRect rectLine = rect;
        rectLine.top = rectLine.bottom - m_nUnderlineHeight; // Move to bottom
        // Optional: Add padding so the line isn't the full width of the click area
        // rectLine.DeflateRect(5, 0); 
        pDC->FillSolidRect(rectLine, m_colUnderline);
    }

    // Cleanup
    pDC->SelectObject(pOldFont);
}