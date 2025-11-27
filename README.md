# TestingButton (MFC Custom Controls Demo)

A sample MFC (Microsoft Foundation Classes) desktop application demonstrating several custom UI controls and modernized visual styles. This repository contains custom button controls, modern/flat tab controls, a card-style panel, list and header customizations, and a demo dialog that ties them together.

---

## Table of Contents
- About
- Features
- Key Components
- Quick Start (Build & Run)
- How to Use the Controls (Examples)
- Troubleshooting & Notes
- Contributing
- License

---

## About

This is a sample project that demonstrates custom MFC controls implemented using owner-draw logic and GDI / GDI+ for styling. The UI elements are modernized versions of commonly used controls (rounded color buttons, icon buttons, flat/modern tabs, a card-like panel with shadows, and stylized headers and list controls).

Purpose: Showcase how to implement visually modern controls in MFC and provide reusable components for others to integrate in their MFC projects.

---

## Features

- Custom Color Button (rounded, hover/press effects, custom font) — `CColorButton`.
- Icon Button (CMFCButton wrapper for icon resources) — `CIconButton`.
- Card Panel (rounded rect, GDI+ drawing, shadow effects) — `CCardPanel`.
- Picture Card Control (static control variant with rounded corners and shadow) — `CCardPanelWithPictureControl`.
- Flat & Modern Tab Controls (owner-draw tabs with underline hover/active state) — `CFlatTabCtrl`, `CModernTabCtrl`.
- Custom Header Control (custom draw header with colors) — `CMyHeaderCtrl`.
- Custom List Control (zebra stripe rows, per-column styling) — `CMyListCtrl`.

---

## Key Components (Files)

- `ColorButton.*` — Rounded color buttons with hover and pressed state.
- `IconButton.*` — Wraps `CMFCButton` to simplify setting an icon by ID and other common options.
- `CardPanel.*` — A custom window class for displaying a card-like panel with rounded corners and non-client hit testing for easy placement.
- `CardPanelWithPictureControl.*` — A control that draws an image inside a rounded card with drop shadow using GDI+.
- `FlatTabCtrl.*`, `ModernTabCtrl.*` — Examples of owner-drawn tabs with modern visuals (centered labels, hover color, underline indicator).
- `MyHeaderCtrl.*`, `MyListCtrl.*` — Custom draw implementations used for headings and list styling.
- `TestingButtonDlg.*` — Demo dialog that showcases the controls and contains example usages.

---

## Quick Start (Build & Run) ✅

Requirements:

- Microsoft Visual Studio (2019, 2022, or newer) with the "Desktop development with C++" workload and MFC support.
- Windows (tested on Windows 10+)

Build and run steps:

1. Open the Visual Studio solution: `TestingButton.slnx`.
2. Build the solution (choose `x64` + `Debug` or `Release` if needed).
3. Run the application from Visual Studio or open the `TestingButton.exe` from `x64/Debug/`.

Notes:

- GDI+ is used for some drawing logic (`Gdiplus::GdiplusStartup()` and shutdown are already present in `TestingButton.cpp`), so no further steps should be required when building with Visual Studio.
- For proper theming and visual styles, the app uses the `CMFCVisualManagerWindows` class to enable native appearance for standard MFC controls.

---

## How to Use the Controls (Examples) 💡

The demo dialog (`TestingButtonDlg`) shows how to use these components, but here are quick code examples:

- Color button (set colors and text font):

```cpp
test_button.SetColor(RGB(34, 139, 34), RGB(50, 205, 50), RGB(255, 255, 255));
test_button.SetTextProperties(14, true, _T("Arial"));
```

- Icon button (set icon by resource ID):

```cpp
upButton.SetIconByID(IDI_ICON9);
leftIcon.SetIconByID(IDI_ICON7);
```

- Modern Tab control (insert tabs and customize height):

```cpp
tabCtrl.InsertItem(0, _T("Operation"));
tabCtrl.InsertItem(1, _T("Analysis"));
tabCtrl.InsertItem(2, _T("Recipe"));
// tabCtrl.SetTabHeight(48);
```

- List control (insert columns and items, uses zebra-striping in custom draw):

```cpp
tableCtrl.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
tableCtrl.SetExtendedStyle(tableCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
tableCtrl.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 100);
tableCtrl.InsertColumn(1, _T("Name"), LVCFMT_LEFT, 150);
tableCtrl.InsertItem(0, _T("101")); // Insert a row
tableCtrl.SetItemText(0, 1, _T("John Smith"));
```

- Card Panel (if registering as a window class):

```cpp
// Ensure this is called once in your application initialization (InitInstance):
CCardPanel::RegisterWindowClass();
```

---

## Troubleshooting & Notes ⚠️

- If build errors arise referencing missing MFC headers, verify you installed the MFC option in the Visual Studio Workloads.
- GDI+ initialization/shutdown is required: `GdiplusStartup()` and `GdiplusShutdown()` are present in `TestingButton.cpp`, and must exist for GDI+ helper functions to work properly.
- If icons do not appear, confirm that the correct resource IDs are used and that resources exist in `res/` or `icon/` folders.
- If a control appears to flicker, it may be due to paint invalidation; the code uses double buffering in some controls (e.g., `CModernTabCtrl::OnPaint`) and `OnEraseBkgnd` handling to prevent flicker.

---

## Contributing ✨

Contributions, bug reports, and feature requests are welcome. Suggested practices:

- Create PRs with descriptive titles and test the UI change in both Debug and Release builds.
- For new controls, keep owner-draw logic isolated and maintain consistent naming and resource usage.
- Add short usage examples in `TestingButtonDlg` when you add a new feature so others can find it.

---

## License

This repository does not contain an explicit license. Please contact the repository owner (`mhtafhim`) if you plan to reuse the code or include a license file (e.g., `LICENSE` with MIT/Apache-2.0) before distributing.

---

## Contact

Author: `mhtafhim` (GitHub) — For questions or contributions, open an issue or PR.

---

Thanks for checking out the project — drop a star if you found it useful! ⭐
# TestingButton