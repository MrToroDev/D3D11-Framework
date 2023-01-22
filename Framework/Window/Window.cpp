#include "Window.h"
#include "../Utils/Utils.h"

int DX::Window::width = 0;
int DX::Window::height = 0;

DX::Window::Window(HINSTANCE hInstance, const char* title, int width, int height, WindowDataInfo data)
    : _instance(hInstance)
{
	const wchar_t CLASS_NAME[] = L"_FRAMEWORK_WINDOW_CLASS";
    WNDCLASSEXW winClass = {};
    winClass.cbSize = sizeof(WNDCLASSEXW);
    winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    winClass.lpfnWndProc = &WindowProc;
    winClass.hInstance = hInstance;
    winClass.hIcon = LoadIconW(0, IDI_APPLICATION);
    winClass.hCursor = LoadCursorW(0, IDC_ARROW);
    winClass.lpszClassName = CLASS_NAME;
    winClass.hIconSm = LoadIconW(0, IDI_APPLICATION);
    //if (data.useIcon) {
    //    HICON icon = static_cast<HICON>(LoadImage( // returns a HANDLE so we have to cast to HICON
    //        NULL,             // hInstance must be NULL when loading from a file
    //        DX::to_wstring(data.iconFile).c_str(),   // the icon file name
    //        IMAGE_ICON,       // specifies that the file is an icon
    //        0,                // width of the image (we'll specify default later on)
    //        0,                // height of the image
    //        LR_LOADFROMFILE |  // we want to load a file (as opposed to a resource)
    //        LR_DEFAULTSIZE |   // default metrics based on the type (IMAGE_ICON, 32x32)
    //        LR_SHARED         // let the system release the handle when it's no longer used
    //    ));

    //    wc.hIcon = icon;
    //    wc.hIconSm = icon;
    //}

    if (!RegisterClassEx(&winClass)) {
        MessageBox(NULL, L"Cannot register the win32 class!", L"Framework Error", MB_OK | MB_ICONERROR);
        return;
    };

    RECT initialRect = { 0, 0, width, height };
    AdjustWindowRectEx(&initialRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);
    LONG initialWidth = initialRect.right - initialRect.left;
    LONG initialHeight = initialRect.bottom - initialRect.top;

    hWnd = CreateWindowEx(
        NULL,
        winClass.lpszClassName,
        DX::to_wstring(title).c_str(),
        (!data.resizable) ? WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME : WS_OVERLAPPEDWINDOW,    // window style
        300,    // x-position of the window
        300,    // y-position of the window 
        initialWidth,    // width of the window
        initialHeight,    // height of the window
        NULL,    // we have no parent window, NULL
        NULL,    // we aren't using menus, NULL
        hInstance,    // application handle
        NULL);    // used with multiple windows, NULL)

    if (hWnd == NULL) {
        MessageBox(NULL, L"Cannot create the window!", L"Framework Error", MB_OK | MB_ICONERROR);
        return;
    }

    ShowWindow(hWnd, SW_SHOW);
}

DX::Window::~Window()
{
    DestroyWindow(hWnd);
    UnregisterClass(L"_FRAMEWORK_WINDOW_CLASS", _instance);
}

bool DX::Window::IsOpen()
{
    return (msg.message != WM_QUIT);
}

void DX::Window::PoolEvents()
{
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

HWND DX::Window::GetHandle()
{
    return hWnd;
}

int DX::Window::GetWidth()
{
    return width;
}

int DX::Window::GetHeight()
{
    return height;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT DX::Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) return true;

    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SIZE:
        int _width = LOWORD(lParam);  // Macro to get the low-order word.
        int _height = HIWORD(lParam); // Macro to get the high-order word.

        width = _width;
        height = _height;
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
