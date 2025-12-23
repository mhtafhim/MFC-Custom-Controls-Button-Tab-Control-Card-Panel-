#pragma once
#include "afxdialogex.h"
#include "DiceMap.h"


// CDiceViewDlg dialog

class CDiceViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDiceViewDlg)

public:
	CDiceViewDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDiceViewDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DICEVIEW_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CDiceMap m_diceMap;
};
