#include "Joystick.h"
#include <string>
#include <sstream>
#include <math.h>
#include <GLFW/glfw3.h>

namespace DX {
	GLFWgamepadstate joystick_state_temp;
}

DX::Joystick::Joystick()
{
}

bool DX::Joystick::IsConnected(JoystickPlayers n)
{
	return glfwJoystickPresent((int)n) && glfwJoystickIsGamepad((int)n) && glfwGetGamepadState((int)n, &joystick_state_temp);
}

float DX::Joystick::GetAxesValue(JoystickAxes x)
{
	//return ((joystick_state_temp.axes[(int)x]) - (-1)) / (1 - (-1));
	return (joystick_state_temp.axes[(int)x]);
}

bool DX::Joystick::IsButtonPressed(JoystickButtons b)
{
	return joystick_state_temp.buttons[(int)b];
}