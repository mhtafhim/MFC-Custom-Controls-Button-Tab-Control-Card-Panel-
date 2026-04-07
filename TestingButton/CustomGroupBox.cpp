#include "pch.h" // Change to "stdafx.h" if using older Visual Studio
#include "CustomGroupBox.h"

using namespace Gdiplus;

IMPLEMENT_DYNAMIC(CCustomGroupBox, CStatic)

CCustomGroupBox::CCustomGroupBox()
{
}

CCustomGroupBox::~CCustomGroupBox()
{
}

BEGIN_MESSAGE_MAP(CCustomGroupBox, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CCustomGroupBox::PreSubclassWindow()
{
	// Remove standard borders so we can draw our custom GDI+ border
	ModifyStyle(WS_BORDER | SS_BLACKFRAME | SS_GRAYFRAME | SS_WHITEFRAME | SS_ETCHEDFRAME, 0);
	CStatic::PreSubclassWindow();
}

BOOL CCustomGroupBox::OnEraseBkgnd(CDC* pDC)
{
	return TRUE; // Prevent flickering; background is drawn in OnPaint
}

// Helper function to create the rounded path
void CCustomGroupBox::GetRoundedRectPath(GraphicsPath* path, RectF rect, int radius)
{
	float d = (float)(radius * 2);
	path->Reset();

	if (radius <= 0) {
		path->AddRectangle(rect);
		return;
	}

	path->AddArc(rect.X, rect.Y, d, d, 180, 90);
	path->AddArc(rect.X + rect.Width - d, rect.Y, d, d, 270, 90);
	path->AddArc(rect.X + rect.Width - d, rect.Y + rect.Height - d, d, d, 0, 90);
	path->AddArc(rect.X, rect.Y + rect.Height - d, d, d, 90, 90);
	path->CloseFigure();
}

void CCustomGroupBox::OnPaint()
{
	CPaintDC dc(this);
	CRect rectClient;
	GetClientRect(&rectClient);

	// 1. Double Buffering to prevent flicker
	CMemDC memDC(dc, rectClient);
	CDC& bufferDC = memDC.GetDC();

	// Fill the outside area with the standard dialog background color
	bufferDC.FillSolidRect(rectClient, RGB(225, 232, 237));

	// 2. Initialize GDI+ Graphics
	Graphics graphics(bufferDC.GetSafeHdc());
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);

	// 3. Define the Outer Box boundaries
	RectF rectPanel((float)rectClient.left, (float)rectClient.top, (float)rectClient.Width() - 1, (float)rectClient.Height() - 1);
	rectPanel.Inflate(-1, -1);
	int cornerRadius = 8; // Adjust roundness here

	GraphicsPath pathPanel;
	GetRoundedRectPath(&pathPanel, rectPanel, cornerRadius);

	// 4. Fill the INSIDE of the box (Light whitish-grey)
	SolidBrush brushPanelBg(Color(255, 245, 248, 250));
	graphics.FillPath(&brushPanelBg, &pathPanel);

	// 5. Draw the Outer Border (Dark Grey)
	Pen penBorder(Color(255, 110, 110, 110), 1.0f);
	graphics.DrawPath(&penBorder, &pathPanel);

	// 6. Draw Title and Line
	CString strTitle;
	GetWindowText(strTitle); // Pulls the text you set in the Resource Editor

	if (!strTitle.IsEmpty())
	{
		float titleHeight = 24.0f; // Height of the header area

		// USING ARIAL FONT
		FontFamily fontFamily(L"Arial");
		Gdiplus::Font fontTitle(&fontFamily, 10, FontStyleRegular, UnitPoint);
		SolidBrush brushText(Color(255, 0, 0, 0)); // Black text

		StringFormat format;
		format.SetAlignment(StringAlignmentCenter);
		format.SetLineAlignment(StringAlignmentCenter);

		RectF rectTitle = rectPanel;
		rectTitle.Height = titleHeight;

		// Draw the text
		graphics.DrawString(strTitle, -1, &fontTitle, rectTitle, &format, &brushText);

		// Draw Separator Line matching the border color
		float lineY = rectPanel.Y + titleHeight;
		Pen penLine(Color(255, 110, 110, 110), 1.0f);

		graphics.SetClip(&pathPanel);
		graphics.DrawLine(&penLine, rectPanel.X, lineY, rectPanel.X + rectPanel.Width, lineY);
		graphics.ResetClip();
	}
}