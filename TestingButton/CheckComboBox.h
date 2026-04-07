#pragma once
#include <afxwin.h>
#include <afxtempl.h>

class CCheckComboBox : public CComboBox
{
    DECLARE_DYNAMIC(CCheckComboBox)

public:
    CCheckComboBox();
    virtual ~CCheckComboBox();

    // Check/Uncheck an item
    void SetCheck(int nIndex, BOOL bCheck);
    BOOL GetCheck(int nIndex) const;

    // Gets a comma-separated string of all checked items (e.g., "Apple, Banana")
    CString GetCheckedItemsText() const;

    // Set checked items by passing a comma-separated string
    void SetCheckedItemsFromText(const CString& strText);

    // Overrides to keep our check-state array in sync with the strings
    virtual int AddString(LPCTSTR lpszString);
    virtual int InsertString(int nIndex, LPCTSTR lpszString);
    virtual int DeleteString(int nIndex);
    virtual void ResetContent();

protected:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    virtual void PreSubclassWindow();

    // The array keeping track of true/false for each item
    CArray<BOOL, BOOL> m_CheckArray;

    // Handle to the internal dropdown listbox
    HWND m_hListBox;

    // Subclass WindowProc to intercept clicks in the dropdown
    static LRESULT CALLBACK ComboListProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

    DECLARE_MESSAGE_MAP()
    afx_msg LRESULT OnCtlColorListBox(WPARAM wParam, LPARAM lParam);
    afx_msg void OnDestroy();
};