#pragma once

class CStatusLabel : public CStatic
{
	DECLARE_DYNAMIC(CStatusLabel)

public:
	// Define the available colors
	enum class LightState {
		Off,
		Red,
		Green,
		Yellow
	};

	CStatusLabel();
	virtual ~CStatusLabel();

	// Function to change the light color from your code
	void SetState(LightState state);

	// Function to match the background of your custom box
	void SetBackgroundColor(COLORREF color);

	// [NEW] Function to change the text from code
	void SetText(CString strText);

	// [NEW] Function to change font style
// nFontSize: Size in points (e.g., 10, 12)
// bBold: TRUE for bold, FALSE for normal
// strFaceName: Optional font family (e.g., "Arial"). Pass "" to keep current.
	void SetLabelFont(int nFontSize, BOOL bBold, CString strFaceName = _T(""));

	virtual void PreSubclassWindow();


protected:
	LightState m_currentState;
	COLORREF   m_bgColor;

	CFont      m_font;


	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};