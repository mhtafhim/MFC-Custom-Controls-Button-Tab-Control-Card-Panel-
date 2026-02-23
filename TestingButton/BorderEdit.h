#pragma once

class CBorderEdit : public CEdit
{
	DECLARE_DYNAMIC(CBorderEdit)

public:
	CBorderEdit();
	virtual ~CBorderEdit();

	// Call this to change the border color (defaults to a standard gray)
	void SetBorderColor(COLORREF color);

protected:
	COLORREF m_borderColor;

	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnNcPaint();
};