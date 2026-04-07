#include "pch.h" // Change to "stdafx.h" if using an older version of Visual Studio
#include "CheckComboBox.h"
#include <CommCtrl.h>

#pragma comment(lib, "comctl32.lib")

IMPLEMENT_DYNAMIC(CCheckComboBox, CComboBox)

CCheckComboBox::CCheckComboBox() : m_hListBox(NULL)
{
}

CCheckComboBox::~CCheckComboBox()
{
}

BEGIN_MESSAGE_MAP(CCheckComboBox, CComboBox)
    ON_MESSAGE(WM_CTLCOLORLISTBOX, OnCtlColorListBox)
    ON_WM_DESTROY()
END_MESSAGE_MAP()

void CCheckComboBox::PreSubclassWindow()
{
    CComboBox::PreSubclassWindow();
    ModifyStyle(WS_BORDER, 0); // Remove the standard border
    ModifyStyleEx(WS_EX_CLIENTEDGE, 0); // Remove the 3D edge
    ModifyStyle(0, CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS);

}

int CCheckComboBox::AddString(LPCTSTR lpszString)
{
    int nIndex = CComboBox::AddString(lpszString);
    if (nIndex >= 0) m_CheckArray.InsertAt(nIndex, FALSE);
    return nIndex;
}

int CCheckComboBox::InsertString(int nIndex, LPCTSTR lpszString)
{
    int nRet = CComboBox::InsertString(nIndex, lpszString);
    if (nRet >= 0) m_CheckArray.InsertAt(nRet, FALSE);
    return nRet;
}

int CCheckComboBox::DeleteString(int nIndex)
{
    int nRet = CComboBox::DeleteString(nIndex);
    if (nRet >= 0) m_CheckArray.RemoveAt(nIndex);
    return nRet;
}

void CCheckComboBox::ResetContent()
{
    CComboBox::ResetContent();
    m_CheckArray.RemoveAll();
}

void CCheckComboBox::SetCheck(int nIndex, BOOL bCheck)
{
    if (nIndex >= 0 && nIndex < m_CheckArray.GetSize()) {
        m_CheckArray[nIndex] = bCheck;
        Invalidate(FALSE); // Force redraw of the closed combobox to update text
    }
}

BOOL CCheckComboBox::GetCheck(int nIndex) const
{
    if (nIndex >= 0 && nIndex < m_CheckArray.GetSize())
        return m_CheckArray[nIndex];
    return FALSE;
}

CString CCheckComboBox::GetCheckedItemsText() const
{
    CString strResult;
    for (int i = 0; i < GetCount(); i++) {
        if (GetCheck(i)) {
            CString strText;
            GetLBText(i, strText);
            if (!strResult.IsEmpty()) strResult += _T(", ");
            strResult += strText;
        }
    }

    // OPTIONAL: If nothing is checked, return a placeholder
    // if (strResult.IsEmpty()) return _T("(None Selected)");

    return strResult;
}

void CCheckComboBox::SetCheckedItemsFromText(const CString& strText)
{
    for (int i = 0; i < GetCount(); i++) SetCheck(i, FALSE); // Clear all

    int nPos = 0;
    CString strToken = strText.Tokenize(_T(","), nPos);
    while (!strToken.IsEmpty()) {
        strToken.Trim();
        int nIndex = FindStringExact(-1, strToken);
        if (nIndex >= 0) SetCheck(nIndex, TRUE);
        strToken = strText.Tokenize(_T(","), nPos);
    }
    Invalidate(FALSE);
}

void CCheckComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    lpMeasureItemStruct->itemHeight = 18; // Adjust height if needed
}

void CCheckComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    if (lpDrawItemStruct->itemID == (UINT)-1) return;

    CDC dc;
    dc.Attach(lpDrawItemStruct->hDC);
    CRect rcItem = lpDrawItemStruct->rcItem;

    // Is Windows asking us to draw the closed edit box portion?
    BOOL bEdit = (lpDrawItemStruct->itemState & ODS_COMBOBOXEDIT) != 0;

    if (bEdit) {
        // Draw the combined string (e.g. "Item 1, Item 3")
        dc.FillSolidRect(&rcItem, ::GetSysColor(COLOR_WINDOW));
        dc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
        rcItem.left += 2;
        dc.DrawText(GetCheckedItemsText(), &rcItem, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
    }
    else {
        // We are drawing an item inside the dropdown list
        int nIndex = lpDrawItemStruct->itemID;
        BOOL bSelected = (lpDrawItemStruct->itemState & ODS_SELECTED) != 0;

        dc.FillSolidRect(&rcItem, bSelected ? ::GetSysColor(COLOR_HIGHLIGHT) : ::GetSysColor(COLOR_WINDOW));
        dc.SetTextColor(bSelected ? ::GetSysColor(COLOR_HIGHLIGHTTEXT) : ::GetSysColor(COLOR_WINDOWTEXT));

        // Draw Checkbox
        CRect rcCheck = rcItem;
        rcCheck.right = rcCheck.left + rcCheck.Height();
        rcCheck.DeflateRect(1, 1);
        UINT uState = DFCS_BUTTONCHECK | (GetCheck(nIndex) ? DFCS_CHECKED : 0);
        dc.DrawFrameControl(&rcCheck, DFC_BUTTON, uState);

        // Draw Text
        CString strText;
        GetLBText(nIndex, strText);
        CRect rcText = rcItem;
        rcText.left = rcCheck.right + 2;
        dc.DrawText(strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }
    dc.Detach();
}

// This catches the creation of the dropdown list so we can subclass it
LRESULT CCheckComboBox::OnCtlColorListBox(WPARAM wParam, LPARAM lParam)
{
    if (m_hListBox == NULL) {
        m_hListBox = (HWND)lParam;
        // Subclass the listbox to intercept mouse clicks
        SetWindowSubclass(m_hListBox, ComboListProc, 1, (DWORD_PTR)this);
    }
    return DefWindowProc(WM_CTLCOLORLISTBOX, wParam, lParam);
}

void CCheckComboBox::OnDestroy()
{
    if (m_hListBox != NULL) {
        RemoveWindowSubclass(m_hListBox, ComboListProc, 1);
        m_hListBox = NULL;
    }
    CComboBox::OnDestroy();
}

// This intercepts clicks so the dropdown DOES NOT close when an item is clicked
LRESULT CALLBACK CCheckComboBox::ComboListProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    CCheckComboBox* pCombo = (CCheckComboBox*)dwRefData;

    if (uMsg == WM_LBUTTONDOWN) {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };

        // ADDED :: HERE to call the global Win32 API
        LRESULT lr = ::SendMessage(hWnd, LB_ITEMFROMPOINT, 0, MAKELPARAM(pt.x, pt.y));

        if (!HIWORD(lr)) { // Clicked on a valid item
            int nIndex = LOWORD(lr);
            pCombo->SetCheck(nIndex, !pCombo->GetCheck(nIndex)); // Toggle state

            // ADDED :: HERE to call the global Win32 API
            ::InvalidateRect(hWnd, NULL, FALSE); // Force redraw of the listbox

            pCombo->Invalidate(FALSE); // Update parent combo text (Valid because we use pCombo->)
        }
        return 0; // Return 0 to EAT the click. This stops the combobox from closing!
    }
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}