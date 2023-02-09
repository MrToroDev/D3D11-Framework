#pragma once

#include <imgui.h>
#include <string>
#include <functional>

namespace DX
{
    /**
     * @brief Base class for a console log gui with ImGui
     * 
     */
    class Console
    {
    public:
        Console();
        ~Console();

        bool IsWindowFocus() const;
        void Draw(std::function<void()> additionalTabs);
        void Draw();

    private:
        void _DrawImGui(std::function<void()> commandCallback);
        bool _ErrFilter, _WarnFilter, _InfFilter, _OkFilter, _CriticFilter;
        bool isFocusing;
    };
}