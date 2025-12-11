
// TestingButtonDlg.h : header file
//

#pragma once
#include "IconButton.h"
#include "MyListCtrl.h"
#include "CardPanel.h"
#include "FlatTabCtrl.h"
#include "ModernTabCtrl.h"
#include "ColorButton.h"
#include "LightTower.h"
#include "BottomTabCtrl.h"
#include "SignalTower.h"
#include "StyleStatic.h"
#include "ObjectiveLensCtrl.h"
#include "ColorSliderCtrl.h"
#include "GradientButton.h"
#include "CustomRadioButton.h"

// CTestingButtonDlg dialog
class CTestingButtonDlg : public CDialogEx
{
// Construction
public:
	CTestingButtonDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTINGBUTTON_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	void setButtonIcon();
	void PopulateList();
	void UpdateTimeLabel();
	void OnTimer(UINT_PTR nIDEvent);
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CColorButton test_button;

	CMyListCtrl tableCtrl;


	CModernTabCtrl tabCtrl;


	CBrush m_brushBack;
	CStatic m_myPanel;


	afx_msg void OnStnClickedPanelView();
	CCardPanel m_cardCtrl;
	afx_msg void OnBnClickedButton3();
	CCardPanel cardView;
	CIconButton topLeftButton;
	CIconButton upButton;
	CIconButton topRiightButton;
	CIconButton leftIcon;
	CIconButton rightButton;
	CIconButton bottomLeftButton;
	CIconButton downButton;
	CIconButton bottomRightButton;
	afx_msg void OnBnClickedButtonTest();
	CIconButton test_button_2;

	CLightTower m_ctrLightTower;
	afx_msg void OnBnClickedButtonBottomLeft();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedButtonBottomRight();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButtonRight();
	afx_msg void OnLensSelectionChanged();

	BOOL OnToolButtonClicked(UINT nID);


	CSignalTower signalTower;
	CStyleStatic demoText;
//	CObjectiveLensCtrl m_lensCtrl;
	CObjectiveLensCtrl m_lensCtrl;
	CColorSliderCtrl m_modernCtrl;
	CGradientButton m_gradientButton;
	CCustomRadioButton m_radioButton;



private: 
	UINT m_nSelectedToolID; // Holds the ID of the currently active button

};
