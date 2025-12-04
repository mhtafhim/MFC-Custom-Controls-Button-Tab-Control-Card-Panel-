#include "pch.h"
#include "GradientButton.h"

IMPLEMENT_DYNAMIC(CGradientButton, CMFCButton)

CGradientButton::CGradientButton()
{
	m_colTop = RGB(255, 255, 255);
	m_colBottom = RGB(235, 235, 235);
	m_colBorder = RGB(200, 200, 200);
	m_colHoverBorder = RGB(150, 180, 220);
	m_colText = RGB(60, 100, 160);

	m_hIcon = NULL;
	m_iconSize = CSize(0, 0);

	m_bTransparent = FALSE;
	m_bDrawFocus = FALSE;
}

CGradientButton::~CGradientButton()
{
	// Clean up icon if loaded
	if (m_hIcon) ::DestroyIcon(m_hIcon);
}

void CGradientButton::SetIconByID(UINT nIconID, int width, int height)
{
	// Clean up previous icon if exists
	if (m_hIcon) ::DestroyIcon(m_hIcon);

	m_iconSize = CSize(28, 28);

	m_hIcon = (HICON)::LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(nIconID),
		IMAGE_ICON,
		width,
		height,
		LR_DEFAULTCOLOR
	);

	// Trigger a repaint so the icon shows up immediately
	Invalidate();
	UpdateWindow();
}

void CGradientButton::SetGradientColors(COLORREF top, COLORREF bottom, COLORREF border)
{
	m_colTop = top;
	m_colBottom = bottom;
	m_colBorder = border;
}

void CGradientButton::SetTextColor(COLORREF color)
{
	m_colText = color;
}

BEGIN_MESSAGE_MAP(CGradientButton, CMFCButton)
END_MESSAGE_MAP()

void CGradientButton::OnDraw(CDC* pDC, const CRect& rect, UINT uiState)
{
	CRect rc = rect;

	BOOL bPressed = (uiState & ODS_SELECTED);
	BOOL bHot = IsHighlighted();
	BOOL bDisabled = (uiState & ODS_DISABLED);

	COLORREF cTop = m_colTop;
	COLORREF cBot = m_colBottom;
	COLORREF cBorder = bHot ? m_colHoverBorder : m_colBorder;
	COLORREF cText = bDisabled ? GetSysColor(COLOR_GRAYTEXT) : m_colText;

	if (bPressed)
	{
		cTop = RGB(220, 220, 220);
		cBot = RGB(235, 235, 235);
	}

	// 1. Draw Background Gradient
	int nRadius = 6;
	CRgn rgn;
	rgn.CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, nRadius, nRadius);

	pDC->SaveDC();
	pDC->SelectClipRgn(&rgn);

	TRIVERTEX vert[2];
	GRADIENT_RECT gRect;

	vert[0].x = rc.left; vert[0].y = rc.top;
	vert[0].Red = GetRValue(cTop) << 8;
	vert[0].Green = GetGValue(cTop) << 8;
	vert[0].Blue = GetBValue(cTop) << 8;
	vert[0].Alpha = 0x0000;

	vert[1].x = rc.right; vert[1].y = rc.bottom;
	vert[1].Red = GetRValue(cBot) << 8;
	vert[1].Green = GetGValue(cBot) << 8;
	vert[1].Blue = GetBValue(cBot) << 8;
	vert[1].Alpha = 0x0000;

	gRect.UpperLeft = 0; gRect.LowerRight = 1;
	GradientFill(pDC->GetSafeHdc(), vert, 2, &gRect, 1, GRADIENT_FILL_RECT_V);

	pDC->RestoreDC(-1);

	// 2. Draw Border
	CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	CPen pen(PS_SOLID, 1, cBorder);
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->RoundRect(rc.left, rc.top, rc.right, rc.bottom, nRadius, nRadius);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	// 3. Prepare Text and Icon Layout
	CString strText;
	GetWindowText(strText);
	CFont* pFont = GetFont();
	CFont* pOldFont = pDC->SelectObject(pFont);

	CSize sizeText = pDC->GetTextExtent(strText);

	// Calculate total width of content (Icon + Spacing + Text)
	int nIconPadding = 0;
	int nTotalWidth = sizeText.cx;
	if (m_hIcon) nTotalWidth += m_iconSize.cx + nIconPadding;

	// Calculate starting X position to center the group
	int nStartX = rc.left + (rc.Width() - nTotalWidth) / 2;
	int nCentery = rc.top + (rc.Height() / 2);

	// 4. Draw Icon (if exists)
	if (m_hIcon)
	{
		int nIconY = nCentery - (m_iconSize.cy / 2);

		// If button is pressed, shift icon down-right 1px for effect (optional)
		if (bPressed) { nStartX++; nIconY++; }

		DrawIconEx(pDC->GetSafeHdc(), nStartX, nIconY, m_hIcon,
			m_iconSize.cx, m_iconSize.cy, 0, NULL, DI_NORMAL);

		// Move X forward for the text
		nStartX += m_iconSize.cx + nIconPadding;
	}

	// 5. Draw Text
	CRect rcText = rc;
	rcText.left = nStartX; // Start text after icon

	if (bPressed) rcText.OffsetRect(1, 1); // Shift text if pressed

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(cText);

	// Draw text vertically centered, but use our calculated Left position
	pDC->DrawText(strText, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	pDC->SelectObject(pOldFont);
}