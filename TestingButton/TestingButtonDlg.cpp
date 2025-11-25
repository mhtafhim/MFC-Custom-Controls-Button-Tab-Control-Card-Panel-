
// TestingButtonDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "TestingButton.h"
#include "TestingButtonDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestingButtonDlg dialog



CTestingButtonDlg::CTestingButtonDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTINGBUTTON_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestingButtonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_TEST, test_button);
	DDX_Control(pDX, IDC_LIST1, tableCtrl);
	DDX_Control(pDX, IDC_TAB1, tabCtrl);
	DDX_Control(pDX, IDC_PANEL_VIEW, m_myPanel);
	DDX_Control(pDX, IDC_CARD_PANEL, m_cardCtrl);
	DDX_Control(pDX, IDC_CUSTOM_CARD, cardView);
}

BEGIN_MESSAGE_MAP(CTestingButtonDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_PANEL_VIEW, &CTestingButtonDlg::OnStnClickedPanelView)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestingButtonDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CTestingButtonDlg message handlers

BOOL CTestingButtonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	test_button.SetIconByID(IDR_MAINFRAME);
	//test_button.EnableWindowsTheming(FALSE);
	test_button.SetFaceColor(RGB(240, 240, 240));
	test_button.SetTextColor(RGB(0, 0, 0));

	//test_button.EnableWindowsTheming(TRUE);


	tabCtrl.InsertItem(0, _T("Operation"));

	tabCtrl.InsertItem(1, _T("Analysis"));

	tabCtrl.InsertItem(2, _T("Recipe"));


	//CStatic* pDummy1 = new CStatic();
	//pDummy1->Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), &tabCtrl);

	//tabCtrl.InsertTab(pDummy1, _T("Tab 1"));




	PopulateList();

	//tabCtrl.

	m_brushBack.CreateSolidBrush(RGB(244, 247, 249)); //235, 236, 237



	return TRUE;  // return TRUE  unless you set the focus to a control
}




void CTestingButtonDlg::PopulateList()
{

	tableCtrl.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);

	// Now you can set extended styles (Grid lines, etc.)
	tableCtrl.SetExtendedStyle(tableCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//m_table_ctrl.DeleteAllItems(); // Clear old data if refreshing
	tableCtrl.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 100);    // Column 0
	tableCtrl.InsertColumn(1, _T("Name"), LVCFMT_LEFT, 150);  // Column 1
	tableCtrl.InsertColumn(2, _T("Role"), LVCFMT_LEFT, 100);   // Column 2
	tableCtrl.InsertColumn(3, _T("Salary"), LVCFMT_LEFT, 100); // Column 3
	// --- SIMULATED DATABASE DATA ---
	struct Employee {
		CString id;
		CString name;
		CString role;
		CString salary;
	};

	Employee data[] = {
		{ _T("101"), _T("John Smith"), _T("Manager"), _T("$5000") },
		{ _T("102"), _T("Jane Doe"), _T("Engineer"), _T("$4000") },
		{ _T("103"), _T("Bob Brown"), _T("Intern"), _T("$1500") },
		{ _T("101"), _T("John Smith"), _T("Manager"), _T("$5000") },
		{ _T("102"), _T("Jane Doe"), _T("Engineer"), _T("$4000") },
		{ _T("103"), _T("Bob Brown"), _T("Intern"), _T("$1500") }
	};
	// -------------------------------

	// Loop through your data
	for (int i = 0; i < 6; i++)
	{
		// 1. Insert the Row (and set Column 0 text)
		// InsertItem returns the index of the new row
		int nIndex = tableCtrl.InsertItem(i, data[i].id);

		// 2. Set text for specific columns in that row
		tableCtrl.SetItemText(nIndex, 1, data[i].name);   // Column 1
		tableCtrl.SetItemText(nIndex, 2, data[i].role);   // Column 2
		tableCtrl.SetItemText(nIndex, 3, data[i].salary); // Column 3

		// Optional: Set row data (useful for retrieving DB ID later)
		// m_ListCtrl.SetItemData(nIndex, (DWORD_PTR)real_database_id);
	}
}






HBRUSH CTestingButtonDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_brushBack;
	}
	return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}









void CTestingButtonDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestingButtonDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestingButtonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestingButtonDlg::OnStnClickedPanelView()
{
	// TODO: Add your control notification handler code here
}

void CTestingButtonDlg::OnBnClickedButton3()
{
	AfxMessageBox(_T("Button 3 clicked"));
	// TODO: Add your control notification handler code here
}
