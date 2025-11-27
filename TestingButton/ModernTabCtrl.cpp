#include "pch.h"
#include "ModernTabCtrl.h"

CModernTabCtrl::CModernTabCtrl()
{
    m_nTabHeight = 50;         // Make it tall (Modern standard is ~45-50px)
    m_nHoverIndex = -1;

    // Modern Color Palette
    m_colActiveText = RGB(0, 120, 215);     // Windows Blue
    m_colInactiveText = RGB(80, 80, 80);    // Dark Gray (Sharp contrast)
    m_colHoverText = RGB(0, 80, 180);       // Slightly darker blue on hover
    m_colUnderline = RGB(0, 120, 215);
    m_colBkg = RGB(255, 255, 255);
}

CModernTabCtrl::~CModernTabCtrl()
{
    if (m_Font.GetSafeHandle())
        m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CModernTabCtrl, CTabCtrl)
    ON_WM_ERASEBKGND()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_PAINT()           // <--- Critical change
END_MESSAGE_MAP()

void CModernTabCtrl::PreSubclassWindow()
{
    // Enable Owner Draw
    ModifyStyle(0, TCS_OWNERDRAWFIXED);

   // ModifyStyle(WS_BORDER, 0);
   // ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 0);


    // Create a modern "Segoe UI" font, size 120 (12pt), Semibold
    // lfHeight calculation: -MulDiv(PointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72)
    // Roughly -16 to -18 for nice large text
    if (!m_Font.GetSafeHandle())
    {
        m_Font.CreateFont(
            22, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Segoe UI"));
    }

    SetFont(&m_Font);

    // Apply the height
    SetItemSize(CSize(0, m_nTabHeight));

    CTabCtrl::PreSubclassWindow();
}

void CModernTabCtrl::SetTabHeight(int nHeight)
{
    m_nTabHeight = nHeight;
    SetItemSize(CSize(0, m_nTabHeight));
}

BOOL CModernTabCtrl::OnEraseBkgnd(CDC* pDC)
{
    CRect rect;
    GetClientRect(rect);
    pDC->FillSolidRect(rect, m_colBkg);
    return TRUE;
}

void CModernTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rect = lpDrawItemStruct->rcItem;
    int nTabIndex = lpDrawItemStruct->itemID;
    bool isSelected = (nTabIndex == GetCurSel());
    bool isHovered = (nTabIndex == m_nHoverIndex);

    // 1. Get Text
    wchar_t label[64];
    TCITEM tci;
    tci.mask = TCIF_TEXT;
    tci.pszText = label;
    tci.cchTextMax = sizeof(label);
    GetItem(nTabIndex, &tci);

    // 2. Select Colors
    COLORREF textColor = m_colInactiveText;
    if (isSelected) textColor = m_colActiveText;
    else if (isHovered) textColor = m_colHoverText;

    // 3. Setup Font and DC
    CFont* pOldFont = pDC->SelectObject(&m_Font);
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(textColor);

    // 4. Draw Background (Clean White)
    pDC->FillSolidRect(rect, m_colBkg);

    // 5. Draw Text (Centered)
    pDC->DrawText(CString(label), rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // 6. Draw Underline
    // Active: Thick steady line
    // Hover: Thin faint line (optional, adds polish)
    if (isSelected)
    {
        CRect rectLine = rect;
        rectLine.top = rectLine.bottom - 4; // 4px thick line
        pDC->FillSolidRect(rectLine, m_colUnderline);
    }
    else if (isHovered)
    {
        // Optional: Draw a lighter grey background or thin line for hover
        CRect rectLine = rect;
        rectLine.top = rectLine.bottom - 2;
        pDC->FillSolidRect(rectLine, RGB(220, 220, 220));
    }

    pDC->SelectObject(pOldFont);
}

void CModernTabCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    TCHITTESTINFO hitInfo;
    hitInfo.pt = point;
    int nIndex = HitTest(&hitInfo);

    if (nIndex != m_nHoverIndex)
    {
        m_nHoverIndex = nIndex;
        Invalidate(); // Repaint to show hover color

        // Track mouse to know when it leaves the control
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = m_hWnd;
        TrackMouseEvent(&tme);
    }

    CTabCtrl::OnMouseMove(nFlags, point);
}

void CModernTabCtrl::OnMouseLeave()
{
    m_nHoverIndex = -1;
    Invalidate(); // Reset colors
    CTabCtrl::OnMouseLeave();
}


void CModernTabCtrl::AdjustRect(BOOL bLarger, LPRECT lpRect)
{
    CTabCtrl::AdjustRect(bLarger, lpRect);

    // If we are calculating the client area (bLarger == FALSE), 
    // expand it to remove the standard 3D border padding.
    if (!bLarger && lpRect)
    {
        lpRect->top -= 2;
        lpRect->left -= 2;
        lpRect->right += 2;
        lpRect->bottom += 2;
    }
}


void CModernTabCtrl::OnPaint()
{
    CPaintDC dc(this); // Device context for painting

    // Use double buffering to prevent flickering
    // (If you are on an older MFC version without CMemDC, use standard CMemDC implementation)
    CMemDC memDC(dc, this);
    CDC* pDC = &memDC.GetDC();

    CRect rcClient;
    GetClientRect(&rcClient);

    // 1. Fill the entire background with White
    // This effectively "erases" the classic 3D borders
    pDC->FillSolidRect(rcClient, m_colBkg);

    // 2. Draw a subtle line at the bottom of the header area (Optional)
    // This separates the tabs from the content area below
    CRect rcHeader = rcClient;
    rcHeader.bottom = m_nTabHeight;
    // pDC->FillSolidRect(0, m_nTabHeight - 1, rcClient.Width(), 1, RGB(230, 230, 230)); 

    // 3. Iterate through tabs and draw them
    int nCount = GetItemCount();
    for (int i = 0; i < nCount; ++i)
    {
        CRect rcItem;
        GetItemRect(i, &rcItem);
        DrawItemManual(pDC, i, rcItem);
    }
}



// This function contains your original logic, adapted for the manual paint loop
void CModernTabCtrl::DrawItemManual(CDC* pDC, int nIndex, CRect rect)
{
    bool isSelected = (nIndex == GetCurSel());
    bool isHovered = (nIndex == m_nHoverIndex);

    // 1. Get Text
    wchar_t label[64];
    TCITEM tci;
    tci.mask = TCIF_TEXT;
    tci.pszText = label;
    tci.cchTextMax = sizeof(label);
    GetItem(nIndex, &tci);

    // 2. Select Colors
    COLORREF textColor = m_colInactiveText;
    if (isSelected) textColor = m_colActiveText;
    else if (isHovered) textColor = m_colHoverText;

    // 3. Setup Font and DC
    CFont* pOldFont = pDC->SelectObject(&m_Font);
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(textColor);

    // 4. Draw Background
    // We already filled the whole control white in OnPaint, 
    // but we can fill the specific tab rect if we wanted a different hover bg color.
    // pDC->FillSolidRect(rect, m_colBkg); 

    // 5. Draw Text (Centered)
    pDC->DrawText(CString(label), rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // 6. Draw Underline
    if (isSelected)
    {
        CRect rectLine = rect;
        rectLine.top = rectLine.bottom - 4; // 4px thick line
        pDC->FillSolidRect(rectLine, m_colUnderline);
    }
    else if (isHovered)
    {
        CRect rectLine = rect;
        rectLine.top = rectLine.bottom - 2; // 2px hover line
        pDC->FillSolidRect(rectLine, RGB(220, 220, 220));
    }

    pDC->SelectObject(pOldFont);
}
