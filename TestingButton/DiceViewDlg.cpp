// DiceViewDlg.cpp : implementation file
//

#include "pch.h"
#include "TestingButton.h"
#include "afxdialogex.h"
#include "DiceViewDlg.h"


// CDiceViewDlg dialog

IMPLEMENT_DYNAMIC(CDiceViewDlg, CDialogEx)

CDiceViewDlg::CDiceViewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DICEVIEW_DLG, pParent)
{

}

CDiceViewDlg::~CDiceViewDlg()
{
}

void CDiceViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DICE, m_diceMap2);
}


BEGIN_MESSAGE_MAP(CDiceViewDlg, CDialogEx)
END_MESSAGE_MAP()


// CDiceViewDlg message handlers


BOOL CDiceViewDlg::OnInitDialog()
{
	m_diceMap2.SetWaferParams(500.0, 5.0, 5.0);

	// 2. Set Reference Die Points (TL, BL, BR)
	PointD pTL = { 10, 40 };
	PointD pBL = { 10, 10 };
	PointD pBR = { 50, 10 };

	//PointD pTL = { 0, 0 };
	//PointD pBL = { -10, 0 };
	//PointD pBR = { -10, 10 };

	m_diceMap2.SetReferenceDie(pTL, pBL, pBR);


	m_diceMap2.SetShowPartialDies(true);


	return TRUE;
}
