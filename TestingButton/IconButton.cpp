#include "pch.h"
#include "IconButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CIconButton::CIconButton()
{
    // Default MFC Button settings
    m_bTransparent = FALSE; // Must be false to draw our custom background
    m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT; // Flat style simplifies drawing logic
    m_bTopImage = TRUE;
    m_bRightImage = FALSE;
    m_nAlignStyle = CMFCButton::ALIGN_CENTER;
    m_bDrawFocus = FALSE; // Disable the dotted focus rectangle

    // Default Colors
    // Standard Button Face color for normal state
    m_clrBack = ::GetSysColor(COLOR_BTNFACE);
    // Light Blue for hover (looks good on most modern UIs)
    m_clrHover = RGB(200, 200, 200);
}

CIconButton::~CIconButton()
{
}

BEGIN_MESSAGE_MAP(CIconButton, CMFCButton)
END_MESSAGE_MAP()

void CIconButton::PreSubclassWindow()
{
    // Ensure the button knows it needs to repaint on mouse events
    ModifyStyle(0, BS_OWNERDRAW);
    CMFCButton::PreSubclassWindow();
}

void CIconButton::SetIconByID(UINT nIconID)
{
    if (nIconID != 0)
    {
        HICON hIcon = AfxGetApp()->LoadIcon(nIconID);
        SetIcon(hIcon);
    }
}

void CIconButton::SetHoverColor(COLORREF clrHover)
{
    m_clrHover = clrHover;
    if (GetSafeHwnd()) Invalidate();
}

void CIconButton::SetBackgroundColor(COLORREF clrBack)
{
    m_clrBack = clrBack;
    if (GetSafeHwnd()) Invalidate();
}

// ---------------------------------------------------------
// Custom Drawing Logic
// ---------------------------------------------------------

void CIconButton::OnDrawBorder(CDC* pDC, CRect& rectClient, UINT uiState)
{
    // Intentionally empty.
    // By overriding this and doing nothing, we remove the border.
}

void CIconButton::OnFillBackground(CDC* pDC, const CRect& rectClient)
{
    // Check if the mouse is hovering (Highlighted) or clicked (Pressed)
    if (IsHighlighted() || IsPressed())
    {
        // Draw the hover color
        pDC->FillSolidRect(rectClient, m_clrHover);
    }
    else
    {
        // Draw the normal background color
        // If you want the button to look transparent (blend with dialog) 
        // until hovered, ensure m_clrBack matches your dialog color 
        // or SetBackgroundColor(GetSysColor(COLOR_BTNFACE));
        pDC->FillSolidRect(rectClient, m_clrBack);
    }
}