
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
	DDX_Control(pDX, IDC_BUTTON_TOP_LEFT, topLeftButton);
	DDX_Control(pDX, IDC_BUTTON_UP, upButton);
	DDX_Control(pDX, IDC_BUTTON_TOP_RIGHT, topRiightButton);
	DDX_Control(pDX, IDC_BUTTON_LEFT, leftIcon);
	DDX_Control(pDX, IDC_BUTTON_RIGHT, rightButton);
	DDX_Control(pDX, IDC_BUTTON_BOTTOM_LEFT, bottomLeftButton);
	DDX_Control(pDX, IDC_BUTTON_DOWN, downButton);
	DDX_Control(pDX, IDC_BUTTON_BOTTOM_RIGHT, bottomRightButton);
	DDX_Control(pDX, IDC_BUTTON_TEST2, test_button_2);
	DDX_Control(pDX, IDC_LIGHT_TOWER, m_ctrLightTower);
	DDX_Control(pDX, IDC_SIGNALTOWER, signalTower);
	DDX_Control(pDX, IDC_DEMO_TEXT, demoText);
	//DDX_Control(pDX, IDC_LENS_OBJECTIVE, m_lensCtrl);
	DDX_Control(pDX, IDC_LENS_CTRL, m_lensCtrl);
	DDX_Control(pDX, IDC_SLIDER1, m_modernCtrl);
	DDX_Control(pDX, IDC_BUTTON_TEST3, m_gradientButton);
	DDX_Control(pDX, IDC_RADIO1, m_radioButton);
}

BEGIN_MESSAGE_MAP(CTestingButtonDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_PANEL_VIEW, &CTestingButtonDlg::OnStnClickedPanelView)
	//ON_BN_CLICKED(IDC_BUTTON3, &CTestingButtonDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CTestingButtonDlg::OnBnClickedButtonTest)

	ON_WM_TIMER()

	ON_STN_CLICKED(IDC_LENS_CTRL, &CTestingButtonDlg::OnLensSelectionChanged)



	//ON_BN_CLICKED(IDC_BUTTON_BOTTOM_LEFT, &CTestingButtonDlg::OnBnClickedButtonBottomLeft)
	//ON_BN_CLICKED(IDC_BUTTON_DOWN, &CTestingButtonDlg::OnBnClickedButtonDown)
	//ON_BN_CLICKED(IDC_BUTTON_BOTTOM_RIGHT, &CTestingButtonDlg::OnBnClickedButtonBottomRight)
	//ON_BN_CLICKED(IDC_BUTTON5, &CTestingButtonDlg::OnBnClickedButton5)
	//ON_BN_CLICKED(IDC_BUTTON_RIGHT, &CTestingButtonDlg::OnBnClickedButtonRight)

	// Map your specific IDs to the single logic function
	ON_COMMAND_EX(IDC_BUTTON_TOP_LEFT, OnToolButtonClicked)
	ON_COMMAND_EX(IDC_BUTTON_UP, OnToolButtonClicked)
	ON_COMMAND_EX(IDC_BUTTON_TOP_RIGHT, OnToolButtonClicked)
	ON_COMMAND_EX(IDC_BUTTON_LEFT, OnToolButtonClicked)
	ON_COMMAND_EX(IDC_BUTTON_RIGHT, OnToolButtonClicked)
	ON_COMMAND_EX(IDC_BUTTON_BOTTOM_LEFT, OnToolButtonClicked)
	ON_COMMAND_EX(IDC_BUTTON_DOWN, OnToolButtonClicked)
	ON_COMMAND_EX(IDC_BUTTON_BOTTOM_RIGHT, OnToolButtonClicked)


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

//	test_button.SetIconByID(IDR_MAINFRAME);
	//test_button.EnableWindowsTheming(FALSE);
//	test_button.SetFaceColor(RGB(240, 240, 240));
//	test_button.SetTextColor(RGB(0, 0, 0));

	//test_button.EnableWindowsTheming(TRUE);


	tabCtrl.InsertItem(0, _T("Operation"));

	tabCtrl.InsertItem(1, _T("Analysis"));

	tabCtrl.InsertItem(2, _T("Recipe"));


	//CStatic* pDummy1 = new CStatic();
	//pDummy1->Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), &tabCtrl);

	//tabCtrl.InsertTab(pDummy1, _T("Tab 1"));

	



	PopulateList();

	//tabCtrl.

	m_brushBack.CreateSolidBrush(RGB(230, 255, 255)); //235, 236, 237


	setButtonIcon();


	test_button.SetColor(RGB(34, 139, 34), RGB(50, 205, 50), RGB(255, 255, 255));
	test_button.SetTextProperties(14, true, _T("Arial")); 


	test_button_2.SetIconByID(IDI_ICON1);





	demoText.SetFontOptions(10,false);


	m_lensCtrl.AddLens(_T("4x\nMinor"));
	m_lensCtrl.AddLens(_T("10x"));
	m_lensCtrl.AddLens(_T("20x"));
	m_lensCtrl.AddLens(_T("40x"));
	m_lensCtrl.AddLens(_T("100x"));


	m_gradientButton.SetIconByID(IDI_ICON9, 30);



	UpdateTimeLabel();       // Show time immediately
	SetTimer(1, 1000, NULL);
 



	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CTestingButtonDlg::setButtonIcon()
{
	 //topLeftButton.SetIconByID(IDI_ICON_TOP_LEFT);
	 upButton.SetIconByID(IDI_ICON9);
	// topRiightButton.SetIconByID(IDI_ICON_TOP_RIGHT);
	 leftIcon.SetIconByID(IDI_ICON7);
	 rightButton.SetIconByID(IDI_ICON8);
	// bottomLeftButton.SetIconByID(IDI_ICON_BUTTON_LEFT);
	 downButton.SetIconByID(IDI_ICON6);
	// bottomRightButton.SetIconByID(IDI_ICON_BOTTOM_RIGHT);



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



void CTestingButtonDlg::UpdateTimeLabel()
{
	CTime currentTime = CTime::GetCurrentTime();
	CString strTime = currentTime.Format(_T(" %m/%d/%Y \n %I:%M:%S %p"));

	// Ensure the ID here matches your Resource ID exactly
	if (GetDlgItem(IDC_DATE_TIME) != NULL)
	{
		SetDlgItemText(IDC_DATE_TIME, strTime);
	}
}


void CTestingButtonDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		UpdateTimeLabel();
	}
	CDialogEx::OnTimer(nIDEvent);
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
	signalTower.SetLightState(CSignalTower::ORANGE, true);

	//AfxMessageBox(_T("Button 3 clicked"));
	m_ctrLightTower.SetMode(TOWER_AMBER);
	// TODO: Add your control notification handler code here
}

void CTestingButtonDlg::OnBnClickedButtonTest()
{
	// TODO: Add your control notification handler code here
}

void CTestingButtonDlg::OnBnClickedButtonBottomLeft()
{
	signalTower.SetLightState(CSignalTower::RED, true);


	m_ctrLightTower.SetMode(TOWER_RED);

	// TODO: Add your control notification handler code here
}

void CTestingButtonDlg::OnBnClickedButtonDown()
{

	//downButton.SetBackgroundColor(RGB(0, 0, 0));
	signalTower.SetLightState(CSignalTower::BLUE, true);

	m_ctrLightTower.SetMode(TOWER_BLUE);

	// TODO: Add your control notification handler code here
}

void CTestingButtonDlg::OnBnClickedButtonBottomRight()
{
	
	m_ctrLightTower.SetMode(TOWER_AMBER);
	// TODO: Add your control notification handler code here
}

void CTestingButtonDlg::OnBnClickedButton5()
{


	// TODO: Add your control notification handler code here
	m_ctrLightTower.SetMode(TOWER_OFF);
}

void CTestingButtonDlg::OnBnClickedButtonRight()
{


	signalTower.SetLightState(CSignalTower::GREEN, true);

	m_ctrLightTower.SetMode(TOWER_GREEN);

	// TODO: Add your control notification handler code here
}



void CTestingButtonDlg::OnLensSelectionChanged()
{
	// 1. Ask the control which lens is currently selected
	int nIndex = m_lensCtrl.GetSelectedIndex();

	// 2. Define variables to store settings based on the lens
	double dZoom = 1.0;
	CString strName;

	// 3. Logic based on index (0 is the first lens, 1 is the second, etc.)
	switch (nIndex)
	{
	case 0: // 4x
		dZoom = 4.0;
		strName = _T("Scanning (4x)");
		// Example: m_Camera.SetZoom(4.0);
		break;

	case 1: // 10x
		dZoom = 10.0;
		strName = _T("Low Power (10x)");
		break;

	case 2: // 20x
		dZoom = 20.0;
		strName = _T("Medium Power (20x)");
		break;

	case 3: // 40x
		dZoom = 40.0;
		strName = _T("High Dry (40x)");
		break;

	case 4: // 100x
		dZoom = 100.0;
		strName = _T("Oil Immersion (100x)");
		break;

	default:
		return;
	}

	// 4. Test it: Update the Window Title or show a message
	CString strOutput;
	strOutput.Format(_T("Lens Changed: %s - Zoom Factor: %.1f"), strName, dZoom);

	// Update the Dialog Title Bar
	//SetWindowText(strOutput);
	AfxMessageBox(strOutput);

	// Or print to Visual Studio Output window
	TRACE(_T("%s\n"), strOutput);
}

// Adjust these RGB values to match your UI design
const COLORREF CLR_NORMAL_BACK = RGB(240, 240, 240); // Standard Grey/White
const COLORREF CLR_NORMAL_HOVER = RGB(220, 220, 220); // Slightly darker when hovering
const COLORREF CLR_SELECTED = RGB(173, 216, 230); // Light Blue for Active Tool

BOOL CTestingButtonDlg::OnToolButtonClicked(UINT nID)
{
	// 1. NON-CLICKABLE LOGIC:
	// If clicking the button that is ALREADY selected, ignore it.
	if (nID == m_nSelectedToolID)
		return TRUE;

	// 2. RESET THE OLD BUTTON (Deselect)
	// We cast GetDlgItem to your custom class to access your custom functions
	if (m_nSelectedToolID != 0)
	{
		CIconButton* pOldBtn = (CIconButton*)GetDlgItem(m_nSelectedToolID);
		if (pOldBtn)
		{
			// Restore normal background
			pOldBtn->SetBackgroundColor(CLR_NORMAL_BACK);
			// Restore normal hover effect
			pOldBtn->SetHoverColor(CLR_NORMAL_HOVER);
		}
	}

	// 3. SETUP THE NEW BUTTON (Select)
	CIconButton* pNewBtn = (CIconButton*)GetDlgItem(nID);
	if (pNewBtn)
	{
		// Set the "Selected" background
		pNewBtn->SetBackgroundColor(CLR_SELECTED);

		// DISABLE HOVER:
		// Set hover color same as background so it looks like hover is disabled
		pNewBtn->SetHoverColor(CLR_SELECTED);
		AfxMessageBox(_T("Tool Selected"));
	}

	// 4. Update the tracking variable
	m_nSelectedToolID = nID;

	// 5. Handle Tool Logic
	// switch(nID) { ... }

	return TRUE; // Stop further processing
}