#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#define DIRECTINPUT_VERSION 0x0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dinput.h>

namespace DX {
	struct WindowDataInfo {
		bool resizable;
		const char* iconFile;
		bool useIcon;
	};

	class Window
	{
	public:
		Window(HINSTANCE hInstance, const char* title, int width, int height, WindowDataInfo data);
		~Window();

		bool IsOpen();
		void PoolEvents();

		HWND GetHandle();
		int GetWidth();
		int GetHeight();

		void GetMousePosition(int& mouseX, int& mouseY);

		// USE DIK_XXX key as IDkey
		bool IsKeyPressed(unsigned int IDkey);

	private:
		IDirectInput8* m_directInput;
		IDirectInputDevice8* m_keyboard;
		IDirectInputDevice8* m_mouse;
		unsigned char m_keyboardState[256];
		DIMOUSESTATE m_mouseState;
		int m_mouseX, m_mouseY;

	private:
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static int width, height;
		bool isOpen;
		HINSTANCE _instance;
		HWND hWnd;
		MSG msg = {};
	};
}