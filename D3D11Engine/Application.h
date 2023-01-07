#pragma once

#include "Definitions.h"
#include <Window/Window.h>
#include <Window/Graphic.h>
#include <Utils/StateMachine.h>
#include <iostream>
#include <dxgidebug.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <Console.h>
#include <Logger.h>
#include "Physics.h"
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_glfw.h>

using json = nlohmann::json;

struct ApplicationData
{
	DX::Window* window = nullptr;
	DX::Graphic* D3Dgraphic = nullptr;
	DX::StateMachine STmachine;
	DX::Console debugConsole;
	
	PhysicWorld physicWorld;
	
	// Options files
	json GraphicSettings;
	json AudioSettings;
	json EditorSettings;
	float FrameLimit;
	bool vsync;

	// Please do not modify those variables
	float FPS;
	float CPU_TIME;
	float GPU_TIME;
	float RENDER_TIME;
};

typedef std::shared_ptr<ApplicationData> ApplicationDataRef;

class Application
{
private:
	Microsoft::WRL::ComPtr<ID3D11Debug> debuglayer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InfoQueue> debugInfo = nullptr;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> dxgiDebugInfo = nullptr;
	
	ApplicationDataRef data = std::make_shared<ApplicationData>();
	float dt, lasttime = 0;
	ImGuiIO* io;

	void InitWinGraphic(HINSTANCE hinstance);
	void InitImgui();
	void InitConfig();
	void HandleDebugMessage();

public:
	Application(HINSTANCE hInstance);
	~Application();

	bool is_open();
	void update();
	void draw();
};