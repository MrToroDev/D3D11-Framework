#include "Window.h"
#include <assert.h>
#include <stb_image.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

bool DX::Window::isfocusing = true;
double DX::Window::xcpos = 0.0;
double DX::Window::ycpos = 0.0;
double DX::Window::lastX = 400.0;
double DX::Window::lastY = 300.0;
namespace DX {
	bool firstMouse = true;
}

void DX::Window::FocusCallback(GLFWwindow* win, int focused) {
	isfocusing = focused;
}

void DX::Window::CursorCallback(GLFWwindow* window, double x, double y)
{
	if (firstMouse) {
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	if (isfocusing) {
		double xoffset = x - lastX;
		double yoffset = y - lastY;
		lastX = x;
		lastY = y;

		xoffset *= 0.01f;
		yoffset *= 0.01f;

		xcpos = xoffset;
		ycpos = yoffset;
	}
}

DX::Window::Window(const char* title, int width, int height, bool resizable)
{
	if (!glfwInit()) {
		MessageBoxA(NULL, "Error - GLFW_INIT. Cannot init glfw window library!", "Error", MB_OK | MB_ICONWARNING);
		return;
	}
	//glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, resizable);
	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window) {
		MessageBoxA(NULL, "Error - GLFW_CREATE_WINDOW. Cannot create the window!", "Error", MB_OK | MB_ICONWARNING);
		return;
	}
	glfwSetWindowFocusCallback(window, FocusCallback);
	glfwSetCursorPosCallback(window, CursorCallback);
}

DX::Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool DX::Window::IsOpen()
{
	return !glfwWindowShouldClose(window);
}

bool DX::Window::IsFocusing()
{
	return isfocusing;
}

bool DX::Window::KeyPressed(int key)
{
	return glfwGetKey(window, key) == GLFW_PRESS;
}

void DX::Window::Close()
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void DX::Window::PollEvents()
{
	glfwPollEvents();
}

void DX::Window::SetIconImage(const char* file)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, STBI_rgb_alpha);
	assert((data) && " && Wrong type of format or (generally) wrong path!");

	GLFWimage image;
	image.width = width;
	image.height = height;
	image.pixels = data;
	glfwSetWindowIcon(window, 1, &image);
}

void DX::Window::SetCursorEnable(bool cursor)
{
	glfwSetInputMode(window, GLFW_CURSOR, (cursor) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

double DX::Window::GetCursorX()
{
	return xcpos;
}

double DX::Window::GetCursorY()
{
	return ycpos;
}

HWND DX::Window::GetHandle()
{
	return glfwGetWin32Window(window);
}

GLFWwindow* DX::Window::GetWindow()
{
	return window;
}

int DX::Window::GetWidth()
{
	int w;
	glfwGetWindowSize(window, &w, nullptr);
	return w;
}

int DX::Window::GetHeight()
{
	int h;
	glfwGetWindowSize(window, nullptr, &h);
	return h;
}