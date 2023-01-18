#include "Logger.h"
#include <Windows.h>

namespace DX {
	std::vector<std::string> _log_buf;
}

void DX::Log(std::string text)
{
	std::string ss = "[OK] ";
	ss.append(text);
	ss.append("\n");
	OutputDebugStringA(ss.c_str());
	_log_buf.push_back(ss);
}

void DX::LogInfo(std::string text)
{
	std::string ss = "[INFO] ";
	ss.append(text);
	ss.append("\n");
	OutputDebugStringA(ss.c_str());
	_log_buf.push_back(ss);
}

void DX::LogWarning(std::string text)
{
	std::string ss = "[WARNING] ";
	ss.append(text);
	ss.append("\n");
	OutputDebugStringA(ss.c_str());
	_log_buf.push_back(ss);
}

void DX::LogError(std::string text)
{
	std::string ss = "[ERROR] ";
	ss.append(text);
	ss.append("\n");
	OutputDebugStringA(ss.c_str());
	_log_buf.push_back(ss);
}

void DX::LogCritic(std::string text)
{
	std::string ss = "[CRITIC] ";
	ss.append(text);
	ss.append("\n");
	OutputDebugStringA(ss.c_str());
	_log_buf.push_back(ss);
}

void DX::LogDebugMessage(std::string text)
{
	std::string ss;
	ss.append(text);
	ss.append("\n");
	OutputDebugStringA(ss.c_str());
	_log_buf.push_back(ss);
}

std::vector<std::string>& DX::GetLogBuffer()
{
	return _log_buf;
}
