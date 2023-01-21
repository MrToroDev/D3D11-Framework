#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

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

	private:
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static int width, height;
		HINSTANCE _instance;
		HWND hWnd;
		MSG msg = {};
		WNDCLASSEX wc;
	};
}