#include "pch.h"
#include "CardPanelWithPictureControl.h"
#include <gdiplus.h>

using namespace Gdiplus;

BEGIN_MESSAGE_MAP(CCardPanelWithPictureControl, CStatic)
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CCardPanelWithPictureControl::OnPaint()
{
    CPaintDC dc(this);

    // 1. Double Buffering to prevent flickering
    CMemDC memDC(dc, this);
    CDC* pDC = &memDC.GetDC();

    // Get Control Size
    CRect rectClient;
    GetClientRect(&rectClient);

    // 2. Paint Background 
    // IMPORTANT: Match this RGB to your Main Dialog's background color!
    // If your dialog is the light blue from the image, use that color here.
    pDC->FillSolidRect(rectClient, RGB(240, 244, 248));

    // Initialize GDI+
    Graphics graphics(pDC->GetSafeHdc());
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    // 3. Define the "White Card" dimensions
    // We shrink the box so the shadow fits inside the control area
    float margin = 8.0f; // Space for shadow
    RectF rectWhite(
        (REAL)rectClient.left + margin,
        (REAL)rectClient.top + margin,
        (REAL)rectClient.Width() - (margin * 2),
        (REAL)rectClient.Height() - (margin * 2)
    );

    float cornerRadius = 10.0f;

    // =======================================================
    // 4. DRAW SHADOWS (The "Onion Skin" method)
    // =======================================================
    // We draw multiple rounded layers with low opacity to fake a blur.

    // Layer 1: Wide, very transparent (The outer glow)
    // Inflate: 4px, OffsetY: 4px, Opacity: 10
    DrawRoundedShadow(&graphics, rectWhite, cornerRadius, 4.0f, 4.0f, Color(10, 0, 0, 0));

    // Layer 2: Medium width (The middle blur)
    // Inflate: 2px, OffsetY: 2px, Opacity: 20
    DrawRoundedShadow(&graphics, rectWhite, cornerRadius, 2.0f, 2.0f, Color(20, 0, 0, 0));

    // Layer 3: Tight, darker (The core shadow)
    // Inflate: 0px, OffsetY: 2px, Opacity: 40
    DrawRoundedShadow(&graphics, rectWhite, cornerRadius, 0.0f, 2.0f, Color(40, 0, 0, 0));


    // =======================================================
    // 5. DRAW THE MAIN WHITE BOX
    // =======================================================
    GraphicsPath path;
    GetRoundedPath(&path, rectWhite, cornerRadius);

    // Fill White
    SolidBrush brushWhite(Color(255, 255, 255, 255));
    graphics.FillPath(&brushWhite, &path);

    // Draw subtle border
    Pen penBorder(Color(255, 220, 220, 220), 1.0f);
    graphics.DrawPath(&penBorder, &path);
}

// =======================================================================
// HELPER: Draws a single rounded shadow layer
// =======================================================================
void CCardPanelWithPictureControl::DrawRoundedShadow(Graphics* g, RectF rect, float radius, float inflate, float offsetY, Color color)
{
    // 1. Expand the rectangle (Inflate)
    rect.Inflate(inflate, inflate);

    // 2. Move it down (Offset)
    rect.Offset(0, offsetY);

    // 3. CRITICAL: Increase radius by the inflation amount.
    // If we don't do this, the corners look sharp/weird.
    float newRadius = radius + inflate;

    // 4. Create path and fill
    GraphicsPath path;
    GetRoundedPath(&path, rect, newRadius);

    SolidBrush brush(color);
    g->FillPath(&brush, &path);
}

// =======================================================================
// HELPER: Creates the rounded rectangle geometry
// =======================================================================
void CCardPanelWithPictureControl::GetRoundedPath(GraphicsPath* path, RectF rect, float radius)
{
    float d = radius * 2.0f;

    // Ensure radius isn't too big for the rect
    if (d > rect.Width) d = rect.Width;
    if (d > rect.Height) d = rect.Height;

    path->AddArc(rect.X, rect.Y, d, d, 180, 90); // Top Left
    path->AddArc(rect.X + rect.Width - d, rect.Y, d, d, 270, 90); // Top Right
    path->AddArc(rect.X + rect.Width - d, rect.Y + rect.Height - d, d, d, 0, 90); // Bottom Right
    path->AddArc(rect.X, rect.Y + rect.Height - d, d, d, 90, 90); // Bottom Left
    path->CloseFigure();
}