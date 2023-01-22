#include <Windows.h>
#include <System/Config.h>
#include "Application.h"
#include <COMException.h>
#include <Utils/Utils.h>

int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    int errorCode = 0;
    try {
        Application* app = new Application(hInstance);
    
        while (app->is_open())
        {
            app->update();

            app->draw();
        }

        delete app;
    }
    catch (DX::com_exception e) {
        MessageBox(NULL, DX::toWchar(e.what()), L"D3D11 - DXGI Error", MB_OK | MB_ICONERROR);
        errorCode = e.GetErrorCode();
    }
    return errorCode;
}