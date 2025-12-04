#include "pch.h"
#include "ObjectiveLensCtrl.h"
#include <string>


//use it for finding selected index
//int nIndex = m_lensCtrl.GetSelectedIndex();




CObjectiveLensCtrl::CObjectiveLensCtrl()
{
    m_nSelectedIndex = 0; // Default to the first lens
}

CObjectiveLensCtrl::~CObjectiveLensCtrl()
{
}

BEGIN_MESSAGE_MAP(CObjectiveLensCtrl, CStatic)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void CObjectiveLensCtrl::AddLens(CString label)
{
    LensItem item;
    item.label = label;
    // clickRect is calculated in OnPaint
    m_lenses.push_back(item);
    Invalidate(); // Trigger redraw
}

void CObjectiveLensCtrl::SetSelection(int index)
{
    if (index >= 0 && index < (int)m_lenses.size())
    {
        m_nSelectedIndex = index;
        Invalidate();
    }
}

BOOL CObjectiveLensCtrl::OnEraseBkgnd(CDC* pDC)
{
    // Return TRUE to handle background in OnPaint (prevents flickering)
    return TRUE;
}

void CObjectiveLensCtrl::OnPaint()
{
    CPaintDC dc(this);
    CRect rectClient;
    GetClientRect(&rectClient);

    // 1. Prepare GDI+ Graphics with Double Buffering
    Graphics graphics(dc.GetSafeHdc());

    // Create a memory bitmap to draw on (prevents flickering)
    Bitmap buffer(rectClient.Width(), rectClient.Height(), &graphics);
    Graphics memGraphics(&buffer);

    // Set High Quality Render Settings
    memGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
    memGraphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);

    // 2. Clear Background (White)
    SolidBrush bgBrush(Color(255, 255, 255, 255));
    memGraphics.FillRectangle(&bgBrush, 0, 0, rectClient.Width(), rectClient.Height());

    if (m_lenses.empty()) {
        graphics.DrawImage(&buffer, 0, 0);
        return;
    }

    // 3. Calculate Layout
    int count = (int)m_lenses.size();
    float padding = 4.0f;
    float availableWidth = (float)rectClient.Width() - (padding * (count + 1));
    float lensWidth = availableWidth / count;

    // Limit max width so they don't look stretched on wide screens
    if (lensWidth > 90.0f) lensWidth = 90.0f;

    float currentX = padding;
    float lensHeight = (float)rectClient.Height() - 10.0f;

    // 4. Draw Loops
    for (int i = 0; i < count; i++)
    {
        RectF lensBounds(currentX, 5.0f, lensWidth, lensHeight);

        // Update the hitbox for this lens
        m_lenses[i].clickRect = lensBounds;

        // Draw the specific lens
        DrawLens(memGraphics, lensBounds, m_lenses[i].label, (i == m_nSelectedIndex));

        currentX += lensWidth + padding;
    }

    // 5. Blit buffer to screen
    graphics.DrawImage(&buffer, 0, 0);
}

void CObjectiveLensCtrl::DrawLens(Graphics& g, RectF bounds, CString label, BOOL isSelected)
{
    // === COLORS & SELECTION LOGIC ===
    Color cDark, cMid, cLight;

    if (isSelected)
    {
        // SELECTED STATE: "Active Blue" Metallic Look
        cDark = Color(255, 0, 80, 160);
        cMid = Color(255, 100, 180, 255);
        cLight = Color(255, 220, 240, 255);
    }
    else
    {
        // UNSELECTED STATE: Standard Silver/Gray Metallic
        cDark = Color(255, 120, 120, 120);
        cMid = Color(255, 220, 220, 220);
        cLight = Color(255, 255, 255, 255);
    }

    // Determine Ring Color based on magnification text (Standard Microscopy Colors)
    Color cStripe = Color(255, 200, 0, 0); // Default Red
    if (label.Find(_T("4x")) != -1) cStripe = Color(255, 220, 20, 20); // Red
    if (label.Find(_T("10x")) != -1) cStripe = Color(255, 255, 215, 0); // Yellow
    if (label.Find(_T("20x")) != -1) cStripe = Color(255, 50, 205, 50); // Green
    if (label.Find(_T("40x")) != -1) cStripe = Color(255, 0, 191, 255); // Blue
    if (label.Find(_T("100x")) != -1) cStripe = Color(255, 240, 240, 240); // White/Gray

    // === GEOMETRY ===
    float bottomHeight = bounds.Height * 0.18f;
    float bodyHeight = bounds.Height - bottomHeight;
    RectF bodyRect(bounds.X, bounds.Y, bounds.Width, bodyHeight);

    float nozzleW = bounds.Width * 0.70f;
    float nozzleX = bounds.X + (bounds.Width - nozzleW) / 2.0f;
    RectF nozzleRect(nozzleX, bounds.Y + bodyHeight, nozzleW, bottomHeight);

    // === GRADIENT BRUSH ===
    LinearGradientBrush brush(bounds, cDark, cDark, LinearGradientModeHorizontal);
    // Gradient: Edge(Dark) -> 20%(Mid) -> 50%(Light) -> 80%(Mid) -> Edge(Dark)
    float positions[] = { 0.0f, 0.2f, 0.5f, 0.8f, 1.0f };
    Color colors[] = { cDark, cMid, cLight, cMid, cDark };
    brush.SetInterpolationColors(colors, positions, 5);

    // === DRAWING OPERATIONS ===

    // 1. Draw Main Body
    g.FillRectangle(&brush, bodyRect);

    // 2. Draw Color Identification Ring
    SolidBrush stripeBrush(cStripe);
    g.FillRectangle(&stripeBrush, bounds.X, bounds.Y + 15.0f, bounds.Width, 6.0f);

    // 3. Draw Groove lines (Dark rings)
    SolidBrush grooveBrush(Color(100, 0, 0, 0));
    g.FillRectangle(&grooveBrush, bounds.X, bounds.Y + 15.0f, bounds.Width, 1.0f);
    g.FillRectangle(&grooveBrush, bounds.X, bounds.Y + 21.0f, bounds.Width, 1.0f);

    // 4. Draw Nozzle (The bottom rounded part)
    GraphicsPath path;
    path.AddLine(nozzleRect.X, nozzleRect.Y, nozzleRect.GetRight(), nozzleRect.Y);
    path.AddLine(nozzleRect.GetRight(), nozzleRect.Y, nozzleRect.GetRight(), nozzleRect.GetBottom() - 5.0f);
    // IMPORTANT: Using .0f to ensure they are treated as Floats for the correct overload
    path.AddArc(nozzleRect.X, nozzleRect.GetBottom() - 10.0f, nozzleRect.Width, 10.0f, 0.0f, 180.0f);
    path.AddLine(nozzleRect.X, nozzleRect.GetBottom() - 5.0f, nozzleRect.X, nozzleRect.Y);
    path.CloseFigure();

    g.FillPath(&brush, &path);

    // 5. Draw Outline / Highlight
    Color cOutline = isSelected ? Color(255, 0, 60, 150) : Color(255, 100, 100, 100);
    Pen outlinePen(cOutline, isSelected ? 2.0f : 1.0f);

    g.DrawRectangle(&outlinePen, bodyRect);
    g.DrawPath(&outlinePen, &path);

    // 6. Draw Text Label
    FontFamily fontFamily(L"Segoe UI");
    Gdiplus::Font font(&fontFamily, 9, FontStyleBold, UnitPoint);
    SolidBrush textBrush(Color(255, 40, 40, 40));

    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentCenter);

    RectF txtRect(bounds.X, bounds.Y + 25.0f, bounds.Width, bodyHeight - 25.0f);
    std::wstring wLabel(label);
    g.DrawString(wLabel.c_str(), -1, &font, txtRect, &format, &textBrush);
}

void CObjectiveLensCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    PointF pt((REAL)point.x, (REAL)point.y);
    bool bSelectionChanged = false;

    // Check which lens was clicked
    for (int i = 0; i < (int)m_lenses.size(); ++i)
    {
        if (m_lenses[i].clickRect.Contains(pt))
        {
            // If clicking a different lens than currently selected
            if (m_nSelectedIndex != i)
            {
                m_nSelectedIndex = i;
                bSelectionChanged = true;

                // Notify Parent Window (Dialog) that selection changed
                GetParent()->PostMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), STN_CLICKED), (LPARAM)m_hWnd);
            }
            break;
        }
    }

    // Only redraw if selection changed
    if (bSelectionChanged)
    {
        Invalidate();
    }

    CStatic::OnLButtonDown(nFlags, point);
}


void CObjectiveLensCtrl::PreSubclassWindow()
{
    // Ensure the control sends mouse notifications to the parent
    // and accepts mouse clicks itself.
    ModifyStyle(0, SS_NOTIFY);

    CStatic::PreSubclassWindow();
}