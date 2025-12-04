#include "pch.h" // Or "stdafx.h" depending on your VS version
#include "CustomRadioButton.h"

CCustomRadioButton::CCustomRadioButton()
{
    // Defaults: Transparent = TRUE, Text = Black
    m_bTransparent = FALSE;
    m_colorText = RGB(0, 0, 0);
    m_colorBack = RGB(230, 255, 255);

    // Create the initial brush
    m_brushBack.CreateSolidBrush(m_colorBack);

   
}

CCustomRadioButton::~CCustomRadioButton()
{
    m_brushBack.DeleteObject();
}

BEGIN_MESSAGE_MAP(CCustomRadioButton, CButton)
    // The magic happens here: =WM_CTLCOLOR handles the reflected message
    ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

HBRUSH CCustomRadioButton::CtlColor(CDC* pDC, UINT nCtlColor)
{
    // 1. Set the Text Color
    pDC->SetTextColor(m_colorText);

    // 2. Set Background Mode
    // TRANSPARENT ensures the text itself doesn't have a rectangular background color
    pDC->SetBkMode(TRANSPARENT);

    // 3. Return the Background Brush
    if (m_bTransparent)
    {
        // Return a hollow brush so the parent background shows through
        return (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else
    {
        // Return our custom solid color brush
        pDC->SetBkColor(m_colorBack);
        return (HBRUSH)m_brushBack;
    }
}

void CCustomRadioButton::SetTransparent(BOOL bTransparent)
{
    m_bTransparent = bTransparent;
    Invalidate(); // Redraw immediately
}

void CCustomRadioButton::SetBackgroundColor(COLORREF color)
{
    m_colorBack = color;
    m_bTransparent = FALSE; // Disable transparency if setting a specific color

    // Recreate the brush
    m_brushBack.DeleteObject();
    m_brushBack.CreateSolidBrush(m_colorBack);

    Invalidate();
}

void CCustomRadioButton::SetTextColor(COLORREF color)
{
    m_colorText = color;
    Invalidate();
}