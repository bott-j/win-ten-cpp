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
// Include external header files
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>

// Include project header files
#include "cviewgdi.hpp"
#include "istate.hpp"

/// <summary>
///   Class constructor.
/// </summary>
/// <param name="hWnd">Handle of window for view.</param>
CViewGDI::CViewGDI(HWND hWnd)
    : m_hWnd(hWnd)
    , m_hBitmapBackground(0)
    , m_hBitmapBackgroundPrevious(0)
    , m_hBitmapDraw(0)
    , m_hBitmapDrawPrevious(0)
    , m_hdcBackground(0)
    , m_hdcBuffer(0)
    , m_needErase(true)
    , m_viewportHeight(0)
    , m_viewportWidth(0)
    , m_viewportXOffset(0)
    , m_viewportYOffset(0)
    , m_scaling(0)
{
    // Initialize GDI+.
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);

    // Draw field
    m_brushGreen.reset(new Gdiplus::SolidBrush((Gdiplus::Color(255, 0, 255, 0))));
    m_brushBlack.reset(new Gdiplus::SolidBrush((Gdiplus::Color(255, 0, 0, 0))));
    m_pointF.reset(new Gdiplus::PointF(30.0f, 10.0f));
    
    // Font will be scaled and recreated during resize
    m_fontFamily.reset(new Gdiplus::FontFamily(L"Times New Roman"));
}

/// <summary>
///     Class destructor.
/// </summary>
CViewGDI::~CViewGDI()
{
    // Destroy resources
    Gdiplus::GdiplusShutdown(m_gdiplusToken);
}

/// <summary>
///     Initialize the view.
/// </summary>
/// <param name="newXOffset">Margin from left in pixels.</param>
/// <param name="newYOffset">Margin from top in pixels.</param>
/// <param name="newWidth">Width of view in pixels.</param>
/// <param name="newHeight">Height of view in pixels.</param>
void CViewGDI::initialize(int newXOffset, int newYOffset, int newWidth, int newHeight) 
{
    // Lock access to GDI functions
    gdiUpdateMutex.lock();

    // If a margin exists need to erase the window 
    if (newXOffset > 0 || newYOffset > 0)
        m_needErase = true;
    HDC hdcWindow = GetWindowDC(m_hWnd);

    m_viewportWidth = newWidth;
    m_viewportHeight = newHeight;
    m_viewportXOffset = newXOffset;
    m_viewportYOffset = newYOffset;
    // Calculate scaling
    m_scaling = static_cast<float>(m_viewportWidth) / winten_constants::W;
    // Create a new device context
    m_hdcBuffer = CreateCompatibleDC(hdcWindow);
    // Create a new device context
    m_hdcBackground = CreateCompatibleDC(hdcWindow);
    // Create a compatible bitmap for drawing
    m_hBitmapDraw = CreateCompatibleBitmap(hdcWindow, newWidth, newHeight);
    // Create a compatible bitmap for drawing
    m_hBitmapBackground = CreateCompatibleBitmap(hdcWindow, newWidth, newHeight);
    // Select new bitmap into device context
    m_hBitmapDrawPrevious = (HBITMAP)SelectObject(m_hdcBuffer, m_hBitmapDraw);
    m_hBitmapBackgroundPrevious = (HBITMAP)SelectObject(m_hdcBackground, m_hBitmapBackground);
    // Create graphics object
    m_graphics.reset(new Gdiplus::Graphics(m_hdcBackground));

    // Draw filled rectangles
    m_graphics->FillRectangle(
        m_brushGreen.get(),
        0,
        0,
        newWidth,
        newHeight);
    m_graphics->FillRectangle(
        m_brushBlack.get(),
        winten_constants::FIELD_BORDER * m_scaling,
        winten_constants::FIELD_BORDER * m_scaling,
        (winten_constants::W - 2.0f * winten_constants::FIELD_BORDER) * m_scaling,
        (winten_constants::H - 2.0f * winten_constants::FIELD_BORDER) * m_scaling);

    // Reset graphics objects
    m_graphics.reset(new Gdiplus::Graphics(m_hdcBuffer));
    m_font.reset(
        new Gdiplus::Font(
            m_fontFamily.get(), 
            newWidth * 24.0f / 640.0f,
            Gdiplus::FontStyleRegular, 
            Gdiplus::UnitPixel));

    // Release window DC
    ReleaseDC(m_hWnd, hdcWindow);

    // Unlock mutex
    gdiUpdateMutex.unlock();
}

/// <summary>
///     Shutdown the view.
/// </summary>
void CViewGDI::shutdown(void) 
{
    // Lock access to GDI objects
    gdiUpdateMutex.lock();

    // Delete graphics object
    m_graphics.reset(nullptr);

    // Restore previous bitmap into device context
    SelectObject(m_hdcBuffer, m_hBitmapDrawPrevious);
    // Restore previous bitmap into device context
    SelectObject(m_hdcBackground, m_hBitmapBackgroundPrevious);
    // Delete drawing bitmap
    DeleteObject(m_hBitmapDraw);
    // Delete drawing bitmap
    DeleteObject(m_hBitmapBackground);
    // Delete the buffering device context
    DeleteDC(m_hdcBuffer);
    // Delete the buffering device context
    DeleteDC(m_hdcBackground);

    // Unlock mutex
    gdiUpdateMutex.unlock();
}

/// <summary>
///     Draws all objects in the view.
/// </summary>
/// <param name="state">State to draw.</param>
/// <param name="fps">Frames per second for display.</param>
/// <param name="latency">Latency for display.</param>
void CViewGDI::DrawAll(
    IState* state,
    float fps,
    float latency) 
{
    HDC hdc;

    // Attempt to get exclusive access to GDI resources
    if (gdiUpdateMutex.try_lock())
    {
        // If graphics object initialised
        if (m_graphics.get() != nullptr)
        {
            // Get window client area device context
            hdc = GetDC(m_hWnd);

            // Copy background layer to buffer
            BitBlt(m_hdcBuffer, 0, 0, m_viewportWidth, m_viewportHeight, m_hdcBackground, 0, 0, SRCCOPY);

            // Draw player
            m_graphics->FillRectangle(
                m_brushGreen.get(),
                (state->player.x - winten_constants::PADDLE_WIDTH / 2.0f) * m_scaling,
                (state->player.y - winten_constants::PADDLE_HEIGHT / 2.0f) * m_scaling,
                winten_constants::PADDLE_WIDTH * m_scaling,
                winten_constants::PADDLE_HEIGHT * m_scaling);

            // Draw NPC
            m_graphics->FillRectangle(
                m_brushGreen.get(),
                (state->npc.x - winten_constants::PADDLE_WIDTH / 2) * m_scaling,
                (state->npc.y - winten_constants::PADDLE_HEIGHT / 2) * m_scaling,
                winten_constants::PADDLE_WIDTH * m_scaling,
                winten_constants::PADDLE_HEIGHT * m_scaling);

            // Draw ball
            m_graphics->FillEllipse(
                m_brushGreen.get(),
                (state->ball.x - winten_constants::BALL_DIAMETER / 2) * m_scaling,
                (state->ball.y - winten_constants::BALL_DIAMETER / 2) * m_scaling,
                winten_constants::BALL_DIAMETER * m_scaling,
                winten_constants::BALL_DIAMETER * m_scaling);

            // Convert numeric values to string 
            std::wostringstream timetext;
            timetext << L"FPS: " << std::setprecision(4) << fps;
            std::wostringstream time2text;
            time2text << L"LAT: " << std::setprecision(4) << latency;
            std::wostringstream scoreNpcText;
            scoreNpcText << state->scoreNpc;
            std::wostringstream scorePlayerText;
            scorePlayerText << state->scorePlayer;
            std::wostringstream messageText;
            messageText << state->message.c_str();

            // Draw text in window
            m_graphics->DrawString(
                scoreNpcText.str().c_str(),
                -1,
                m_font.get(),
                Gdiplus::PointF(40.0f, 10.0f),
                m_brushGreen.get());
            m_graphics->DrawString(
                timetext.str().c_str(),
                -1,
                m_font.get(),
                Gdiplus::PointF(300.0f, 10.0f),
                m_brushGreen.get());
            m_graphics->DrawString(
                time2text.str().c_str(),
                -1,
                m_font.get(),
                Gdiplus::PointF(300.0f, 40.0f),
                m_brushGreen.get());
            m_graphics->DrawString(
                scorePlayerText.str().c_str(),
                -1,
                m_font.get(),
                Gdiplus::PointF(560.0f, 10.0f),
                m_brushGreen.get());
            m_graphics->DrawString(
                messageText.str().c_str(),
                -1,
                m_font.get(),
                Gdiplus::PointF(300.0f, 150.0f),
                m_brushGreen.get());

            // Copy the buffer hdc to the window hdc
            if (m_needErase)
            {
                RECT rc;
                HBRUSH brushErase = CreateSolidBrush(RGB(0, 0, 0));

                GetClientRect(m_hWnd, &rc);
                FillRect(hdc, &rc, brushErase);
                DeleteObject(brushErase);
                m_needErase = false;
            }

            // Copy back buffer to window client area
            BitBlt(hdc, m_viewportXOffset, m_viewportYOffset, m_viewportWidth, m_viewportHeight, m_hdcBuffer, 0, 0, SRCCOPY);

            // Release the device context
            ReleaseDC(m_hWnd, hdc);
        }

        // Unlock mutex
        gdiUpdateMutex.unlock();
    }
}
