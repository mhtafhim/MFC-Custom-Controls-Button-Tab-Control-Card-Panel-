#include "pch.h"
#include "DiceMap.h"
#include <cmath>
#include <string>
#include <iostream>

CDiceMap::CDiceMap()
{
    m_dWaferDiameter = 300.0;
    m_dDieWidth = 10.0;
    m_dDieHeight = 10.0;
    m_dGapX = 0.0;
    m_dGapY = 0.0;
    m_dRotationAngleRad = 0;
    m_RefCenter = { 0, 0 };
    m_bShowPartialDies = false;
}

CDiceMap::~CDiceMap()
{
}

BEGIN_MESSAGE_MAP(CDiceMap, CStatic)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONUP() // Capture Mouse Click
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
    // Width = distance(BL, BR)
    double dx_w = pBR.x - pBL.x;
    double dy_w = pBR.y - pBL.y;
    m_dDieWidth = std::sqrt(dx_w * dx_w + dy_w * dy_w);

    // Height = distance(BL, TL)
    double dx_h = pTL.x - pBL.x;
    double dy_h = pTL.y - pBL.y;
    m_dDieHeight = std::sqrt(dx_h * dx_h + dy_h * dy_h);

    // Angle
    //m_dRotationAngleRad = std::atan2(dy_w, dx_w);

    // Center of Ref Die
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

bool CDiceMap::CheckDieIntersection(PointD dieCenter, double w, double h, double angRad, double waferRad, bool& outIsPartial)
{
    double halfW = w / 2.0;
    double halfH = h / 2.0;
    PointD corners[4] = { {-halfW, halfH}, {halfW, halfH}, {halfW, -halfH}, {-halfW, -halfH} };
    int cornersInside = 0;

    for (int k = 0; k < 4; k++) {
        RotatePoint(corners[k], angRad);
        corners[k].x += dieCenter.x;
        corners[k].y += dieCenter.y;
        if ((corners[k].x * corners[k].x + corners[k].y * corners[k].y) <= (waferRad * waferRad)) {
            cornersInside++;
        }
    }

    if (cornersInside == 4) { outIsPartial = false; return true; } // Fully Inside
    if (!m_bShowPartialDies) return false;

    // Check Partials
    if (cornersInside > 0) { outIsPartial = true; return true; }

    // Edge crossing check (Corner case)
    PointD vDist = { -dieCenter.x, -dieCenter.y };
    double negAng = -angRad;
    double localCx = vDist.x * std::cos(negAng) - vDist.y * std::sin(negAng);
    double localCy = vDist.x * std::sin(negAng) + vDist.y * std::cos(negAng);
    double closestX = max(-halfW, min(localCx, halfW));
    double closestY = max(-halfH, min(localCy, halfH));
    double dx = localCx - closestX;
    double dy = localCy - closestY;
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
     
    // Scan large enough area to cover offsets
    m_nMaxGridCol = (int)(m_dWaferDiameter / stepX) + 2;
    m_nMaxGridRow = (int)(m_dWaferDiameter / stepY) + 2;

    for (int r = -m_nMaxGridRow; r <= m_nMaxGridRow; r++)
    {
        for (int c = -m_nMaxGridCol; c <= m_nMaxGridCol; c++)
        {
            PointD offset = { c * stepX, r * stepY };
            RotatePoint(offset, m_dRotationAngleRad);
            PointD absCenter = { m_RefCenter.x + offset.x, m_RefCenter.y + offset.y };

            bool isPartial = false;
            if (CheckDieIntersection(absCenter, m_dDieWidth, m_dDieHeight, m_dRotationAngleRad, radius, isPartial))
            {
                DieInfo die;
                die.gridCol = c;
                die.gridRow = r;
                die.bIsPartial = isPartial;
                die.bSelected = false; // Default: Not Selected

                double halfW = m_dDieWidth / 2.0; double halfH = m_dDieHeight / 2.0;
                PointD raw[4] = { {-halfW, halfH}, {halfW, halfH}, {halfW, -halfH}, {-halfW, -halfH} };
                for (int k = 0; k < 4; k++) {
                    RotatePoint(raw[k], m_dRotationAngleRad);
                    die.corners[k] = { absCenter.x + raw[k].x, absCenter.y + raw[k].y };

                }
                //CString str = std::to_wstring(die.corners[0].x).c_str();
                //CString str2 = std::to_wstring(die.corners[0].y).c_str();
                //CString str3 = std::to_wstring(die.corners[1].x).c_str();
                //CString str4 = std::to_wstring(die.corners[1].y).c_str();
                //CString str5 = std::to_wstring(die.corners[2].x).c_str();
                //CString str6 = std::to_wstring(die.corners[2].y).c_str();
                //CString str7 = std::to_wstring(die.corners[3].x).c_str();
                //CString str8 = std::to_wstring(die.corners[3].y).c_str();
                //CString msg;
                //msg.Format(L"C1: X=%s, Y=%s | C2: X=%s, Y=%s | C3: X=%s, Y=%s | C4: X=%s, Y=%s",
                //    str, str2, str3, str4, str5, str6, str7, str8);
                //AfxMessageBox(msg);

                m_DieList.push_back(die);
            }
        }
    }

    // --- SORTING: Bottom to Top, Left to Right ---
    std::sort(m_DieList.begin(), m_DieList.end(), [](const DieInfo& a, const DieInfo& b) {
        PointD ca = a.GetCenter();
        PointD cb = b.GetCenter();

        // 1. Sort by Y ascending (Lowest Y is at the bottom visually)
        if (std::abs(ca.y - cb.y) > 0.001) {
            return ca.y < cb.y;
        }
        // 2. Sort by X ascending (Left to Right)
        return ca.x < cb.x;
        });

    // Assign IDs
    for (size_t i = 0; i < m_DieList.size(); i++) {
        m_DieList[i].id = (int)(i + 1);
    }
}

CPoint CDiceMap::LogicalToDevice(PointD logicPt, int cx, int cy, double scale, int offsetX, int offsetY)
{
    long sx = (long)(offsetX + (logicPt.x * scale));
    long sy = (long)(offsetY - (logicPt.y * scale));
    return CPoint(sx, sy);
}

// --- MOUSE CLICK HANDLER ---
void CDiceMap::OnLButtonUp(UINT nFlags, CPoint point)
{
    CRect rectClient;
    GetClientRect(&rectClient);
    int cx = rectClient.Width();
    int cy = rectClient.Height();
    if (cx == 0) return;

    double contentSize = m_dWaferDiameter * 1.05;
    double scale = min((double)cx, (double)cy) / contentSize;
    int centerX = cx / 2;
    int centerY = cy / 2;
    double rPx = (m_dWaferDiameter / 2.0) * scale;
    double rPxSq = rPx * rPx;

    // Optional: Only check clicks inside the circle
    if (std::pow(point.x - centerX, 2) + std::pow(point.y - centerY, 2) > rPxSq) return;

    bool bChanged = false;

    // Check hit on dies
    for (auto& die : m_DieList)
    {
        CPoint pts[4];
        for (int k = 0; k < 4; k++) pts[k] = LogicalToDevice(die.corners[k], cx, cy, scale, centerX, centerY);

        CRgn rgn;
        rgn.CreatePolygonRgn(pts, 4, ALTERNATE);

        if (rgn.PtInRegion(point))
        {
			//AfxMessageBox(std::to_wstring(die.id).c_str()); // Show Die ID
               CString str = std::to_wstring(die.corners[0].x).c_str();
               CString str2 = std::to_wstring(die.corners[0].y).c_str();
               CString str3 = std::to_wstring(die.corners[1].x).c_str();
               CString str4 = std::to_wstring(die.corners[1].y).c_str();
               CString str5 = std::to_wstring(die.corners[2].x).c_str();
               CString str6 = std::to_wstring(die.corners[2].y).c_str();
               CString str7 = std::to_wstring(die.corners[3].x).c_str();
               CString str8 = std::to_wstring(die.corners[3].y).c_str();
               CString msg;
               msg.Format(L"ID : %s \nC1: X=%s, Y=%s \n C2: X=%s, Y=%s \n C3: X=%s, Y=%s \n C4: X=%s, Y=%s", std::to_wstring(die.id).c_str(),
                   str, str2, str3, str4, str5, str6, str7, str8);
               AfxMessageBox(msg);

          //  AfxMessageBox(std::to_wstring(die.corners[0].y).c_str());

            die.bSelected = !die.bSelected; // Toggle selection
            bChanged = true;
            break; // Stop after finding top-most match
        }
    }

    if (bChanged) Invalidate();
    CStatic::OnLButtonUp(nFlags, point);
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
//    // 1. Background (Light Blue/Grey from original)
//    memDC.FillSolidRect(rectClient, RGB(225, 225, 225));
//
//    double contentSize = m_dWaferDiameter * 1.05;
//    double scale = min((double)cx, (double)cy) / contentSize;
//    int centerX = cx / 2;
//    int centerY = cy / 2;
//    int rPx = (int)((m_dWaferDiameter / 2.0) * scale);
//
//    // 2. Wafer Region
//    CRect rWafer(centerX - rPx, centerY - rPx, centerX + rPx, centerY + rPx);
//    CRgn rgnWafer;
//    rgnWafer.CreateEllipticRgn(rWafer.left, rWafer.top, rWafer.right, rWafer.bottom);
//
//    // Draw Wafer Body
//    CBrush brWafer(RGB(240, 240, 240));
//    CPen penWafer(PS_SOLID, 2, RGB(100, 100, 100));
//    CBrush* pOldBr = memDC.SelectObject(&brWafer);
//    CPen* pOldPen = memDC.SelectObject(&penWafer);
//    memDC.Ellipse(rWafer);
//    memDC.SelectObject(pOldBr);
//    memDC.SelectObject(pOldPen);
//
//    // ================= START CLIP =================
//    memDC.SelectClipRgn(&rgnWafer);
//
//    // 3. Blue Grid
//    CPen penGrid(PS_SOLID, 1, RGB(100, 149, 237)); // Cornflower Blue
//    memDC.SelectObject(&penGrid);
//    double stepX = m_dDieWidth + m_dGapX;
//    double stepY = m_dDieHeight + m_dGapY;
//    double lineLen = m_dWaferDiameter * 1.5;
//
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
//    // 4. Dies (With Selection Colors)
//    CBrush brFull(RGB(255, 255, 255));    // Normal White
//    CBrush brPartial(RGB(210, 210, 210)); // Partial Gray
//    CBrush brRef(RGB(200, 255, 200));     // Ref Green (Optional)
//    CBrush brSelected(RGB(0, 255, 127));  // SELECTED: Bright Green
//
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
//        // --- COLOR PRIORITY ---
//        if (die.bSelected) {
//            memDC.SelectObject(&brSelected); // Selected takes priority
//        }
//        else if (die.bIsPartial) {
//            memDC.SelectObject(&brPartial);
//        }
//        else if (dist < 0.1) {
//            memDC.SelectObject(&brRef);
//        }
//        else {
//            memDC.SelectObject(&brFull);
//        }
//
//        // Draw Rect (Clipped)
//        memDC.Polygon(pts, 4);
//
//        // Draw Text
//        PointD center = die.GetCenter();
//        CPoint ptCenter = LogicalToDevice(center, cx, cy, scale, centerX, centerY);
//        CRect rText(ptCenter.x - 40, ptCenter.y - 40, ptCenter.x + 40, ptCenter.y + 40);
//        CString sID; sID.Format(_T("%d"), die.id);
//        memDC.SetTextColor(RGB(0, 0, 0));
//        memDC.DrawText(sID, rText, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
//    }
//
//    // ================= END CLIP =================
//    memDC.SelectClipRgn(NULL);
//
//    // 5. Redraw Wafer Border Ring (To cover jagged edges)
//    memDC.SelectStockObject(NULL_BRUSH);
//    memDC.SelectObject(&penWafer);
//    memDC.Ellipse(rWafer);
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

    // 1. Window Background
    memDC.FillSolidRect(rectClient, RGB(225, 225, 225));

    double contentSize = m_dWaferDiameter * 1.05;
    double scale = min((double)cx, (double)cy) / contentSize;
    int centerX = cx / 2;
    int centerY = cy / 2;
    int rPx = (int)((m_dWaferDiameter / 2.0) * scale);

    // 2. Wafer Circle (Background)
    CRect rWafer(centerX - rPx, centerY - rPx, centerX + rPx, centerY + rPx);

    CBrush brWafer(RGB(245, 245, 245)); // Very light gray wafer background
    CPen penWafer(PS_SOLID, 2, RGB(80, 80, 80)); // Dark Grey Outline
    memDC.SelectObject(&brWafer);
    memDC.SelectObject(&penWafer);
    memDC.Ellipse(rWafer);

    // 3. Grid Lines
    // We want the lines to cut off at the wafer edge, so we turn on clipping here
    CRgn rgnWafer;
    rgnWafer.CreateEllipticRgn(rWafer.left, rWafer.top, rWafer.right, rWafer.bottom);
    memDC.SelectClipRgn(&rgnWafer);

    CPen penGrid(PS_SOLID, 1, RGB(100, 149, 237)); // Cornflower Blue Streets
    memDC.SelectObject(&penGrid);

    double stepX = m_dDieWidth + m_dGapX;
    double stepY = m_dDieHeight + m_dGapY;
    double lineLen = m_dWaferDiameter * 1.5;

    // Math: The line is exactly halfway between two dies.
    // Offset = DieWidth/2 + Gap/2  =>  (Width + Gap)/2  =>  Step / 2.
    double halfStepX = stepX / 2.0;
    double halfStepY = stepY / 2.0;

    // A. Vertical Lines
    for (int c = -m_nMaxGridCol; c <= m_nMaxGridCol; c++) {
        // Calculate the gap center relative to unrotated (0,0)
        // We start from a Die Center (c * step) and add half a step to get to the gap
        PointD offset = { (c * stepX) + halfStepX, 0 };
        RotatePoint(offset, m_dRotationAngleRad);

        // Apply to Anchor
        PointD lineCenter = { m_RefCenter.x + offset.x, m_RefCenter.y + offset.y };
        PointD vUp = { 0, lineLen }; RotatePoint(vUp, m_dRotationAngleRad);

        memDC.MoveTo(LogicalToDevice({ lineCenter.x + vUp.x, lineCenter.y + vUp.y }, cx, cy, scale, centerX, centerY));
        memDC.LineTo(LogicalToDevice({ lineCenter.x - vUp.x, lineCenter.y - vUp.y }, cx, cy, scale, centerX, centerY));
    }

    // B. Horizontal Lines
    for (int r = -m_nMaxGridRow; r <= m_nMaxGridRow; r++) {
        PointD offset = { 0, (r * stepY) + halfStepY };
        RotatePoint(offset, m_dRotationAngleRad);

        PointD lineCenter = { m_RefCenter.x + offset.x, m_RefCenter.y + offset.y };
        PointD vRight = { lineLen, 0 }; RotatePoint(vRight, m_dRotationAngleRad);

        memDC.MoveTo(LogicalToDevice({ lineCenter.x - vRight.x, lineCenter.y - vRight.y }, cx, cy, scale, centerX, centerY));
        memDC.LineTo(LogicalToDevice({ lineCenter.x + vRight.x, lineCenter.y + vRight.y }, cx, cy, scale, centerX, centerY));
    }

    // DISABLE CLIPPING for Dies (so they stick out if needed)
    memDC.SelectClipRgn(NULL);

    // 4. Draw Dies
    // --- Define Colors Here ---
    CBrush brSilver(RGB(192, 192, 192));      // Standard Inside Die (Silver)
    CBrush brOverlap(RGB(160, 170, 185));     // Overlapping/Partial Die (Darker Blue-Gray)
    CBrush brRef(RGB(192, 210, 192));         // Reference Die (Subtle Green tint)
    CBrush brSelected(RGB(50, 205, 50));      // Selected (Bright Green)

    CPen penBorder(PS_SOLID, 1, RGB(50, 50, 50));
    memDC.SelectObject(&penBorder);

    int fontSize = max(20, (int)(m_dDieHeight * scale * 3.5 ));
    //int fontSize =60;
    CFont font;
    font.CreatePointFont(fontSize, _T("Arial"), &memDC);
    memDC.SelectObject(&font);
    memDC.SetBkMode(TRANSPARENT);

    for (const auto& die : m_DieList)
    {
        CPoint pts[4];
        for (int k = 0; k < 4; k++) pts[k] = LogicalToDevice(die.corners[k], cx, cy, scale, centerX, centerY);

        PointD c = die.GetCenter();
        double distFromRef = std::sqrt(pow(c.x - m_RefCenter.x, 2) + pow(c.y - m_RefCenter.y, 2));

        // --- Color Selection Logic ---
        if (die.bSelected) {
            memDC.SelectObject(&brSelected);
        }
        else if (die.bIsPartial) {
            memDC.SelectObject(&brOverlap); // Different color for overlap
        }
        else if (distFromRef < 0.1) {
            memDC.SelectObject(&brRef);     // Optional: Mark the reference die slightly different
        }
        else {
            memDC.SelectObject(&brSilver);  // Standard Silver
        }

        // Draw Full Polygon (No Clipping)
        memDC.Polygon(pts, 4);

        // Draw ID Text
        PointD center = die.GetCenter();
        CPoint ptCenter = LogicalToDevice(center, cx, cy, scale, centerX, centerY);
        CRect rText(ptCenter.x - 40, ptCenter.y - 40, ptCenter.x + 40, ptCenter.y + 40);

        CString sID;
        sID.Format(_T("%d"), die.id);

        memDC.SetTextColor(RGB(0, 0, 0));
        memDC.DrawText(sID, rText, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
    }

    // 5. Redraw Wafer Border (Cleanup)
    memDC.SelectStockObject(NULL_BRUSH);
    memDC.SelectObject(&penWafer);
    memDC.Ellipse(rWafer);

    dc.BitBlt(0, 0, cx, cy, &memDC, 0, 0, SRCCOPY);
    memDC.SelectObject(pOldBitmap);
}


void CDiceMap::OnSize(UINT nType, int cx, int cy) {
    CStatic::OnSize(nType, cx, cy);
    Invalidate();
}

BOOL CDiceMap::OnEraseBkgnd(CDC* pDC) { return TRUE; }

int CDiceMap::GetSelectedCount() {
    int count = 0;
    for (const auto& die : m_DieList) if (die.bSelected) count++;
    return count;
}

void CDiceMap::ClearSelection() {
    for (auto& die : m_DieList) die.bSelected = false;
    Invalidate();
}