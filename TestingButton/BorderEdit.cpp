#include "pch.h"      // Change to "stdafx.h" if using an older version of Visual Studio
#include "BorderEdit.h"
#include <uxtheme.h>

#pragma comment(lib, "uxtheme.lib")

IMPLEMENT_DYNAMIC(CBorderEdit, CEdit)

CBorderEdit::CBorderEdit()
{
	// Set default border color to a standard flat gray (matches your image)
	m_borderColor = RGB(171, 173, 179);
}

CBorderEdit::~CBorderEdit()
{
}

void CBorderEdit::SetBorderColor(COLORREF color)
{
	m_borderColor = color;
	// Force the non-client area (the border) to redraw immediately
	RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
}

void CBorderEdit::PreSubclassWindow()
{
	CEdit::PreSubclassWindow();

	// 1. Strip the modern Windows theme (this fixes the "bottom line only" bug)
	SetWindowTheme(GetSafeHwnd(), L"", L"");

	// 2. Force the control to have a standard border space, but remove the 3D sunken look
	ModifyStyle(0, WS_BORDER, SWP_FRAMECHANGED);
	ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);

	// 3. Add 4 pixels of padding to the left and right so text doesn't touch the border
	SetMargins(4, 4);
}

BEGIN_MESSAGE_MAP(CBorderEdit, CEdit)
	ON_WM_NCPAINT()
END_MESSAGE_MAP()

// This is where the manual border drawing happens
void CBorderEdit::OnNcPaint()
{
	// Let Windows do its default drawing first
	Default();

	// Get a Device Context for the ENTIRE window (including the border area)
	CWindowDC dc(this);

	// Get the size of the control
	CRect rect;
	GetWindowRect(&rect);
	rect.OffsetRect(-rect.left, -rect.top); // Shift coordinates to start at 0,0

	// Draw our custom 4-sided flat border over whatever Windows just drew
	CBrush borderBrush(m_borderColor);
	dc.FrameRect(&rect, &borderBrush);
}