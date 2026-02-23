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

    // 1. Double Buffering (Standard MFC way to prepare buffer)
    CMemDC memDC(dc, rectClient);
    CDC& bufferDC = memDC.GetDC();

    // Fill background with Parent's color so corners blend perfectly
    bufferDC.FillSolidRect(rectClient, GetSysColor(COLOR_3DFACE));

    // 2. Initialize GDI+ Graphics
    Graphics graphics(bufferDC.GetSafeHdc());

    // ** CRITICAL: This makes the corners smooth **
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);

    // 3. Define the Main Panel Rectangle
    RectF rectPanel((float)rectClient.left, (float)rectClient.top, (float)rectClient.Width() - 1, (float)rectClient.Height() - 1);
    rectPanel.Inflate(-1, -1); // Slight padding from edge

    // Create Rounded Path
    GraphicsPath pathPanel;
    GetRoundedRectPath(&pathPanel, rectPanel, m_nCornerRadius);

    // Draw Panel Background (Dark Grey)
    SolidBrush brushBg(Color(255, 45, 45, 48));
    graphics.FillPath(&brushBg, &pathPanel);

    // Draw Panel Border
    Pen penBorder(Color(255, 100, 100, 100), 1.0f);
    graphics.DrawPath(&penBorder, &pathPanel);

    // 4. Draw Title
    float titleHeight = 0;
    if (!m_strTitle.IsEmpty())
    {
        titleHeight = 30.0f;

        FontFamily fontFamily(L"Segoe UI");
        Gdiplus::Font fontTitle(&fontFamily, 10, FontStyleRegular, UnitPoint);
        SolidBrush brushText(Color(255, 200, 200, 200)); // Light Grey

        StringFormat format;
        format.SetAlignment(StringAlignmentCenter);
        format.SetLineAlignment(StringAlignmentCenter);

        RectF rectTitle = rectPanel;
        rectTitle.Height = titleHeight; // Restrict to top area

        graphics.DrawString(m_strTitle, -1, &fontTitle, rectTitle, &format, &brushText);
    }

    // 5. Draw Segments
    if (!m_segments.empty())
    {
        // Calculate Content Area
        RectF rcContent = rectPanel;
        rcContent.Y += titleHeight;
        rcContent.Height -= titleHeight;
        rcContent.Inflate(-5, -5); // Padding inside the wrapper

        size_t count = m_segments.size();
        float gap = 3.0f;
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

        // Segment Font
        FontFamily fontFamilySeg(L"Arial");
        Gdiplus::Font fontSeg(&fontFamilySeg, 10, FontStyleBold, UnitPoint);
        StringFormat format;
        format.SetAlignment(StringAlignmentCenter);
        format.SetLineAlignment(StringAlignmentCenter);

        for (size_t i = 0; i < count; i++)
        {
            PanelSegment& seg = m_segments[i];
            bool isActive = (seg.nID == m_nActiveId);

            // Convert COLORREF to GDI+ Color
            Color segColor;
            segColor.SetFromCOLORREF(seg.color);

            // Calculate Dimmed Color if inactive
            if (!isActive) {
                segColor = Color(255, segColor.GetR() / 5, segColor.GetG() / 5, segColor.GetB() / 5);
            }

            SolidBrush brushSeg(segColor);

            // Draw Segment Box
            graphics.FillRectangle(&brushSeg, rcSeg);

            // Draw Highlight Border (White)
            if (isActive) {
                Pen penHigh(Color(255, 255, 255, 255), 2.0f);
                // Draw slightly inside to avoid clipping
                RectF rcHigh = rcSeg;
                rcHigh.Inflate(-1, -1);
                graphics.DrawRectangle(&penHigh, rcHigh);
            }

            // Draw Text
            SolidBrush brushText(isActive ? Color(255, 0, 0, 0) : Color(255, 100, 100, 100));
            graphics.DrawString(seg.strLabel, -1, &fontSeg, rcSeg, &format, &brushText);

            // Move to next position
            if (m_bVertical) rcSeg.Offset(0, segH + gap);
            else rcSeg.Offset(segW + gap, 0);
        }
    }
}