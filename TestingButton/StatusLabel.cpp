#include "pch.h"
#include "TestingButton.h" // Change to your app's main header
#include "StatusLabel.h"

IMPLEMENT_DYNAMIC(CStatusLabel, CStatic)

CStatusLabel::CStatusLabel()
{
	m_currentState = LightState::Off;

	// Default background color matching the box you made earlier
	m_bgColor = RGB(232, 236, 241);
}

CStatusLabel::~CStatusLabel() {}

BEGIN_MESSAGE_MAP(CStatusLabel, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_SETTEXT, &CStatusLabel::OnSetText)
END_MESSAGE_MAP()

void CStatusLabel::SetState(LightState state)
{
	if (m_currentState != state)
	{
		m_currentState = state;
		Invalidate(); // Trigger a redraw
		UpdateWindow();
	}
}

void CStatusLabel::SetBackgroundColor(COLORREF color)
{
	m_bgColor = color;
	Invalidate();
}

// Prevent flickering
BOOL CStatusLabel::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

// Intercept when text is changed via SetWindowText so we redraw properly
LRESULT CStatusLabel::OnSetText(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = Default();
	Invalidate();
	UpdateWindow();
	return lRes;
}

void CStatusLabel::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);

	// 1. Fill background to match your custom box
	dc.FillSolidRect(rect, m_bgColor);

	// 2. Setup circle size and position
	int circleDiameter = 14; // Size of the light
	int centerY = rect.Height() / 2;

	CRect circleRect(
		rect.left,
		centerY - (circleDiameter / 2),
		rect.left + circleDiameter,
		centerY + (circleDiameter / 2)
	);

	// 3. Determine color
	COLORREF colorToDraw;
	switch (m_currentState)
	{
	case LightState::Red:    colorToDraw = RGB(190, 10, 10);   break;
	case LightState::Green:  colorToDraw = RGB(24, 115, 40);   break;
	case LightState::Yellow: colorToDraw = RGB(218, 165, 32);  break;
	case LightState::Off:
	default:                 colorToDraw = RGB(160, 160, 160); break;
	}

	// 4. Draw the circle
	CBrush brush(colorToDraw);
	CPen pen(PS_SOLID, 1, colorToDraw);
	CBrush* pOldBrush = dc.SelectObject(&brush);
	CPen* pOldPen = dc.SelectObject(&pen);

	dc.Ellipse(circleRect);

	// 5. Setup text position (shifted right by circle size + 8 pixels padding)
	CRect textRect = rect;
	textRect.left = circleRect.right + 8;

	// 6. Get the text from the control and draw it
	CString strText;
	GetWindowText(strText);

	if (!strText.IsEmpty())
	{
		// Use the font assigned in the dialog editor
		CFont* pFont = GetFont();
		CFont* pOldFont = nullptr;
		if (pFont) pOldFont = dc.SelectObject(pFont);

		dc.SetTextColor(RGB(0, 0, 0)); // Black text
		dc.SetBkMode(TRANSPARENT);

		// Draw text vertically centered, aligned left
		dc.DrawText(strText, textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		if (pOldFont) dc.SelectObject(pOldFont);
	}

	// 7. Cleanup
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
}

// [NEW] Implementation of SetText
void CStatusLabel::SetText(CString strText)
{
	// SetWindowText is a standard CWnd function that updates the internal text buffer
	SetWindowText(strText);

	// Force a redraw immediately so the new text appears instantly
	Invalidate();
	UpdateWindow();
}

void CStatusLabel::SetLabelFont(int nFontSize, BOOL bBold, CString strFaceName)
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));

	// 1. Get the current font's properties as a base
	CFont* pCurrentFont = GetFont();
	if (pCurrentFont)
	{
		pCurrentFont->GetLogFont(&lf);
	}
	else
	{
		// Fallback to a standard font if none exists yet
		CFont* pDefFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
		pDefFont->GetLogFont(&lf);
	}

	// 2. Apply Custom FaceName if provided
	if (!strFaceName.IsEmpty())
	{
		_tcscpy_s(lf.lfFaceName, LF_FACESIZE, strFaceName);
	}

	// 3. Apply Boldness
	lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;

	// 4. Calculate Height from Point Size
	// We need a device context to convert Points to Logical Pixels
	CClientDC dc(this);
	lf.lfHeight = -MulDiv(nFontSize, dc.GetDeviceCaps(LOGPIXELSY), 72);

	// 5. Delete old font object and create the new one
	m_font.DeleteObject();
	if (m_font.CreateFontIndirect(&lf))
	{
		// 6. Tell the window to use this new font
		// Your existing OnPaint calls GetFont(), so it will pick this up automatically.
		SetFont(&m_font);

		Invalidate();
		UpdateWindow();
	}
}

void CStatusLabel::PreSubclassWindow()
{
	// Call the base class version first
	CStatic::PreSubclassWindow();

	// Set default font: Size 10, Bold = TRUE, Font Face = "" (uses default/current face)
	SetLabelFont(10, FALSE, _T(""));
}