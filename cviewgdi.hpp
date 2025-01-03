/*
* Copyright (C) 2024 James Bott
*
* This program is free software: you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free Software 
* Foundation, version 3.
*
* This program is distributed in the hope that it will be useful, but WITHOUT 
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
* FOR A PARTICULAR PURPOSE. See the GNU General Public License for more 
* details.
*
* You should have received a copy of the GNU General Public License along with 
* this program. If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef WINTEN_CVIEWGDI_HPP
#define WINTEN_CVIEWGDI_HPP

// Include external header files
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <iomanip>
#include <memory>
#include <mutex>
#include <sstream>

// Include project header files
#include "istate.hpp"
#include "iview.hpp"
#include "winten_constants.hpp"

/// <summary>
///     Class implements GDI+ view under windows.
/// </summary>
class CViewGDI : public IView
{
private:
    // Handle of owning window
    HWND m_hWnd;
    // Window resources allocated
    HDC m_hdcBuffer;
    HDC m_hdcBackground;
    HBITMAP m_hBitmapDrawPrevious;
    HBITMAP m_hBitmapDraw;
    HBITMAP m_hBitmapBackground;
    HBITMAP m_hBitmapBackgroundPrevious;
    // GDI resources allocated
    std::unique_ptr<Gdiplus::Graphics> m_graphics;
    std::unique_ptr<Gdiplus::SolidBrush> m_brushGreen;
    std::unique_ptr<Gdiplus::SolidBrush> m_brushBlack;
    std::unique_ptr<Gdiplus::FontFamily> m_fontFamily;
    std::unique_ptr<Gdiplus::Font> m_font;
    Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
    ULONG_PTR           m_gdiplusToken;
    // Internal state
    int m_viewportHeight;
    int m_viewportWidth;
    int m_viewportXOffset;
    int m_viewportYOffset;
    float m_scaling;
    bool m_needErase;
    std::mutex gdiUpdateMutex;

public:
    CViewGDI(HWND hWnd);
    ~CViewGDI();
    void initialize(int newXOffset, int newYOffset, int newWidth, int newHeight) override;
    void shutdown(void) override;
    void DrawAll(
        IState* state,
        float fps,
        float latency) override;
};

#endif