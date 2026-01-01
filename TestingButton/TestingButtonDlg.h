
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
#include "RoundStatic.h"
#include "DiceMap.h"

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





	double m_dWaferDiameter=300;
	double m_dGapX = 1.0;
	double m_dGapY = 1.0;
	PointD pTL = { 32.58,121.255 };
	PointD pBL = { 32.24, 88.965 };
	PointD pBR = { 56.629, 88.699 };
	//PointD pTL = { 0 , 0 };
	//PointD pBL = { 0 , -5 };
	//PointD pBR = {5 , -5 };

	//PointD pTL = { -2 , 4 };
	//PointD pBL = { -4 , 2 };
	//PointD pBR = { -2 , 0 };


private: 
	UINT m_nSelectedToolID; // Holds the ID of the currently active button

public:
	CRoundStatic m_roundStatic;
	CDiceMap m_diceMap;
	afx_msg void OnBnClickedButtonTest2();
	CButton m_maskCheck;
	afx_msg void OnBnClickedMaskCheck();
	
	afx_msg void OnBnClickedButton4();
	void OnBnClickedReadData();
	double GetDoubleFromEdit(int nID);
	void SetEditVal(int nID, double val);
	void InitEditControls();
};
