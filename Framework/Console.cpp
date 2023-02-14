#include "Console.h"
#include "Logger.h"
#include <sstream>
#include <fstream>

namespace DX
{
	Console::Console()
	{
		this->_CriticFilter = true;
		this->_ErrFilter = true;
		this->_InfFilter = true;
		this->_OkFilter = true;
		this->_WarnFilter = true;
	}

	Console::~Console()
	{
	}

	bool Console::IsWindowFocus() const
	{
		return isFocusing;
	}

	void Console::Draw(std::function<void()> commandCallback)
	{
		_DrawImGui(commandCallback);
	}

	void Console::Draw()
	{
		_DrawImGui([]() {});
	}

	void Console::_DrawImGui(std::function<void()> commandCallback)
	{
		int flags = ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::Begin("DebugConsole", (bool*)false, flags);
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Clear")) {
					DX::GetLogBuffer().clear();
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Filters")) {
				ImGui::Checkbox("Error", &_ErrFilter);
				ImGui::Checkbox("Critic", &_CriticFilter);
				ImGui::Checkbox("Warning", &_WarnFilter);
				ImGui::Checkbox("Info", &_InfFilter);
				ImGui::Checkbox("Ok", &_OkFilter);

				ImGui::EndMenu();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				ImGui::SetTooltip("Changes the filter's options for the Debug Console");
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::BeginTabBar("_debugTab1")) {
			if (ImGui::BeginTabItem("Console")) {
				ImGuiWindowFlags childFlags = ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysHorizontalScrollbar
					| ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysVerticalScrollbar;
				ImGui::BeginChild(ImGuiID("_debugChild1"), ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 70), false, childFlags);
				for (auto e : DX::GetLogBuffer()) {
					if (e.find("ERROR") != std::string::npos && _ErrFilter) {
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
						ImGui::Text(e.c_str());
						ImGui::PopStyleColor();
					}
					else if (e.find("CRITIC") != std::string::npos && _CriticFilter) {
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5451f, 0.0f, 0.0f, 1.0f));
						ImGui::Text(e.c_str());
						ImGui::PopStyleColor();
					}
					else if (e.find("WARNING") != std::string::npos && _WarnFilter) {
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
						ImGui::Text(e.c_str());
						ImGui::PopStyleColor();
					}
					else if (e.find("INFO") != std::string::npos && _InfFilter) {
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 1.0f, 1.0f));
						ImGui::Text(e.c_str());
						ImGui::PopStyleColor();
					}
					else if (e.find("OK") != std::string::npos && _OkFilter) {
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
						ImGui::Text(e.c_str());
						ImGui::PopStyleColor();
					}
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}

			commandCallback();

			ImGui::EndTabBar();
		}

		ImGui::End();
	}
}