#pragma once
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

class CCustomGroupBox : public CStatic
{
	DECLARE_DYNAMIC(CCustomGroupBox)

public:
	CCustomGroupBox();
	virtual ~CCustomGroupBox();

protected:
	virtual void PreSubclassWindow();

	// Message map functions
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

private:
	// GDI+ Helper to draw the rounded corners
	void GetRoundedRectPath(Gdiplus::GraphicsPath* path, Gdiplus::RectF rect, int radius);
};