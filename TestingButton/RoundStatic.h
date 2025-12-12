#pragma once
#include <afxwin.h>

class CRoundStatic : public CStatic
{
public:
    CRoundStatic();
    virtual ~CRoundStatic();

    // Setters for customization
    void SetBackColor(COLORREF color);
    void SetTextColor(COLORREF color);
    void SetCornerRadius(int radius);

protected:
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()

private:
    COLORREF m_BackColor;
    COLORREF m_TextColor;
    int m_CornerRadius;
};