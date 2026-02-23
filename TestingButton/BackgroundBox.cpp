#include "pch.h"
#include "TestingButton.h" // Change to your app's header
#include "BackgroundBox.h"

IMPLEMENT_DYNAMIC(CBackgroundBox, CStatic)

CBackgroundBox::CBackgroundBox() {}
CBackgroundBox::~CBackgroundBox() {}

BEGIN_MESSAGE_MAP(CBackgroundBox, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CBackgroundBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);

	// --- 1. Define Colors ---
	COLORREF clrBackground = RGB(232, 236, 241); // Light grayish-blue
	COLORREF clrBorder = RGB(160, 160, 160); // Dark Gray
	COLORREF clrTitleText = RGB(0, 112, 192);   // Blue

	// --- 2. Draw the Rounded Rectangle ---
	CBrush bgBrush(clrBackground);
	CPen borderPen(PS_SOLID, 1, clrBorder);

	CBrush* pOldBrush = dc.SelectObject(&bgBrush);
	CPen* pOldPen = dc.SelectObject(&borderPen);

	// Draw rounded rectangle (10 is the corner radius)
	dc.RoundRect(rect.left, rect.top, rect.right, rect.bottom, 10, 10);

	// --- 3. Draw Title Text ---
	CRect titleRect = rect;
	titleRect.bottom = titleRect.top + 30; // Height of the title section

	CFont titleFont;
	titleFont.CreateFont(
		22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	CFont* pOldFont = dc.SelectObject(&titleFont);
	dc.SetTextColor(clrTitleText);
	dc.SetBkMode(TRANSPARENT);

	dc.DrawText(_T("FSM RAFT"), titleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// --- 4. Draw Horizontal Separator Line ---
	dc.MoveTo(rect.left, titleRect.bottom);
	dc.LineTo(rect.right, titleRect.bottom);

	// --- 5. Draw Vertical Separator Lines ---
	int yStart = titleRect.bottom + 10; // Top of the short lines
	int yEnd = rect.bottom - 10;        // Bottom of the short lines
	int width = rect.Width();

	// NEW: Short line in the middle of the LEFT box
	// Adjust 0.18 slightly if you need to move it left or right
	int leftShortLineX = rect.left + (int)(width * 0.18);
	dc.MoveTo(leftShortLineX, yStart);
	dc.LineTo(leftShortLineX, yEnd);

	// Vertical Line 1 (Main Middle Divider)
	// Notice this one goes from titleRect.bottom to rect.bottom (full height)
	int centerLineX = rect.left + (int)(width * 0.48);
	dc.MoveTo(centerLineX, titleRect.bottom);
	dc.LineTo(centerLineX, rect.bottom);

	// Vertical Line 2 (Short line in the RIGHT box)
	int rightShortLine1X = rect.left + (int)(width * 0.65);
	dc.MoveTo(rightShortLine1X, yStart);
	dc.LineTo(rightShortLine1X, yEnd);

	// Vertical Line 3 (Short line in the RIGHT box)
	int rightShortLine2X = rect.left + (int)(width * 0.82);
	dc.MoveTo(rightShortLine2X, yStart);
	dc.LineTo(rightShortLine2X, yEnd);

	// --- Cleanup GDI Objects ---
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldFont);
}