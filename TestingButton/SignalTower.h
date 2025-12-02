#pragma once
#include <gdiplus.h>

class CSignalTower : public CStatic
{
	DECLARE_DYNAMIC(CSignalTower)

public:
	CSignalTower();
	virtual ~CSignalTower();

	enum LightColor { RED = 0, ORANGE, GREEN, BLUE };

	// Call this to toggle lights
	void SetLightState(LightColor color, bool bOn);

protected:
	bool m_bRedOn;
	bool m_bOrangeOn;
	bool m_bGreenOn;
	bool m_bBlueOn;

	ULONG_PTR m_gdiplusToken; // Required for GDI+

	// Helper to draw a realistic lens
	void Draw3DLens(Gdiplus::Graphics& g, Gdiplus::RectF rect, Gdiplus::Color baseColor, bool bIsOn);

	// Helper to draw the metallic base
	void DrawBase(Gdiplus::Graphics& g, Gdiplus::RectF rect);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};