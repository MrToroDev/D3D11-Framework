#include "Window.h"
#include "../Utils/Utils.h"

int DX::Window::width = 0;
int DX::Window::height = 0;

DX::Window::Window(HINSTANCE hInstance, const char* title, int width, int height, WindowDataInfo data)
    : _instance(hInstance)
{
	const wchar_t CLASS_NAME[] = L"_FRAMEWORK_WINDOW_CLASS";
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WindowProc;
    if (data.useIcon) {
        wc.hIcon = static_cast<HICON>(LoadImage( // returns a HANDLE so we have to cast to HICON
            NULL,             // hInstance must be NULL when loading from a file
            DX::to_wstring(data.iconFile).c_str(),   // the icon file name
            IMAGE_ICON,       // specifies that the file is an icon
            0,                // width of the image (we'll specify default later on)
            0,                // height of the image
            LR_LOADFROMFILE |  // we want to load a file (as opposed to a resource)
            LR_DEFAULTSIZE |   // default metrics based on the type (IMAGE_ICON, 32x32)
            LR_SHARED         // let the system release the handle when it's no longer used
        ));
    }
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszMenuName = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbClsExtra = NULL; 
    wc.cbWndExtra = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, L"Cannot Register the window class!", L"Framework Window Class", MB_OK | MB_ICONERROR);
        return;
    }


    DWORD style = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
    if (data.resizable) {
        style = style | WS_THICKFRAME | WS_MAXIMIZEBOX;
    }
    hWnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        DX::to_wstring(title).c_str(),    // Window text
        style,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );
    if (hWnd == NULL) {
        MessageBox(NULL, L"Cannot create the window!", L"Framework Window Class", MB_OK | MB_ICONERROR);
        return;
    }
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
}

DX::Window::~Window()
{
    DestroyWindow(hWnd);
    UnregisterClass(L"_FRAMEWORK_WINDOW_CLASS", _instance);
}

bool DX::Window::IsOpen()
{
    return msg.message != WM_QUIT;
}

void DX::Window::PoolEvents()
{
    if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
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
