#pragma once


class CCardPanelWithPictureControl : public CStatic
{
public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();

private:
    // Helper to create a rounded path
    void GetRoundedPath(Gdiplus::GraphicsPath* path, Gdiplus::RectF rect, float radius);

    // Helper to draw a specific shadow layer
    void DrawRoundedShadow(Gdiplus::Graphics* g, Gdiplus::RectF rect, float radius, float inflate, float offsetY, Gdiplus::Color color);
};

