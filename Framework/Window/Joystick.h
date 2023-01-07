#pragma once

namespace DX
{
	enum class JoystickPlayers {
		Joystick1 = 0,
		Joystick2 = 1,
		Joystick3 = 2,
		Joystick4 = 3,
		Joystick5 = 4,
		Joystick6 = 5,
		Joystick7 = 6,
		Joystick8 = 7,
		Joystick9 = 8,
		Joystick10 = 9,
		Joystick11 = 10,
		Joystick12 = 11,
		Joystick13 = 12,
		Joystick14 = 13,
		Joystick15 = 14,
		Joystick16 = 15,
	};

	enum class JoystickAxes {
		LEFT_STICK_X,
		LEFT_STICK_Y,
		RIGHT_STICK_X,
		RIGHT_STICK_Y,
		LEFT_TRIGGER_L2,
		RIGHT_TRIGGER_R2,
	};

	enum class JoystickButtons {
		Y = 0,
		X,
		A,
		B,
		PS_X = A,
		PS_Y = B,
		PS_SQUARE = X,
		PS_TRIANGLE = Y,
		LEFT_TRIGGER_1 = 4,
		RIGHT_TRIGGER_1 = 5,
		LEFT_TRIGGER_2 = 6,
		RIGHT_TRIGGER_2 = 7,
		SELECT,
		START
	};

	class Joystick
	{
	public:
		Joystick();

		bool IsConnected(JoystickPlayers n);
		float GetAxesValue(JoystickAxes x);
		bool IsButtonPressed(JoystickButtons b);

	private:
		JoystickPlayers m_selected;
	};
}