#pragma once

#include "Definitions.h"
#include <Window/Window.h>
#include <Window/Graphic.h>
#include <System/AssetManager.h>
#include <Utils/StateMachine.h>
#include <Console.h>
#include <Logger.h>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include "Physics.h"
#include <imgui.h>
#include <imgui_impl_dx11.h>

using json = nlohmann::json;

struct ApplicationData
{
	DX::Window* window = nullptr;
	DX::Graphic* D3Dgraphic = nullptr;
	DX::StateMachine STmachine;
	DX::Console debugConsole;
	DX::AssetManager assetManager;

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
	ApplicationDataRef data = std::make_shared<ApplicationData>();
	float dt;
	bool opened = true;
	uint64_t lastCount, currentCount, frequency;
	ImGuiIO* io;

	void InitWinGraphic(HINSTANCE hinstance);
	void InitImgui();
	void InitConfig();
	
public:
	Application(HINSTANCE hInstance);
	~Application();

	bool is_open();
	void update();
	void draw();
};