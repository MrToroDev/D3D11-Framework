#include "Timer.h"

using namespace DX;

Timer::Timer()
{
	start = std::chrono::high_resolution_clock::now();
	stop = std::chrono::high_resolution_clock::now();
}

float Timer::GetMilisecondsElapsed()
{
	if (isrunning)
	{
		auto elapsed = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - start);
		return elapsed.count();
	}
	else
	{
		auto elapsed = std::chrono::duration<float>(stop - start);
		return elapsed.count();
	}
}

void Timer::Restart()
{
	isrunning = true;
	start = std::chrono::high_resolution_clock::now();
}

bool Timer::Stop()
{
	if (!isrunning)
		return false;
	else
	{
		stop = std::chrono::high_resolution_clock::now();
		isrunning = false;
		return true;
	}
}

bool Timer::Start()
{
	if (isrunning)
	{
		return false;
	}
	else
	{
		start = std::chrono::high_resolution_clock::now();
		isrunning = true;
		return true;
	}
}