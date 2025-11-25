#include "pch.h"
#include "IconButton.h"


CIconButton::CIconButton()
{
    m_bTransparent = FALSE;
    m_nFlatStyle = CMFCButton::BUTTONSTYLE_3D;
    m_bTopImage = TRUE;
    m_bRightImage = FALSE;
    m_nAlignStyle = CMFCButton::ALIGN_CENTER;
    m_bDrawFocus = FALSE;


}

CIconButton::~CIconButton()
{
}


BEGIN_MESSAGE_MAP(CIconButton, CMFCButton)
END_MESSAGE_MAP()


void CIconButton::PreSubclassWindow()
{
    // You can add any additional initialization here if needed
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
