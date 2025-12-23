#include "pch.h"
#include "DiceMap.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

CDiceMap::CDiceMap()
{
    m_dWaferDiameter = 300.0;
    m_dDieWidth = 10.0;
    m_dDieHeight = 10.0;
    m_dGapX = 0.0;
    m_dGapY = 0.0;
    m_dRotationAngleRad = 0.0;
    m_RefCenter = { 0, 0 };
    m_bShowPartialDies = false; // Default: OFF
}

CDiceMap::~CDiceMap()
{
}

BEGIN_MESSAGE_MAP(CDiceMap, CStatic)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CDiceMap::SetShowPartialDies(bool bShow)
{
    m_bShowPartialDies = bShow;
    RecalculateLayout();
    if (GetSafeHwnd()) Invalidate();
}

void CDiceMap::SetWaferParams(double diameterMM, double gapXMM, double gapYMM)
{
    m_dWaferDiameter = diameterMM;
    m_dGapX = gapXMM;
    m_dGapY = gapYMM;
}

void CDiceMap::SetReferenceDie(PointD pTL, PointD pBL, PointD pBR)
{
    double dx_w = pBR.x - pBL.x;
    double dy_w = pBR.y - pBL.y;
    m_dDieWidth = std::sqrt(dx_w * dx_w + dy_w * dy_w);

    double dx_h = pTL.x - pBL.x;
    double dy_h = pTL.y - pBL.y;
    m_dDieHeight = std::sqrt(dx_h * dx_h + dy_h * dy_h);

    m_dRotationAngleRad = std::atan2(dy_w, dx_w);

    m_RefCenter.x = (pTL.x + pBR.x) / 2.0;
    m_RefCenter.y = (pTL.y + pBR.y) / 2.0;

    RecalculateLayout();
    if (GetSafeHwnd()) Invalidate();
}

void CDiceMap::RotatePoint(PointD& pt, double angleRad)
{
    double x_new = pt.x * std::cos(angleRad) - pt.y * std::sin(angleRad);
    double y_new = pt.x * std::sin(angleRad) + pt.y * std::cos(angleRad);
    pt.x = x_new;
    pt.y = y_new;
}

// MATH: Check if a rotated rectangle intersects or is contained by a circle at (0,0)
bool CDiceMap::CheckDieIntersection(PointD dieCenter, double w, double h, double angRad, double waferRad, bool& outIsPartial)
{
    double halfW = w / 2.0;
    double halfH = h / 2.0;

    // 1. Calculate the 4 Corners relative to Wafer Center (0,0)
    PointD corners[4];
    corners[0] = { -halfW, +halfH }; // Local TL
    corners[1] = { +halfW, +halfH };
    corners[2] = { +halfW, -halfH };
    corners[3] = { -halfW, -halfH };

    int cornersInside = 0;
    for (int k = 0; k < 4; k++) {
        // Rotate local corner to align with die rotation
        RotatePoint(corners[k], angRad);
        // Translate to absolute position
        corners[k].x += dieCenter.x;
        corners[k].y += dieCenter.y;

        // Check distance
        if ((corners[k].x * corners[k].x + corners[k].y * corners[k].y) <= (waferRad * waferRad)) {
            cornersInside++;
        }
    }

    // CASE 1: Full Die (All 4 corners inside)
    if (cornersInside == 4) {
        outIsPartial = false;
        return true;
    }

    // CASE 2: If we don't want partial dies, we stop here
    if (!m_bShowPartialDies) {
        return false;
    }

    // CASE 3: Partial Die Logic
    // If at least 1 corner is inside, it definitely intersects
    if (cornersInside > 0) {
        outIsPartial = true;
        return true;
    }

    // CASE 4: Hard Case - All corners outside, but edges cut through the circle
    // To check this, we transform the CIRCLE center into the RECTANGLE'S local space.
    // Vector from Die Center to Wafer Center (0,0)
    PointD vDist = { 0.0 - dieCenter.x, 0.0 - dieCenter.y };

    // Rotate this vector backwards by -angRad to align with axis-aligned rectangle
    double negAng = -angRad;
    double localCx = vDist.x * std::cos(negAng) - vDist.y * std::sin(negAng);
    double localCy = vDist.x * std::sin(negAng) + vDist.y * std::cos(negAng);

    // Find closest point on Axis-Aligned Rect to the transformed Circle Center
    double closestX = max(-halfW, min(localCx, halfW));
    double closestY = max(-halfH, min(localCy, halfH));

    // Distance from Circle Center to that closest point
    double dx = localCx - closestX;
    double dy = localCy - closestY;

    // If distance is less than radius, they touch
    if ((dx * dx + dy * dy) <= (waferRad * waferRad)) {
        outIsPartial = true;
        return true;
    }

    return false;
}

void CDiceMap::RecalculateLayout()
{
    m_DieList.clear();
    if (m_dWaferDiameter <= 0 || m_dDieWidth <= 0 || m_dDieHeight <= 0) return;

    double radius = m_dWaferDiameter / 2.0;
    double stepX = m_dDieWidth + m_dGapX;
    double stepY = m_dDieHeight + m_dGapY;

    m_nMaxGridCol = (int)(radius / stepX) + 2;
    m_nMaxGridRow = (int)(radius / stepY) + 2;

    for (int r = -m_nMaxGridRow; r <= m_nMaxGridRow; r++)
    {
        for (int c = -m_nMaxGridCol; c <= m_nMaxGridCol; c++)
        {
            // 1. Calculate Absolute Center of this die
            PointD offset;
            offset.x = c * stepX;
            offset.y = r * stepY;
            RotatePoint(offset, m_dRotationAngleRad);

            PointD absCenter;
            absCenter.x = m_RefCenter.x + offset.x;
            absCenter.y = m_RefCenter.y + offset.y;

            // 2. Check Validity
            bool isPartial = false;
            bool isValid = CheckDieIntersection(absCenter, m_dDieWidth, m_dDieHeight, m_dRotationAngleRad, radius, isPartial);

            if (isValid)
            {
                DieInfo die;
                die.gridCol = c;
                die.gridRow = r;
                die.bIsPartial = isPartial;

                // Reconstruct corners for drawing
                double halfW = m_dDieWidth / 2.0; double halfH = m_dDieHeight / 2.0;
                PointD raw[4] = { {-halfW, halfH}, {halfW, halfH}, {halfW, -halfH}, {-halfW, -halfH} };

                for (int k = 0; k < 4; k++) {
                    RotatePoint(raw[k], m_dRotationAngleRad);
                    die.corners[k].x = absCenter.x + raw[k].x;
                    die.corners[k].y = absCenter.y + raw[k].y;
                }

                die.id = (c == 0 && r == 0) ? 9999 : 0;
                m_DieList.push_back(die);
            }
        }
    }

    // Sort
    std::sort(m_DieList.begin(), m_DieList.end(), [](const DieInfo& a, const DieInfo& b) {
        PointD ca = a.GetCenter();
        PointD cb = b.GetCenter();
        if (std::abs(ca.y - cb.y) > 0.001) return ca.y > cb.y;
        return ca.x < cb.x;
        });

    // Renumber (Partial dies get IDs too, usually)
    int counter = 1;
    for (size_t i = 0; i < m_DieList.size(); i++) {
        // Optionally, you could skip numbering partial dies if desired
        m_DieList[i].id = counter++;
    }
}

CPoint CDiceMap::LogicalToDevice(PointD logicPt, int cx, int cy, double scale, int offsetX, int offsetY)
{
    long sx = (long)(offsetX + (logicPt.x * scale));
    long sy = (long)(offsetY - (logicPt.y * scale));
    return CPoint(sx, sy);
}

//void CDiceMap::OnPaint()
//{
//    CPaintDC dc(this);
//    CRect rectClient;
//    GetClientRect(&rectClient);
//    int cx = rectClient.Width();
//    int cy = rectClient.Height();
//    if (cx == 0) return;
//
//    CDC memDC;
//    memDC.CreateCompatibleDC(&dc);
//    CBitmap memBitmap;
//    memBitmap.CreateCompatibleBitmap(&dc, cx, cy);
//    CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);
//
//    memDC.FillSolidRect(rectClient, RGB(225, 225, 225));
//
//    double contentSize = m_dWaferDiameter * 1.05;
//    double scale = min((double)cx, (double)cy) / contentSize;
//    int centerX = cx / 2;
//    int centerY = cy / 2;
//
//    // Draw Wafer
//    int rPx = (int)((m_dWaferDiameter / 2.0) * scale);
//    CRect rWafer(centerX - rPx, centerY - rPx, centerX + rPx, centerY + rPx);
//    CRgn rgnWafer;
//    rgnWafer.CreateEllipticRgn(rWafer.left, rWafer.top, rWafer.right, rWafer.bottom);
//
//    CBrush brWafer(RGB(240, 240, 240));
//    CPen penWafer(PS_SOLID, 2, RGB(100, 100, 100));
//    memDC.SelectObject(&brWafer);
//    memDC.SelectObject(&penWafer);
//    memDC.Ellipse(rWafer);
//
//    // Draw Grid (Clipped)
//    memDC.SelectClipRgn(&rgnWafer);
//    CPen penGrid(PS_SOLID, 1, RGB(100, 149, 237));
//    memDC.SelectObject(&penGrid);
//
//    double stepX = m_dDieWidth + m_dGapX;
//    double stepY = m_dDieHeight + m_dGapY;
//    double lineLen = m_dWaferDiameter * 1.5;
//
//    // Draw grid relative to anchor
//    for (int c = -m_nMaxGridCol; c <= m_nMaxGridCol + 1; c++) {
//        PointD offset = { (c * stepX) - (stepX / 2.0), 0 };
//        RotatePoint(offset, m_dRotationAngleRad);
//        PointD center = { m_RefCenter.x + offset.x, m_RefCenter.y + offset.y };
//        PointD vUp = { 0, lineLen }; RotatePoint(vUp, m_dRotationAngleRad);
//        memDC.MoveTo(LogicalToDevice({ center.x + vUp.x, center.y + vUp.y }, cx, cy, scale, centerX, centerY));
//        memDC.LineTo(LogicalToDevice({ center.x - vUp.x, center.y - vUp.y }, cx, cy, scale, centerX, centerY));
//    }
//    for (int r = -m_nMaxGridRow; r <= m_nMaxGridRow + 1; r++) {
//        PointD offset = { 0, (r * stepY) - (stepY / 2.0) };
//        RotatePoint(offset, m_dRotationAngleRad);
//        PointD center = { m_RefCenter.x + offset.x, m_RefCenter.y + offset.y };
//        PointD vRight = { lineLen, 0 }; RotatePoint(vRight, m_dRotationAngleRad);
//        memDC.MoveTo(LogicalToDevice({ center.x - vRight.x, center.y - vRight.y }, cx, cy, scale, centerX, centerY));
//        memDC.LineTo(LogicalToDevice({ center.x + vRight.x, center.y + vRight.y }, cx, cy, scale, centerX, centerY));
//    }
//
//    memDC.SelectClipRgn(NULL);
//
//    // Draw Dies
//    CBrush brFull(RGB(255, 255, 255));
//    CBrush brPartial(RGB(200, 200, 200)); // Darker grey for partials
//    CBrush brRef(RGB(200, 255, 200));
//    CPen penBorder(PS_SOLID, 1, RGB(50, 50, 50));
//    memDC.SelectObject(&penBorder);
//
//    int fontSize = max(80, (int)(m_dDieHeight * scale * 4));
//    CFont font;
//    font.CreatePointFont(fontSize, _T("Arial"), &memDC);
//    memDC.SelectObject(&font);
//    memDC.SetBkMode(TRANSPARENT);
//
//    for (const auto& die : m_DieList)
//    {
//        CPoint pts[4];
//        for (int k = 0; k < 4; k++) pts[k] = LogicalToDevice(die.corners[k], cx, cy, scale, centerX, centerY);
//
//        PointD c = die.GetCenter();
//        double dist = std::sqrt(pow(c.x - m_RefCenter.x, 2) + pow(c.y - m_RefCenter.y, 2));
//
//        // Color Logic
//        if (die.bIsPartial) {
//            memDC.SelectObject(&brPartial);
//        }
//        else if (dist < 0.1) {
//            memDC.SelectObject(&brRef);
//        }
//        else {
//            memDC.SelectObject(&brFull);
//        }
//
//        memDC.Polygon(pts, 4);
//
//        // Optional: Draw red X on partials (like your image)
//        if (die.bIsPartial) {
//            CPen redPen(PS_SOLID, 2, RGB(255, 0, 0));
//            CPen* pOld = memDC.SelectObject(&redPen);
//            memDC.MoveTo(pts[0]); memDC.LineTo(pts[2]);
//            memDC.MoveTo(pts[1]); memDC.LineTo(pts[3]);
//            memDC.SelectObject(pOld);
//        }
//        else {
//            // Only show numbers on full dies? Or all? Currently all.
//            PointD center = die.GetCenter();
//            CPoint ptCenter = LogicalToDevice(center, cx, cy, scale, centerX, centerY);
//            CRect rText(ptCenter.x - 30, ptCenter.y - 30, ptCenter.x + 30, ptCenter.y + 30);
//            CString sID;
//            sID.Format(_T("%d"), die.id);
//            memDC.DrawText(sID, rText, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
//        }
//    }
//
//    dc.BitBlt(0, 0, cx, cy, &memDC, 0, 0, SRCCOPY);
//    memDC.SelectObject(pOldBitmap);
//}

//void CDiceMap::OnPaint()
//{
//    CPaintDC dc(this);
//    CRect rectClient;
//    GetClientRect(&rectClient);
//    int cx = rectClient.Width();
//    int cy = rectClient.Height();
//    if (cx == 0) return;
//
//    // --- Double Buffering ---
//    CDC memDC;
//    memDC.CreateCompatibleDC(&dc);
//    CBitmap memBitmap;
//    memBitmap.CreateCompatibleBitmap(&dc, cx, cy);
//    CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);
//
//    // 1. Fill Background
//    memDC.FillSolidRect(rectClient, RGB(225, 225, 225));
//
//    double contentSize = m_dWaferDiameter * 1.05;
//    double scale = min((double)cx, (double)cy) / contentSize;
//    int centerX = cx / 2;
//    int centerY = cy / 2;
//
//    // 2. Draw Wafer Outline
//    int rPx = (int)((m_dWaferDiameter / 2.0) * scale);
//    CRect rWafer(centerX - rPx, centerY - rPx, centerX + rPx, centerY + rPx);
//    CRgn rgnWafer;
//    rgnWafer.CreateEllipticRgn(rWafer.left, rWafer.top, rWafer.right, rWafer.bottom);
//
//    CBrush brWafer(RGB(240, 240, 240));
//    CPen penWafer(PS_SOLID, 2, RGB(100, 100, 100));
//    memDC.SelectObject(&brWafer);
//    memDC.SelectObject(&penWafer);
//    memDC.Ellipse(rWafer);
//
//    // 3. Draw Blue Grid Lines
//    memDC.SelectClipRgn(&rgnWafer);
//    CPen penGrid(PS_SOLID, 1, RGB(100, 149, 237));
//    memDC.SelectObject(&penGrid);
//
//    double stepX = m_dDieWidth + m_dGapX;
//    double stepY = m_dDieHeight + m_dGapY;
//    double lineLen = m_dWaferDiameter * 1.5;
//
//    // Vertical Lines
//    for (int c = -m_nMaxGridCol; c <= m_nMaxGridCol + 1; c++) {
//        PointD offset = { (c * stepX) - (stepX / 2.0), 0 };
//        RotatePoint(offset, m_dRotationAngleRad);
//        PointD center = { m_RefCenter.x + offset.x, m_RefCenter.y + offset.y };
//        PointD vUp = { 0, lineLen }; RotatePoint(vUp, m_dRotationAngleRad);
//        memDC.MoveTo(LogicalToDevice({ center.x + vUp.x, center.y + vUp.y }, cx, cy, scale, centerX, centerY));
//        memDC.LineTo(LogicalToDevice({ center.x - vUp.x, center.y - vUp.y }, cx, cy, scale, centerX, centerY));
//    }
//    // Horizontal Lines
//    for (int r = -m_nMaxGridRow; r <= m_nMaxGridRow + 1; r++) {
//        PointD offset = { 0, (r * stepY) - (stepY / 2.0) };
//        RotatePoint(offset, m_dRotationAngleRad);
//        PointD center = { m_RefCenter.x + offset.x, m_RefCenter.y + offset.y };
//        PointD vRight = { lineLen, 0 }; RotatePoint(vRight, m_dRotationAngleRad);
//        memDC.MoveTo(LogicalToDevice({ center.x - vRight.x, center.y - vRight.y }, cx, cy, scale, centerX, centerY));
//        memDC.LineTo(LogicalToDevice({ center.x + vRight.x, center.y + vRight.y }, cx, cy, scale, centerX, centerY));
//    }
//
//    memDC.SelectClipRgn(NULL);
//
//    // 4. Draw Dies
//    CBrush brFull(RGB(255, 255, 255));    // White for full dies
//    CBrush brPartial(RGB(210, 210, 210)); // Light Grey for partials (edge dies)
//    CBrush brRef(RGB(200, 255, 200));     // Light Green for reference die
//
//    CPen penBorder(PS_SOLID, 1, RGB(50, 50, 50));
//    memDC.SelectObject(&penBorder);
//
//    // Setup Font
//    int fontSize = max(80, (int)(m_dDieHeight * scale * 4));
//    CFont font;
//    font.CreatePointFont(fontSize, _T("Arial"), &memDC);
//    memDC.SelectObject(&font);
//    memDC.SetBkMode(TRANSPARENT);
//
//    for (const auto& die : m_DieList)
//    {
//        // Calculate Screen Coordinates
//        CPoint pts[4];
//        for (int k = 0; k < 4; k++) pts[k] = LogicalToDevice(die.corners[k], cx, cy, scale, centerX, centerY);
//
//        PointD c = die.GetCenter();
//        double dist = std::sqrt(pow(c.x - m_RefCenter.x, 2) + pow(c.y - m_RefCenter.y, 2));
//
//        // --- Select Color ---
//        if (die.bIsPartial) {
//            memDC.SelectObject(&brPartial); // Gray background
//        }
//        else if (dist < 0.1) {
//            memDC.SelectObject(&brRef);     // Green background
//        }
//        else {
//            memDC.SelectObject(&brFull);    // White background
//        }
//
//        // --- Draw Rectangle ---
//        memDC.Polygon(pts, 4);
//
//        // --- Draw Number ---
//        PointD center = die.GetCenter();
//        CPoint ptCenter = LogicalToDevice(center, cx, cy, scale, centerX, centerY);
//
//        CRect rText(ptCenter.x - 40, ptCenter.y - 40, ptCenter.x + 40, ptCenter.y + 40);
//        CString sID;
//        sID.Format(_T("%d"), die.id);
//
//        memDC.SetTextColor(RGB(0, 0, 0));
//        memDC.DrawText(sID, rText, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
//    }
//
//    dc.BitBlt(0, 0, cx, cy, &memDC, 0, 0, SRCCOPY);
//    memDC.SelectObject(pOldBitmap);
//}

void CDiceMap::OnPaint()
{
    CPaintDC dc(this);
    CRect rectClient;
    GetClientRect(&rectClient);
    int cx = rectClient.Width();
    int cy = rectClient.Height();
    if (cx == 0) return;

    // --- Double Buffering ---
    CDC memDC;
    memDC.CreateCompatibleDC(&dc);
    CBitmap memBitmap;
    memBitmap.CreateCompatibleBitmap(&dc, cx, cy);
    CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

    // 1. Fill the Window Background (The area OUTSIDE the wafer)
    // Using a darker color to make the wafer pop
    memDC.FillSolidRect(rectClient, RGB(220, 225, 235));

    // Calculate Scale
    double contentSize = m_dWaferDiameter * 1.05;
    double scale = min((double)cx, (double)cy) / contentSize;
    int centerX = cx / 2;
    int centerY = cy / 2;
    int rPx = (int)((m_dWaferDiameter / 2.0) * scale);

    // 2. Define the Wafer Region (The Circle)
    CRect rWafer(centerX - rPx, centerY - rPx, centerX + rPx, centerY + rPx);
    CRgn rgnWafer;
    rgnWafer.CreateEllipticRgn(rWafer.left, rWafer.top, rWafer.right, rWafer.bottom);

    // 3. Draw the Wafer Background (The Silver part)
    CBrush brWafer(RGB(240, 240, 240));
    CPen penWafer(PS_SOLID, 2, RGB(100, 100, 100));

    // Draw the circle *before* clipping to establish the base color
    CBrush* pOldBr = memDC.SelectObject(&brWafer);
    CPen* pOldPen = memDC.SelectObject(&penWafer);
    memDC.Ellipse(rWafer);
    memDC.SelectObject(pOldBr);
    memDC.SelectObject(pOldPen);

    // ===========================================================
    // ACTIVATE CLIPPING
    // Everything drawn after this line will be physically cut off 
    // if it goes outside the circle.
    // ===========================================================
    memDC.SelectClipRgn(&rgnWafer);

    // 4. Draw Blue Grid Lines
    CPen penGrid(PS_SOLID, 1, RGB(100, 149, 237));
    memDC.SelectObject(&penGrid);

    double stepX = m_dDieWidth + m_dGapX;
    double stepY = m_dDieHeight + m_dGapY;
    double lineLen = m_dWaferDiameter * 1.5;

    // Vertical Lines
    for (int c = -m_nMaxGridCol; c <= m_nMaxGridCol + 1; c++) {
        PointD offset = { (c * stepX) - (stepX / 2.0), 0 };
        RotatePoint(offset, m_dRotationAngleRad);
        PointD center = { m_RefCenter.x + offset.x, m_RefCenter.y + offset.y };
        PointD vUp = { 0, lineLen }; RotatePoint(vUp, m_dRotationAngleRad);
        memDC.MoveTo(LogicalToDevice({ center.x + vUp.x, center.y + vUp.y }, cx, cy, scale, centerX, centerY));
        memDC.LineTo(LogicalToDevice({ center.x - vUp.x, center.y - vUp.y }, cx, cy, scale, centerX, centerY));
    }
    // Horizontal Lines
    for (int r = -m_nMaxGridRow; r <= m_nMaxGridRow + 1; r++) {
        PointD offset = { 0, (r * stepY) - (stepY / 2.0) };
        RotatePoint(offset, m_dRotationAngleRad);
        PointD center = { m_RefCenter.x + offset.x, m_RefCenter.y + offset.y };
        PointD vRight = { lineLen, 0 }; RotatePoint(vRight, m_dRotationAngleRad);
        memDC.MoveTo(LogicalToDevice({ center.x - vRight.x, center.y - vRight.y }, cx, cy, scale, centerX, centerY));
        memDC.LineTo(LogicalToDevice({ center.x + vRight.x, center.y + vRight.y }, cx, cy, scale, centerX, centerY));
    }

    // 5. Draw Dies (NOW INSIDE THE CLIPPED REGION)
    CBrush brFull(RGB(255, 255, 255));
    CBrush brPartial(RGB(210, 210, 210));
    CBrush brRef(RGB(200, 255, 200));
    CPen penBorder(PS_SOLID, 1, RGB(50, 50, 50));
    memDC.SelectObject(&penBorder);

    int fontSize = max(80, (int)(m_dDieHeight * scale * 4));
    CFont font;
    font.CreatePointFont(fontSize, _T("Arial"), &memDC);
    memDC.SelectObject(&font);
    memDC.SetBkMode(TRANSPARENT);

    for (const auto& die : m_DieList)
    {
        // 1. Calculate the 4 corners (even if they go way outside)
        CPoint pts[4];
        for (int k = 0; k < 4; k++) pts[k] = LogicalToDevice(die.corners[k], cx, cy, scale, centerX, centerY);

        PointD c = die.GetCenter();
        double dist = std::sqrt(pow(c.x - m_RefCenter.x, 2) + pow(c.y - m_RefCenter.y, 2));

        // 2. Set Color
        if (die.bIsPartial) memDC.SelectObject(&brPartial);
        else if (dist < 0.1) memDC.SelectObject(&brRef);
        else memDC.SelectObject(&brFull);

        // 3. Draw Polygon
        // Because Clipping is active, GDI will chop off any pixel outside the circle
        memDC.Polygon(pts, 4);

        // 4. Draw Text
        // Even the text will be clipped if the center of the die is near the edge
        PointD center = die.GetCenter();
        CPoint ptCenter = LogicalToDevice(center, cx, cy, scale, centerX, centerY);
        CRect rText(ptCenter.x - 40, ptCenter.y - 40, ptCenter.x + 40, ptCenter.y + 40);
        CString sID; sID.Format(_T("%d"), die.id);
        memDC.SetTextColor(RGB(0, 0, 0));
        memDC.DrawText(sID, rText, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
    }

    // ===========================================================
    // DISABLE CLIPPING
    // ===========================================================
    memDC.SelectClipRgn(NULL);

    // 6. Draw Wafer Border Ring on top
    // We redraw the outline *after* everything else. 
    // This covers up the jagged edges where the dies were clipped.
    memDC.SelectStockObject(NULL_BRUSH); // Don't fill, just outline
    memDC.SelectObject(&penWafer);       // Use the thick grey pen
    memDC.Ellipse(rWafer);

    // Final Blit
    dc.BitBlt(0, 0, cx, cy, &memDC, 0, 0, SRCCOPY);
    memDC.SelectObject(pOldBitmap);
}


void CDiceMap::OnSize(UINT nType, int cx, int cy) {
    CStatic::OnSize(nType, cx, cy);
    Invalidate();
}

BOOL CDiceMap::OnEraseBkgnd(CDC* pDC) { return TRUE; }