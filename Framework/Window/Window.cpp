#include "Window.h"
#include "../Utils/Utils.h"
#include <cassert>

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
    winClass.hCursor = LoadCursorW(0, IDC_ARROW);
    winClass.lpszClassName = CLASS_NAME;
    if (data.useIcon) {

        // TODO: NEED FIX

        HICON icon = (HICON)LoadImage( // returns a HANDLE so we have to cast to HICON
            NULL,             // hInstance must be NULL when loading from a file
            DX::to_wstring(data.iconFile).c_str(),   // the icon file name
            IMAGE_ICON,       // specifies that the file is an icon
            0,                // width of the image (we'll specify default later on)
            0,                // height of the image
            LR_LOADFROMFILE |  // we want to load a file (as opposed to a resource)
            LR_DEFAULTSIZE);

        winClass.hIcon = LoadIcon(hInstance, IDI_QUESTION);
        winClass.hIconSm = icon;
    }

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
        UnregisterClass(L"_FRAMEWORK_WINDOW_CLASS", _instance);
        return;
    }

    ShowWindow(hWnd, SW_SHOW);

    // ------------------------Direct Input Init------------------------
    m_mouseX = 0;
    m_mouseY = 0;
    assert(SUCCEEDED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL)));
    assert(SUCCEEDED(m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL)));
    assert(SUCCEEDED(m_keyboard->SetDataFormat(&c_dfDIKeyboard)));
    m_keyboard->Acquire();
    assert(SUCCEEDED(m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL)));
    assert(SUCCEEDED(m_mouse->SetDataFormat(&c_dfDIMouse)));
    assert(SUCCEEDED(m_mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)));
    m_mouse->Acquire();

    isOpen = true;
}

DX::Window::~Window()
{
    m_mouse->Unacquire();
    m_mouse->Release();
    m_keyboard->Unacquire();
    m_keyboard->Release();
    m_directInput->Release();
    UnregisterClass(L"_FRAMEWORK_WINDOW_CLASS", _instance);
}

bool DX::Window::IsOpen()
{
    return isOpen;
}

void DX::Window::PoolEvents()
{
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT) {
            isOpen = false;
        }
    }

    HRESULT result;
    // Read the keyboard device.
    result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
    if (FAILED(result))
    {
        // If the keyboard lost focus or was not acquired then try to get control back.
        if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            m_keyboard->Acquire();
        }
    }

    result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
    if (FAILED(result))
    {
        // If the mouse lost focus or was not acquired then try to get control back.
        if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            m_mouse->Acquire();
        }
    }

    // Update the location of the mouse cursor based on the change of the mouse location during the frame.
    m_mouseX += m_mouseState.lX;
    m_mouseY += m_mouseState.lY;

    // Ensure the mouse location doesn't exceed the screen width or height.
    if (m_mouseX < 0) { m_mouseX = 0; }
    if (m_mouseY < 0) { m_mouseY = 0; }

    if (m_mouseX > width) { m_mouseX = width; }
    if (m_mouseY > height) { m_mouseY = height; }
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

void DX::Window::GetMousePosition(int& mouseX, int& mouseY)
{
    mouseX = m_mouseX;
    mouseY = m_mouseY;
}

bool DX::Window::IsKeyPressed(unsigned int IDkey)
{
    // Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
    if (m_keyboardState[IDkey] & 0x80)
    {
        return true;
    }

    return false;
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

        if (_width == 0 || _height == 0) { // Window Minimized
            // do nothing, just wait to get window focus,
            // so we use the last window size as a reference.
            break;
        }

        width = _width;
        height = _height;
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
