
// TestingButtonDlg.h : header file
//

#pragma once
#include "IconButton.h"
#include "MyListCtrl.h"
#include "CardPanel.h"
#include "FlatTabCtrl.h"
#include "ModernTabCtrl.h"
#include "ColorButton.h"

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
};
