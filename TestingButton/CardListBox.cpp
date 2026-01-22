#include "pch.h"
#include "CardListBox.h"

#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;

// -------------------------------------------------------------------------
// CInnerListBox Implementation
// -------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CInnerListBox, CListBox)
    // REMOVED: ON_WM_NCPAINT() -> This allows the scrollbar to draw!
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// -------------------------------------------------------------------------
// CardListBox Implementation
// -------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CardListBox, CStatic)

CardListBox::CardListBox()
{
    m_strTitle = _T("Default Title");
}

CardListBox::~CardListBox()
{
}

BEGIN_MESSAGE_MAP(CardListBox, CStatic)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
END_MESSAGE_MAP()

void CardListBox::PreSubclassWindow()
{
    CStatic::PreSubclassWindow();
    ModifyStyle(0, SS_NOTIFY | SS_OWNERDRAW);
    CreateInnerList();
}

void CardListBox::CreateInnerList()
{
    if (m_wndList.GetSafeHwnd() == NULL)
    {
        CRect rectDummy(0, 0, 100, 100);

        // ADDED: WS_HSCROLL to allow horizontal scrolling for long lines
        DWORD dwStyle = WS_CHILD | WS_VISIBLE | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_HSCROLL;

        m_wndList.Create(dwStyle, rectDummy, this, 1001);

        // Set Font
        static CFont fontList;
        if (fontList.GetSafeHandle() == NULL) {
            fontList.CreatePointFont(90, _T("Segoe UI"));
        }
        m_wndList.SetFont(&fontList);

        UpdateLayout();
    }
}

void CardListBox::OnSize(UINT nType, int cx, int cy)
{
    CStatic::OnSize(nType, cx, cy);
    UpdateLayout();
}

void CardListBox::UpdateLayout()
{
    if (m_wndList.GetSafeHwnd() == NULL) return;

    CRect rectClient;
    GetClientRect(rectClient);
    if (rectClient.IsRectEmpty()) return;

    int outerMargin = 10;
    int headerHeight = 45;
    int sidePadding = 15;

    int x = outerMargin + sidePadding;
    int y = outerMargin + headerHeight;
    int w = rectClient.Width() - (outerMargin * 2) - (sidePadding * 2);
    int h = rectClient.Height() - y - outerMargin - 5;

    if (w > 0 && h > 0)
    {
        m_wndList.MoveWindow(x, y, w, h);
    }
}

// --- Helper to Auto-Calculate Horizontal Scroll Width ---
void CardListBox::RecalcHorizontalScroll()
{
    if (!m_wndList.GetSafeHwnd()) return;

    CDC* pDC = m_wndList.GetDC();
    CFont* pOldFont = pDC->SelectObject(m_wndList.GetFont());

    int maxWidth = 0;
    int count = m_wndList.GetCount();

    // Check every string in the list to find the widest one
    for (int i = 0; i < count; i++)
    {
        CString str;
        m_wndList.GetText(i, str);
        CSize sz = pDC->GetTextExtent(str);
        if (sz.cx > maxWidth) maxWidth = sz.cx;
    }

    pDC->SelectObject(pOldFont);
    m_wndList.ReleaseDC(pDC);

    // Add a little padding to the right
    m_wndList.SetHorizontalExtent(maxWidth + 20);
}

int CardListBox::AddString(LPCTSTR lpszItem)
{
    if (m_wndList.GetSafeHwnd() == NULL) CreateInnerList();

    int idx = m_wndList.AddString(lpszItem);

    // Recalculate horizontal scroll width whenever we add a string
    RecalcHorizontalScroll();

    return idx;
}

void CardListBox::ResetContent()
{
    if (m_wndList.GetSafeHwnd()) m_wndList.ResetContent();
}

int CardListBox::GetCount()
{
    if (m_wndList.GetSafeHwnd()) return m_wndList.GetCount();
    return 0;
}

void CardListBox::SetTitle(CString strTitle)
{
    m_strTitle = strTitle;
    if (GetSafeHwnd()) RedrawWindow();
}

BOOL CardListBox::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CardListBox::OnPaint()
{
    CPaintDC dc(this);
    CRect rectClient;
    GetClientRect(&rectClient);
    CMemDC memDC(dc, this);
    CDC* pDC = &memDC.GetDC();

    // 1. Background
    pDC->FillSolidRect(rectClient, RGB(244, 247, 249));

    Graphics graphics(pDC->GetSafeHdc());
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);

    // 2. White Card Area
    float margin = 10.0f;
    RectF rectWhite(
        (REAL)rectClient.left + margin, (REAL)rectClient.top + margin,
        (REAL)rectClient.Width() - (margin * 2), (REAL)rectClient.Height() - (margin * 2));
    float radius = 8.0f;

    // 3. Shadow
    RectF rShadow = rectWhite;
    rShadow.Offset(0, 2.0f);
    GraphicsPath pathShadow;
    AddRoundedRectToPath(pathShadow, rShadow, radius);
    SolidBrush bShadow(Color(20, 0, 0, 0));
    graphics.FillPath(&bShadow, &pathShadow);

    // 4. White Box & Border
    GraphicsPath path;
    AddRoundedRectToPath(path, rectWhite, radius);
    SolidBrush brushWhite(Color(255, 255, 255, 255));
    graphics.FillPath(&brushWhite, &path);
    Pen penBorder(Color(255, 220, 220, 220), 1.0f);
    graphics.DrawPath(&penBorder, &path);

    // 5. Title Text
    if (!m_strTitle.IsEmpty())
    {
        Gdiplus::FontFamily fontFamily(L"Segoe UI");
        Gdiplus::Font font(&fontFamily, 14, FontStyleBold, UnitPixel);
        SolidBrush textBrush(Color(255, 44, 62, 80));

        RectF textRect = rectWhite;
        textRect.Inflate(-15, -12);

        StringFormat format;
        format.SetAlignment(StringAlignmentNear);
        format.SetLineAlignment(StringAlignmentNear);

        graphics.DrawString(m_strTitle, -1, &font, textRect, &format, &textBrush);

        // 6. Separator Line
        float lineY = rectWhite.Y + 40.0f;
        Pen penLine(Color(255, 230, 230, 230), 1.0f);
        graphics.DrawLine(&penLine, rectWhite.X, lineY, rectWhite.GetRight(), lineY);
    }
}

void CardListBox::AddRoundedRectToPath(GraphicsPath& path, RectF rect, float radius)
{
    float d = radius * 2.0f;
    path.AddArc(rect.X, rect.Y, d, d, 180, 90);
    path.AddArc(rect.X + rect.Width - d, rect.Y, d, d, 270, 90);
    path.AddArc(rect.X + rect.Width - d, rect.Y + rect.Height - d, d, d, 0, 90);
    path.AddArc(rect.X, rect.Y + rect.Height - d, d, d, 90, 90);
    path.CloseFigure();
}