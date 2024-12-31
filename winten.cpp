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

// Include external headers
//#include "targetver.h"
// Windows Header Files
#define WIN32_LEAN_AND_MEAN // Use reduced feature set
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory>
#include <tchar.h>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <deque>
#include <mmsystem.h>

// Include project headers
#include "contextcontroller.hpp"
#include "cstatedemo.hpp"
#include "cviewgdi.hpp"
#include "winten.h"
#include "winten_constants.hpp"

// Link multimedia and GDI+ libraries
#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib,"winmm.lib")

#define MAX_LOADSTRING 100
#define TIMER_ID 1

//std::deque<long double> counts(100, 0);

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
HWND                InitInstance(HINSTANCE, int, ContextController*);

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
VOID CALLBACK gameTimer(UINT      uTimerID, UINT      uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    ContextController controller; // Controller context object in local scope

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINTEN, szWindowClass, MAX_LOADSTRING);

    // Register window class
    MyRegisterClass(hInstance);

    // Perform application initialization:
    HWND hWnd = InitInstance(hInstance, nCmdShow, &controller);
    if (!hWnd)
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINTEN));

    // Create initial state and register with context
    controller.transitionTo(
        std::move(
            std::make_unique<CStateDemo>()
        )
    );  

    // Set a timer for world update and rendering
    timeSetEvent(
        15, 
        0, 
        gameTimer, 
        (DWORD_PTR) & controller, 
        TIME_PERIODIC);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINTEN));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(HOLLOW_BRUSH);// (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINTEN);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    wcex.cbWndExtra = sizeof(LONG_PTR);

    return RegisterClassExW(&wcex);
}

//VOID CALLBACK gameTimer(HWND hWnd, UINT ByVal, UINT_PTR id, DWORD dwTime)
VOID CALLBACK gameTimer(UINT      uTimerID, UINT      uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    ContextController *controller = (ContextController*)dwUser;
    unsigned __int64 freq;
    unsigned __int64 thisTime, timeAfterUpdate;

    // Get frequency and counter ticks
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&thisTime);

    // Update the world and render
    controller->update(static_cast<long double>(thisTime) / static_cast<long double>(freq));
            
    QueryPerformanceCounter((LARGE_INTEGER*)&timeAfterUpdate);
    controller->setAfterUpdateTime(static_cast<long double>(timeAfterUpdate) / static_cast<long double>(freq));

}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow, ContextController *controller)
{
    HWND hWnd;
    hInst = hInstance; // Store instance handle in our global variable

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, controller);

    if (!hWnd)
    {
        return NULL;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return hWnd;
}




//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ContextController* controller;
    controller = (ContextController*)GetWindowLongPtr(hWnd, 0);
 
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_ERASEBKGND:
        return 1;
    case WM_SIZING:
    {
        // Pause the game
        //world.pause(true);
    }
    return TRUE;
    case WM_SIZE:
    {
        int width = lParam & 0x0000FFFF;
        int height = (lParam & 0xFFFF0000) >> 16;
        int newHeight;
        int newWidth;
        int newXOffset;
        int newYOffset;

        // Depeding on resize code
        switch (wParam)
        {
            //Maximized 
        case SIZE_RESTORED:
            // Restored
        case SIZE_MAXIMIZED:
        {
            if (static_cast<float>(width) / static_cast<float>(height) > winten_constants::ASPECT_RATIO)
            {
                newHeight = height;
                newWidth = static_cast<int>(winten_constants::ASPECT_RATIO * height);
                newXOffset = (width - newWidth) / 2;
                newYOffset = 0;
            }
            else
            {
                newHeight = static_cast<int>(width / winten_constants::ASPECT_RATIO);
                newWidth = width;
                newXOffset = 0;
                newYOffset = (height - newHeight) / 2;
            }

            // Resize the display
            controller->shutdown();
            controller->initialize(newXOffset, newYOffset, newWidth, newHeight);
            //world.pause(false);
        }
        break;
        case SIZE_MINIMIZED:
            //world.pause(true);
            break;
        }
    }
    return 0;
    case WM_PAINT:

        return 1;
        break;
    case WM_DESTROY:
        // Stop timers
        timeKillEvent(TIMER_ID);

        controller->shutdown();

        PostQuitMessage(0);
        break;
    case WM_CREATE:
    {
        CREATESTRUCT* create = (CREATESTRUCT*)lParam;
        controller = (ContextController*)create->lpCreateParams;
        
        // Store controller pointer for later use in window procedure
        SetWindowLongPtr(hWnd, 0, (LONG_PTR)controller);
        
        // Create the view and register with controller
        std::unique_ptr<IView> pView(new CViewGDI(hWnd));
        controller->setView(std::move(pView));
        controller->initialize(0, 0, create->cx, create->cy);
    }

    break;
    case WM_KEYUP:
        switch (wParam)
        {
        case VK_DOWN:
            controller->keyDown(false);
            break;
        case VK_UP:
            controller->keyUp(false);
            break;
        case VK_ESCAPE:
            controller->keyEscape(false);
        }
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_DOWN:
            controller->keyDown(true);
            break;
        case VK_UP:
            controller->keyUp(true);
            break;
        case VK_ESCAPE:
            controller->keyEscape(true);
        }
        controller->keyPressed();
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
