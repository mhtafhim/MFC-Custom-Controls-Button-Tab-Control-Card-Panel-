#pragma once
#include <vector>
#include <algorithm>

struct PointD {
    double x;
    double y;
};

struct DieInfo {
    int id;
    int gridCol;
    int gridRow;
    PointD corners[4];
    bool bIsPartial; // NEW: Track if it's a partial die

    PointD GetCenter() const {
        return { (corners[0].x + corners[2].x) / 2.0, (corners[0].y + corners[2].y) / 2.0 };
    }
};

class CDiceMap : public CStatic
{
public:
    CDiceMap();
    virtual ~CDiceMap();

    void SetWaferParams(double diameterMM, double gapXMM, double gapYMM);
    void SetReferenceDie(PointD pTL, PointD pBL, PointD pBR);

    // NEW: Option to Toggle Partial Dies
    void SetShowPartialDies(bool bShow);

protected:
    double m_dWaferDiameter;
    double m_dGapX;
    double m_dGapY;
    double m_dDieWidth;
    double m_dDieHeight;
    double m_dRotationAngleRad;
    PointD m_RefCenter;

    // NEW: Toggle Flag
    bool m_bShowPartialDies;

    int m_nMaxGridCol;
    int m_nMaxGridRow;
    std::vector<DieInfo> m_DieList;

    void RecalculateLayout();

    // Geometry Helpers
    CPoint LogicalToDevice(PointD logicPt, int cx, int cy, double scale, int offsetX, int offsetY);
    void RotatePoint(PointD& pt, double angleRad);

    // NEW: Math to check intersection
    bool CheckDieIntersection(PointD dieCenter, double w, double h, double angRad, double waferRad, bool& outIsPartial);

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};