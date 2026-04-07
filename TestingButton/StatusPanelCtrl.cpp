#include "pch.h"
#include "StatusPanelCtrl.h"

BEGIN_MESSAGE_MAP(CStatusPanelCtrl, CStatic)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CStatusPanelCtrl::CStatusPanelCtrl()
{
    m_nActiveId = -1;
    m_bVertical = TRUE;
    m_nCornerRadius = 15;
    m_strTitle = _T("PROCESS STATE");
}

CStatusPanelCtrl::~CStatusPanelCtrl()
{
}

void CStatusPanelCtrl::SetTitle(CString strTitle)
{
    m_strTitle = strTitle;
    Invalidate();
}

void CStatusPanelCtrl::SetVertical(BOOL bVertical)
{
    m_bVertical = bVertical;
    Invalidate();
}

void CStatusPanelCtrl::SetCornerRadius(int radius)
{
    m_nCornerRadius = radius;
    Invalidate();
}

void CStatusPanelCtrl::AddSegment(int nID, CString strLabel, COLORREF color)
{
    PanelSegment seg;
    seg.nID = nID;
    seg.strLabel = strLabel;
    seg.color = color;
    m_segments.push_back(seg);
}

void CStatusPanelCtrl::SetState(int nID)
{
    if (m_nActiveId != nID)
    {
        m_nActiveId = nID;
        Invalidate();
        UpdateWindow();
    }
}

BOOL CStatusPanelCtrl::OnEraseBkgnd(CDC* pDC)
{
    return TRUE; // Prevent flicker
}

// Helper to manually construct a rounded rectangle path for GDI+
void CStatusPanelCtrl::GetRoundedRectPath(GraphicsPath* pPath, RectF rect, int diameter)
{
    float arcSize = (float)diameter;
    // Top Left
    pPath->AddArc(rect.X, rect.Y, arcSize, arcSize, 180, 90);
    // Top Right
    pPath->AddArc(rect.X + rect.Width - arcSize, rect.Y, arcSize, arcSize, 270, 90);
    // Bottom Right
    pPath->AddArc(rect.X + rect.Width - arcSize, rect.Y + rect.Height - arcSize, arcSize, arcSize, 0, 90);
    // Bottom Left
    pPath->AddArc(rect.X, rect.Y + rect.Height - arcSize, arcSize, arcSize, 90, 90);
    pPath->CloseFigure();
}


void CStatusPanelCtrl::OnPaint()
{
    CPaintDC dc(this);
    CRect rectClient;
    GetClientRect(&rectClient);

    // 1. Double Buffering
    CMemDC memDC(dc, rectClient);
    CDC& bufferDC = memDC.GetDC();

    // Fill background with Parent's color
    bufferDC.FillSolidRect(rectClient, RGB(225, 232, 237));

    // 2. Initialize GDI+ Graphics
    Graphics graphics(bufferDC.GetSafeHdc());
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);

    // 3. Define the Main Panel Rectangle
    RectF rectPanel((float)rectClient.left, (float)rectClient.top, (float)rectClient.Width() - 1, (float)rectClient.Height() - 1);
    rectPanel.Inflate(-1, -1);

    // Create Rounded Path for the Border
    GraphicsPath pathPanel;
    GetRoundedRectPath(&pathPanel, rectPanel, m_nCornerRadius);

    // Draw Panel Border (Thin Grey/Black)
    Pen penBorder(Color(255, 150, 150, 150), 1.0f);
    graphics.DrawPath(&penBorder, &pathPanel);

    // 4. Draw Title and Separator Line
    float titleHeight = 0;
    if (!m_strTitle.IsEmpty())
    {
        titleHeight = 22.0f; // Adjusted height slightly for better spacing

        FontFamily fontFamily(L"Segoe UI");
        Gdiplus::Font fontTitle(&fontFamily, 10, FontStyleRegular, UnitPoint);
        SolidBrush brushText(Color(255, 0, 0, 0));

        StringFormat format;
        format.SetAlignment(StringAlignmentCenter);
        format.SetLineAlignment(StringAlignmentCenter);

        RectF rectTitle = rectPanel;
        rectTitle.Height = titleHeight;

        graphics.DrawString(m_strTitle, -1, &fontTitle, rectTitle, &format, &brushText);

        // Draw Line under Title
        float lineY = rectPanel.Y + titleHeight;
        Pen penLine(Color(255, 0, 0, 0), 1.0f);

        graphics.SetClip(&pathPanel);
        graphics.DrawLine(&penLine, rectPanel.X, lineY, rectPanel.X + rectPanel.Width, lineY);
        graphics.ResetClip();
    }

    // 5. Draw Segments
    if (!m_segments.empty())
    {
        // Calculate Content Area
        RectF rcContent = rectPanel;
        rcContent.Y += titleHeight;
        rcContent.Height -= titleHeight;

        // Add padding so segments don't touch the border
        float padding = 6.0f;
        rcContent.Inflate(-padding, -padding);

        size_t count = m_segments.size();
        float gap = 4.0f; // Slightly larger gap for rounded look
        float segW, segH;

        if (m_bVertical) {
            float totalGap = (count - 1) * gap;
            segW = rcContent.Width;
            segH = (rcContent.Height - totalGap) / count;
        }
        else {
            float totalGap = (count - 1) * gap;
            segW = (rcContent.Width - totalGap) / count;
            segH = rcContent.Height;
        }

        RectF rcSeg = rcContent;
        rcSeg.Width = segW;
        rcSeg.Height = segH;

        FontFamily fontFamilySeg(L"Arial");
        Gdiplus::Font fontSeg(&fontFamilySeg, 9, FontStyleBold, UnitPoint);
        StringFormat format;
        format.SetAlignment(StringAlignmentCenter);
        format.SetLineAlignment(StringAlignmentCenter);

        // [NEW] Calculate Inner Radius to match Outer Radius visually
        // Geometrically, Inner Radius = Outer Radius - Padding
        int segRadius = m_nCornerRadius - (int)padding;
        if (segRadius < 2) segRadius = 2; // Prevent invalid radius

        for (size_t i = 0; i < count; i++)
        {
            PanelSegment& seg = m_segments[i];
            bool isActive = (seg.nID == m_nActiveId);

            // [NEW] Create Path for the Segment
            GraphicsPath segPath;
            GetRoundedRectPath(&segPath, rcSeg, segRadius);

            Color segColor;
            segColor.SetFromCOLORREF(seg.color);

            if (!isActive) {
                // Dim the color
                segColor = Color(255, segColor.GetR() / 2.5, segColor.GetG() / 2.5, segColor.GetB() / 2.5);
            }

            // Fill the Rounded Path
            SolidBrush brushSeg(segColor);
            graphics.FillPath(&brushSeg, &segPath);

            // Draw Border/Highlight
            if (isActive) {
                // White highlight border
                Pen penHigh(Color(255, 255, 255, 255), 2.0f);
                graphics.DrawPath(&penHigh, &segPath);
            }
            else {
                // Subtle border for inactive items
                Pen penDim(Color(100, 0, 0, 0), 1.0f);
                graphics.DrawPath(&penDim, &segPath);
            }

            // Draw Text
            SolidBrush brushText(isActive ? Color(255, 0, 0, 0) : Color(255, 100, 100, 100));
            graphics.DrawString(seg.strLabel, -1, &fontSeg, rcSeg, &format, &brushText);

            if (m_bVertical) rcSeg.Offset(0, segH + gap);
            else rcSeg.Offset(segW + gap, 0);
        }
    }
}