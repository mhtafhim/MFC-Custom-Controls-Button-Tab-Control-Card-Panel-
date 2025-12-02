#include "pch.h"
#include "SignalTower.h"
#include <gdiplus.h>

using namespace Gdiplus;

IMPLEMENT_DYNAMIC(CSignalTower, CStatic)

CSignalTower::CSignalTower()
	: m_bRedOn(false), m_bOrangeOn(false), m_bGreenOn(false), m_bBlueOn(false)
{
	// Initialize GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

CSignalTower::~CSignalTower()
{
	Gdiplus::GdiplusShutdown(m_gdiplusToken);
}

BEGIN_MESSAGE_MAP(CSignalTower, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CSignalTower::OnEraseBkgnd(CDC* pDC)
{
	return TRUE; // Prevent flickering
}

void CSignalTower::SetLightState(LightColor color, bool bOn)
{
	switch (color)
	{
	case RED:    m_bRedOn = bOn; break;
	case ORANGE: m_bOrangeOn = bOn; break;
	case GREEN:  m_bGreenOn = bOn; break;
	case BLUE:   m_bBlueOn = bOn; break;
	}
	Invalidate(); // Trigger redraw
}




void CSignalTower::Draw3DLens(Gdiplus::Graphics& g, Gdiplus::RectF rect, Gdiplus::Color baseColor, bool bIsOn)
{
	// 1. Calculate Colors based on State
	Color cCenter, cEdge;

	if (bIsOn)
	{
		// ON: Center is almost white (glow), Edge is the pure color
		cCenter = Color(255, 255, 255, 255);
		cEdge = baseColor;
	}
	else
	{
		// OFF: Center is the base color but dimmed, Edge is very dark (shadow)
		// This keeps the color visible but makes it look "unlit"
		cCenter = Color(baseColor.GetR() / 2, baseColor.GetG() / 2, baseColor.GetB() / 2);
		cEdge = Color(40, 40, 40); // Dark grey edges
	}

	// 2. Create the Gradient Brush
	LinearGradientBrush brush(rect, cEdge, cCenter, LinearGradientModeHorizontal);

	// 3. Set a Custom Blend Curve for 3D Cylinder Effect
	// This creates: Dark Edge -> Color Body -> Bright Highlight -> Color Body -> Dark Edge
	REAL factors[] = { 0.0f, 0.5f, 1.0f, 0.5f, 0.0f }; // 0=Dark, 1=Bright
	REAL positions[] = { 0.0f, 0.2f, 0.5f, 0.8f, 1.0f }; // Positions along width
	brush.SetBlend(factors, positions, 5);

	g.FillRectangle(&brush, rect);

	// 4. Draw Texture Ridges (The horizontal lines on the plastic)
	// We draw faint lines to simulate the fresnel lens texture
	Pen ridgePen(Color(30, 0, 0, 0), 1.0f); // Semi-transparent black
	int lines = (int)(rect.Height / 3); // A line every 3 pixels

	for (int i = 0; i < lines; i++) {
		REAL y = rect.Y + (i * 3);
		g.DrawLine(&ridgePen, rect.X, y, rect.X + rect.Width, y);
	}

	// 5. Draw a thin border to separate segments
	Pen borderPen(Color(100, 100, 100), 1.0f);
	g.DrawRectangle(&borderPen, rect);
}

void CSignalTower::DrawBase(Gdiplus::Graphics& g, Gdiplus::RectF rect)
{
	// Simple metallic gradient for the pole
	LinearGradientBrush brush(rect, Color(50, 50, 50), Color(220, 220, 220), LinearGradientModeHorizontal);

	// Metallic shine blend
	REAL factors[] = { 0.2f, 1.0f, 0.4f, 0.8f, 0.2f };
	REAL positions[] = { 0.0f, 0.3f, 0.5f, 0.7f, 1.0f };
	brush.SetBlend(factors, positions, 5);

	g.FillRectangle(&brush, rect);
}

void CSignalTower::OnPaint()
{
	CPaintDC dc(this);
	Graphics g(dc.GetSafeHdc());
	g.SetSmoothingMode(SmoothingModeAntiAlias);

	CRect client;
	GetClientRect(&client);

	float w = (float)client.Width();
	float h = (float)client.Height();

	// Define Proportions
	// Cap: 5%, 4 Lights: 18% each, Pole: remaining
	float hCap = h * 0.05f;
	float hLight = h * 0.18f;

	// 1. Draw Top Cap (Grey)
	Draw3DLens(g, RectF(0, 0, w, hCap), Color(180, 180, 180), true);

	// 2. Draw Lights
	// Note: We define the "Base Color" here. The Draw3DLens function will 
	// automatically dim it for the OFF state and brighten it for the ON state.

	// RED
	Draw3DLens(g, RectF(0, hCap, w, hLight), Color(255, 0, 0), m_bRedOn);

	// ORANGE (RGB 255, 140, 0 is a nice deep amber)
	Draw3DLens(g, RectF(0, hCap + hLight, w, hLight), Color(255, 140, 0), m_bOrangeOn);

	// GREEN
	Draw3DLens(g, RectF(0, hCap + (hLight * 2), w, hLight), Color(0, 255, 0), m_bGreenOn);

	// BLUE
	Draw3DLens(g, RectF(0, hCap + (hLight * 3), w, hLight), Color(0, 120, 255), m_bBlueOn);

	// 3. Draw Pole Base
	float poleWidth = w * 0.4f;
	float poleX = (w - poleWidth) / 2;
	float poleY = hCap + (hLight * 4);
	DrawBase(g, RectF(poleX, poleY, poleWidth, h - poleY));
}