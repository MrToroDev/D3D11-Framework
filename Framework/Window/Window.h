#pragma once

#include <Windows.h>
#include <GLFW/glfw3.h>

namespace DX {
	class Window
	{
	public:
		Window(const char* title, int width, int height, bool resizable);
		~Window();

		bool IsOpen();
		bool IsFocusing();
		bool KeyPressed(int key);
		void Close();
		void PollEvents();
		void SetIconImage(const char* file);

		void SetCursorEnable(bool cursor);
		double GetCursorX();
		double GetCursorY();

		HWND GetHandle();
		GLFWwindow* GetWindow();
		int GetWidth();
		int GetHeight();

	private:
		GLFWwindow* window;
		static bool isfocusing;
		static double xcpos, ycpos;
		static double lastX, lastY;
		static void FocusCallback(GLFWwindow* window, int focused);
		static void CursorCallback(GLFWwindow* window, double x, double y);
	};
}