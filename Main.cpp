#pragma once
#include <iostream>
#include "Players/PcPlayer.h"
#include "DXApp.h"
#include <windowsx.h>

// the WindowProc function prototype
// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch(message)
    {
        // this message is read when the window is closed
        case WM_DESTROY:
                // close the application entirely
                PostQuitMessage(0);
                return 0;
        
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc (hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);\

    // the handle for the window, filled by a function
    HWND hWnd;
    // this struct holds information for the window class
    WNDCLASSEX wc;

    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    //wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = "WindowClass1";

    // register the window class
    RegisterClassEx(&wc);
    RECT wr = {0, 0, 1920, 1080};    // set the size, but not the position
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

    // create the window and use the result as the handle
    hWnd = CreateWindowEx(NULL,
                          "WindowClass1",    // name of the window class
                          "Our Direct3D Program",   // title of the window
                          WS_EX_TOPMOST | WS_POPUP, 
                          0,    // x-position of the window
                          0,    // y-position of the window
                          SCREEN_WIDTH, SCREEN_HEIGHT, 
                          NULL,    // we have no parent window, NULL
                          NULL,    // we aren't using menus, NULL
                          hInstance,    // application handle
                          NULL);    // used with multiple windows, NULL



    // display the window on the screen
    ShowWindow(hWnd, nCmdShow);

    // enter the main loop:
    DXApp App = DXApp(hWnd);
    // this struct holds Windows event messages
    MSG msg = {0};

    // wait for the next message in the queue, store the result in 'msg'
    while(TRUE)
    {
        // Check to see if any messages are waiting in the queue
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // translate keystroke messages into the right format
            TranslateMessage(&msg);

            // send the message to the WindowProc function
            DispatchMessage(&msg);

            // check to see if it's time to quit
            if(msg.message == WM_QUIT)
                break;
            // check to see if it's time to quit
            if(msg.message == WM_LBUTTONDOWN)
            {
                
                D2D_SIZE_U pt;
                pt.width = GET_X_LPARAM(msg.lParam);
                pt.height = GET_Y_LPARAM(msg.lParam);
                
                App.OnClick(pt);
            }
            if(msg.message == WM_LBUTTONUP)
                App.OnClickUp();
        }
        else
        {
            App.RenderFrame();
        }
    }

    // return this part of the WM_QUIT message to Windows
    return msg.wParam;
}
