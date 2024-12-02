#include <iostream>
#include "Players/PcPlayer.h"
#include "Screen/Menu.h"
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

    std::srand(std::time(nullptr));
    // the handle for the window, filled by a function
    HWND hWnd;
    // this struct holds information for the window class
    WNDCLASSEXW wc;

    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEXW));

    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    //wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass1";

    // register the window class
    RegisterClassExW(&wc);
    RECT wr = {0, 0, 1920, 1080};    // set the size, but not the position
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

    // create the window and use the result as the handle
    hWnd = CreateWindowExW(NULL,
                          L"WindowClass1",    // name of the window class
                          L"Our Direct3D Program",   // title of the window
                          WS_EX_TOPMOST | WS_POPUP, 
                          0,    // x-position of the window
                          0,    // y-position of the window
                          GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 
                          NULL,    // we have no parent window, NULL
                          NULL,    // we aren't using menus, NULL
                          hInstance,    // application handle
                          NULL);    // used with multiple windows, NULL



    // display the window on the screen
    ShowWindow(hWnd, nCmdShow);

    // enter the main loop:
    DXApp App(hWnd);
    App.changeScreen(std::make_unique<MenuScreen>());
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
                App.onWinClick(POINT{GET_X_LPARAM(msg.lParam),GET_Y_LPARAM(msg.lParam)}, Button::left);
            if(msg.message == WM_LBUTTONUP)
                App.onWinClickUp(Button::left);
            if(msg.message == WM_RBUTTONDOWN)
                App.onWinClick(POINT{GET_X_LPARAM(msg.lParam),GET_Y_LPARAM(msg.lParam)}, Button::right);
            if(msg.message == WM_RBUTTONUP)
                App.onWinClickUp(Button::right);
            if(msg.message == WM_CHAR )
                App.onWinChar(static_cast<wchar_t>(LOWORD(msg.wParam)));
        }
        else
        {
            App.renderFrame();
        }
    }

    // return this part of the WM_QUIT message to Windows
    return msg.wParam;
};