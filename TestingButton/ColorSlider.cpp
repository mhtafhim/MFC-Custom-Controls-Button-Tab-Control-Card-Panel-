#include "pch.h"
#include "ColorSlider.h"

IMPLEMENT_DYNAMIC(CColorSlider, CSliderCtrl)

CColorSlider::CColorSlider()
{
    m_bTransparent = FALSE;
    m_colorBack = RGB(255,255,255); // Default gray
    m_brushBack.CreateSolidBrush(m_colorBack);
}

CColorSlider::~CColorSlider()
{
    m_brushBack.DeleteObject();
}

BEGIN_MESSAGE_MAP(CColorSlider, CSliderCtrl)
    // ON_WM_CTLCOLOR_REFLECT handles the color message within the class itself
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CColorSlider::PreSubclassWindow()
{
    CSliderCtrl::PreSubclassWindow();

    // CRITICAL: Disable Windows Themes for this specific control.
    // Modern Windows themes (Visual Styles) ignore background brushes for Sliders.
    // Reverting to the "Classic" non-themed style allows the color to fill the whole rect.
    SetWindowTheme(m_hWnd, L"", L"");
}

void CColorSlider::SetBackgroundColor(COLORREF color)
{
    m_bTransparent = FALSE;
    m_colorBack = color;

    // Recreate the brush
    m_brushBack.DeleteObject();
    m_brushBack.CreateSolidBrush(m_colorBack);

    // Trigger redraw
    Invalidate();
}

void CColorSlider::SetTransparent(BOOL bTransparent)
{
    m_bTransparent = bTransparent;
    Invalidate();
}

HBRUSH CColorSlider::CtlColor(CDC* pDC, UINT nCtlColor)
{
    // Set text background (ticks) to transparent so they don't look blocky
    pDC->SetBkMode(TRANSPARENT);

    if (m_bTransparent)
    {
        // Return a hollow brush for transparency
        return (HBRUSH)GetStockObject(NULL_BRUSH);
    }

    // Return our custom solid color brush
    return m_brushBack;
}

// Fix for Transparency Smearing:
// When dragging a transparent slider, the old thumb position isn't automatically 
// erased by the background. We force a redraw during mouse interaction.
void CColorSlider::OnMouseMove(UINT nFlags, CPoint point)
{
    CSliderCtrl::OnMouseMove(nFlags, point);

    // If we are dragging (Left button down) and it is transparent/colored
    if ((nFlags & MK_LBUTTON))
    {
        Invalidate();
        UpdateWindow();
    }
}

void CColorSlider::OnLButtonUp(UINT nFlags, CPoint point)
{
    CSliderCtrl::OnLButtonUp(nFlags, point);
    // Final cleanup redraw
    Invalidate();
}

BOOL CColorSlider::OnEraseBkgnd(CDC* pDC)
{
    // If transparent, let the parent window draw the background 
    // before we draw the slider on top.
    if (m_bTransparent)
    {
        return TRUE;
    }

    return CSliderCtrl::OnEraseBkgnd(pDC);
}