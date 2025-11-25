#pragma once
#include <afxbutton.h>


class CIconButton : public CMFCButton
{
public:
	CIconButton();
	virtual ~CIconButton();
	void SetIconByID(UINT nIconID);

protected:
	virtual void PreSubclassWindow();



	DECLARE_MESSAGE_MAP()
};

