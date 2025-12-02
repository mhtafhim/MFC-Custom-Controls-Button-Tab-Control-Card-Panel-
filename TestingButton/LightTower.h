#pragma once
#include "afxwin.h"

// Define 4 distinct modes + OFF
enum TowerMode {
    TOWER_OFF = 0,
    TOWER_RED,      // Top
    TOWER_AMBER,    // Second
    TOWER_GREEN,    // Third
    TOWER_BLUE      // Bottom
};

class CLightTower : public CStatic
{
public:
    CLightTower();
    virtual ~CLightTower();

    void SetMode(TowerMode mode);

protected:
    TowerMode m_CurrentMode;
    void DrawSegment(CDC* pDC, CRect rect, COLORREF color, bool bOn);

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};