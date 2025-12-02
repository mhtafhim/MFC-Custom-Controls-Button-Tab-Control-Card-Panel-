#include "pch.h"
#include "BottomTabCtrl.h"


//C:\Users\FSM-Mahmudul Haque\source\repos\TestingButton\TestingButton\BottomTabCtrl.h

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBottomTabCtrl::CBottomTabCtrl()
{
    m_nTabHeight = 45;         // Modern tabs are usually 40-50px
    m_nHoverIndex = -1;

    // Modern Color Palette
    m_colActiveText = RGB(0, 120, 215);     // Windows Blue
    m_colInactiveText = RGB(80, 80, 80);    // Dark Gray
    m_colHoverText = RGB(0, 80, 180);       // Darker Blue
    m_colUnderline = RGB(0, 120, 215);      // Active Line Color
    m_colBkg = RGB(255, 255, 255);          // Background White
}

CBottomTabCtrl::~CBottomTabCtrl()
{
    if (m_Font.GetSafeHandle())
        m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CBottomTabCtrl, CTabCtrl)
    ON_WM_ERASEBKGND()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CBottomTabCtrl::PreSubclassWindow()
{
    // 1. TCS_OWNERDRAWFIXED: Allows us to draw the buttons
    // 2. TCS_BOTTOM: Moves the tab strip to the bottom of the control
    ModifyStyle(0, TCS_OWNERDRAWFIXED | TCS_BOTTOM);

    // Create a modern "Segoe UI" font
    if (!m_Font.GetSafeHandle())
    {
        m_Font.CreateFont(
            20, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Segoe UI"));
    }

    SetFont(&m_Font);

    // Apply the height
    SetItemSize(CSize(0, m_nTabHeight));

    CTabCtrl::PreSubclassWindow();
}

void CBottomTabCtrl::SetTabHeight(int nHeight)
{
    m_nTabHeight = nHeight;
    SetItemSize(CSize(0, m_nTabHeight));
}

BOOL CBottomTabCtrl::OnEraseBkgnd(CDC* pDC)
{
    // Return TRUE to prevent flickering (we paint background in OnPaint)
    return TRUE;
}

void CBottomTabCtrl::OnPaint()
{
    CPaintDC dc(this);

    // Double buffering to prevent flickering
    CMemDC memDC(dc, this);
    CDC* pDC = &memDC.GetDC();

    CRect rcClient;
    GetClientRect(&rcClient);

    // 1. Fill the entire background with White
    pDC->FillSolidRect(rcClient, m_colBkg);

    // 2. Iterate through tabs and draw them
    int nCount = GetItemCount();
    for (int i = 0; i < nCount; ++i)
    {
        CRect rcItem;
        GetItemRect(i, &rcItem);
        DrawItemManual(pDC, i, rcItem);
    }
}

void CBottomTabCtrl::DrawItemManual(CDC* pDC, int nIndex, CRect rect)
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

    // 4. Draw Text (Centered)
    pDC->DrawText(CString(label), rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // 5. Draw Indicator Line
    // Since tabs are at the BOTTOM, the "Active" line looks best at the TOP of the tab
    // (creating a boundary between the content above and the tab below).
    if (isSelected)
    {
        CRect rectLine = rect;
        rectLine.bottom = rectLine.top + 4; // Draw 4px line from top downwards
        pDC->FillSolidRect(rectLine, m_colUnderline);
    }
    else if (isHovered)
    {
        CRect rectLine = rect;
        rectLine.bottom = rectLine.top + 2; // Draw 2px faint line
        pDC->FillSolidRect(rectLine, RGB(220, 220, 220));
    }

    pDC->SelectObject(pOldFont);
}

void CBottomTabCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    TCHITTESTINFO hitInfo;
    hitInfo.pt = point;
    int nIndex = HitTest(&hitInfo);

    if (nIndex != m_nHoverIndex)
    {
        m_nHoverIndex = nIndex;
        Invalidate(); // Repaint to show hover color change

        // Track mouse so we know when it leaves the control
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = m_hWnd;
        TrackMouseEvent(&tme);
    }

    CTabCtrl::OnMouseMove(nFlags, point);
}

void CBottomTabCtrl::OnMouseLeave()
{
    m_nHoverIndex = -1;
    Invalidate(); // Reset colors
    CTabCtrl::OnMouseLeave();
}





void CBottomTabCtrl::AdjustRect(BOOL bLarger, LPRECT lpRect)
{
    // Let the default logic calculate the space for tabs
    CTabCtrl::AdjustRect(bLarger, lpRect);

    // The default logic adds/removes 3D border padding (usually 2-3px).
    // Since we are doing a flat design, we want to reclaim that space.
    if (!bLarger && lpRect)
    {
        // Expand the rect back out to remove border gap
        // For TCS_BOTTOM:
        // Top, Left, Right usually have borders. Bottom has the tabs.

        lpRect->top -= 2;   // Push top up
        lpRect->left -= 2;  // Push left out
        lpRect->right += 2; // Push right out

        // Do NOT adjust lpRect->bottom aggressively, or the content will
        // cover the tabs sitting at the bottom.
        // Usually, Windows leaves a gap above the bottom tabs for the border,
        // so we can push it down slightly to touch the tab line.
        lpRect->bottom += 1;
    }
}