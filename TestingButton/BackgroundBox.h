#pragma once

class CBackgroundBox : public CStatic
{
	DECLARE_DYNAMIC(CBackgroundBox)

public:
	CBackgroundBox();
	virtual ~CBackgroundBox();

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};