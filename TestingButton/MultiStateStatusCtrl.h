#pragma once
#include <vector>
#include <afxwin.h>

// Structure to define a single block in the control
struct StateSegment {
    int nID;            // The ID used by backend to trigger this
    CString strLabel;   // Text to display (e.g., "ALARM")
    COLORREF color;     // The "On" color
};

class CMultiStateStatusCtrl : public CStatic
{
public:
    CMultiStateStatusCtrl();
    virtual ~CMultiStateStatusCtrl();

    // Configuration: Call these in OnInitDialog
    void AddSegment(int nID, CString strLabel, COLORREF color);
    void ResetSegments(); // Clears all segments

    // Runtime: Call this to change status
    void SetActiveId(int nID);
    int GetActiveId() const { return m_nActiveId; }

protected:
    std::vector<StateSegment> m_segments;
    int m_nActiveId;
    CBrush m_brushBg;
    CFont m_font;

    // Drawing Logic
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    // Helper to calculate dimmed color
    COLORREF GetDimmedColor(COLORREF color);

    DECLARE_MESSAGE_MAP()
};