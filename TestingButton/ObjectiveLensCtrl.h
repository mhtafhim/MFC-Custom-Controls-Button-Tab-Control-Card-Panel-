#pragma once
#include <vector>
#include <gdiplus.h>

// Link with Gdiplus
#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;

class CObjectiveLensCtrl : public CStatic
{
public:
    CObjectiveLensCtrl();
    virtual ~CObjectiveLensCtrl();

    // Setup: Add a lens like "10x", "40x"
    void AddLens(CString label);

    // Get the currently active lens index
    int GetSelectedIndex() const { return m_nSelectedIndex; }

    // Manually set selection
    void SetSelection(int index);

    virtual void PreSubclassWindow(); // <--- Add this line


protected:
    struct LensItem {
        CString label;
        RectF clickRect; // Stores the hitbox for mouse clicks
    };

    std::vector<LensItem> m_lenses;
    int m_nSelectedIndex;

    // Helper to draw a single lens
    void DrawLens(Graphics& g, RectF bounds, CString label, BOOL isSelected);

    // Message Map Functions
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};