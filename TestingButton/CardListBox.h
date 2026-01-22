#pragma once

// -------------------------------------------------------------------------
// Internal Helper: The actual ListBox
// -------------------------------------------------------------------------
class CInnerListBox : public CListBox
{
public:
    virtual ~CInnerListBox() {}
protected:
    virtual void PreSubclassWindow() {
        // 1. Remove the Border Styles explicitly
        ModifyStyle(WS_BORDER | WS_THICKFRAME, 0);
        ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE | WS_EX_DLGMODALFRAME, 0);
        CListBox::PreSubclassWindow();
    }

    // REMOVED: afx_msg void OnNcPaint() {} -> DELETE THIS LINE so Scrollbars appear!

    // Ensure background matches the card (White)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC) {
        CRect rect; GetClientRect(rect);
        pDC->FillSolidRect(rect, RGB(255, 255, 255));
        return TRUE;
    }
    DECLARE_MESSAGE_MAP()
};

// -------------------------------------------------------------------------
// Main Control: CardListBox
// -------------------------------------------------------------------------
class CardListBox : public CStatic
{
    DECLARE_DYNAMIC(CardListBox)

public:
    CardListBox();
    virtual ~CardListBox();

    void SetTitle(CString strTitle);
    int AddString(LPCTSTR lpszItem); // Updated to handle Horizontal Scrolling
    void ResetContent();
    int GetCount();

protected:
    CString m_strTitle;
    CInnerListBox m_wndList;

    virtual void PreSubclassWindow();
    void UpdateLayout();
    void CreateInnerList();
    void RecalcHorizontalScroll(); // New Helper

    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    DECLARE_MESSAGE_MAP()

private:
    void AddRoundedRectToPath(Gdiplus::GraphicsPath& path, Gdiplus::RectF rect, float radius);
};