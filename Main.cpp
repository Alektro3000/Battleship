#include <iostream>
#include "Players/PcPlayer.h"
#include "Widget/Menu.h"
#include "DXApp.h"
#include <fstream>
#include <windowsx.h>

extern DXApp *AppPtr;

// the WindowProc function prototype
// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch (message)
    {
    // this message is read when the window is closed
    case WM_DESTROY:
        // close the application entirely
        Context::getInstance().clear();
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        if (AppPtr)
            AppPtr->onWinResize(LOWORD(lParam), HIWORD(lParam));
        break;
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int main(HINSTANCE hInstance, int nCmdShow)
{
 SetProcessDPIAware();
    HWND hWnd;
    WNDCLASSEXW wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEXW));

    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"BattleshipClass";

    // register the window class
    RegisterClassExW(&wc);
    RECT wr = {0, 0, 800, 600};                        // set the size, but not the position
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE); // adjust the size

    // create the window and use the result as the handle
    hWnd = CreateWindowEx(NULL,
                          L"BattleshipClass",  // name of the window class
                          L"BattleShip",       // title of the window
                          //WS_EX_TOPMOST | WS_POPUP, 
                          WS_OVERLAPPEDWINDOW,
                          0,                   // x-position of the window
                          0,                   // y-position of the window
                          //GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
                          wr.right-wr.left, wr.bottom-wr.top,
                          NULL,      // we have no parent window, NULL
                          NULL,      // we aren't using menus, NULL
                          hInstance, // application handle
                          NULL);     // used with multiple windows, NULL

    L"BattleshipClass", // name of the window class

    // display the window on the widget
    ShowWindow(hWnd, nCmdShow);

    // enter the main loop:
    DXApp   App(hWnd);
    App.pushWidget(std::make_unique<widget::Menu>(), false);
    AppPtr = &App;
    // this struct holds Windows event messages
    MSG msg = {0};

    // wait for the next message in the queue, store the result in 'msg'
    while (true)
    {
        // Check to see if any messages are waiting in the queue
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // translate keystroke messages into the right format
            TranslateMessage(&msg);

            // send the message to the WindowProc function
            DispatchMessage(&msg);

            // check to see if it's time to quit
            if (msg.message == WM_QUIT)
                break;
            // check to see if it's time to quit
            if (msg.message == WM_LBUTTONDOWN)
                App.onWinClick(POINT{GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam)}, MouseButton::left);
            if (msg.message == WM_LBUTTONUP)
                App.onWinClickUp(MouseButton::left);
            if (msg.message == WM_RBUTTONDOWN)
                App.onWinClick(POINT{GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam)}, MouseButton::right);
            if (msg.message == WM_RBUTTONUP)
                App.onWinClickUp(MouseButton::right);
            if (msg.message == WM_CHAR)
                App.onWinChar(static_cast<WCHAR>(LOWORD(msg.wParam)));
        }
        else
        {
            App.renderFrame();
        }
    }

    // return this part of the WM_QUIT message to Windows
    return msg.wParam;
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{main(hInstance, nCmdShow);
   try
   {
    //main(hInstance, nCmdShow);
   }
   catch(const std::exception& e)
   {
    std::ofstream outError;
    outError.open("log.txt");
    outError << e.what() << '\n';
   }
   
};
