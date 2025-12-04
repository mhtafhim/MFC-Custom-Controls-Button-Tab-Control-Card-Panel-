#pragma once
#include <afxbutton.h>

class CGradientButton : public CMFCButton
{
	DECLARE_DYNAMIC(CGradientButton)

public:
	CGradientButton();
	virtual ~CGradientButton();

	void SetGradientColors(COLORREF top, COLORREF bottom, COLORREF border);
	void SetTextColor(COLORREF color);

	// Updated Function Signature
	void SetIconByID(UINT nIconID, int width = 16, int height = 16);

protected:
	COLORREF m_colTop;
	COLORREF m_colBottom;
	COLORREF m_colBorder;
	COLORREF m_colText;
	COLORREF m_colHoverBorder;

	// New members to store icon info
	HICON m_hIcon;
	CSize m_iconSize;

	virtual void OnDraw(CDC* pDC, const CRect& rect, UINT uiState) override;

	DECLARE_MESSAGE_MAP()
};