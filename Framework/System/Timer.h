#pragma once
#include <chrono>

namespace DX {
	class Timer
	{
	public:
		Timer();
		float GetMilisecondsElapsed();
		void Restart();
		bool Stop();
		bool Start();
	private:
		bool isrunning = false;
		std::chrono::time_point<std::chrono::steady_clock> start;
		std::chrono::time_point<std::chrono::steady_clock> stop;
	};
}