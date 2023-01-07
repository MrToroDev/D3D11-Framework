#pragma once

#include <vector>
#include <string>

namespace DX
{
	void Log(std::string text);
	void LogInfo(std::string text);
	void LogWarning(std::string text);
	void LogError(std::string text);
	void LogCritic(std::string text);
	void LogDebugMessage(std::string text);

	std::vector<std::string>& GetLogBuffer();
}