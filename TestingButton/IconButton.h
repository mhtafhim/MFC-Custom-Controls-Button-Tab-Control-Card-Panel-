#pragma once
#include <afxbutton.h>

class CIconButton : public CMFCButton
{
public:
    CIconButton();
    virtual ~CIconButton();

    // Setup the icon from resource ID
    void SetIconByID(UINT nIconID);

    // Optional: Functions to customize colors programmatically
    void SetHoverColor(COLORREF clrHover);
    void SetBackgroundColor(COLORREF clrBack);

protected:
    virtual void PreSubclassWindow();

    // Overrides to handle custom drawing
    virtual void OnDrawBorder(CDC* pDC, CRect& rectClient, UINT uiState);
    virtual void OnFillBackground(CDC* pDC, const CRect& rectClient);

    // Internal color variables
    COLORREF m_clrHover;
    COLORREF m_clrBack;

    DECLARE_MESSAGE_MAP()
};