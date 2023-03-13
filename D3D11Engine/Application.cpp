#include "Application.h"
#include <sstream>
#include <fstream>
#include <filesystem>
#include <COMException.h>
#include <Utils/Utils.h>
#include "States/EditorState.h"
//#include "States/TestsState.h"
#include <time.h>
#include <imgui_impl_win32.h>

#pragma comment(lib, "dxgi.lib")

void Application::InitWinGraphic(HINSTANCE hinstance)
{
	DX::WindowDataInfo wInfo;
	wInfo.resizable = true;
	wInfo.useIcon = true;
	wInfo.iconFile = "Assets/Gfx/Icon.ico";

	data->window = new DX::Window(
		hinstance, APP_NAME, 
		data->GraphicSettings.get_integer("Video", "width"),
		data->GraphicSettings.get_integer("Video", "height"),
		wInfo
	);
	DX::LogInfo("Window Created!");

	data->D3Dgraphic = new DX::Graphic(data->window, data->GraphicSettings.get_bool("Video", "fullscreen"));
	DX::LogInfo("Graphic Initialized! {DXGI - D3D11}");

	data->vsync = data->GraphicSettings.get_bool("Graphics", "vsync");
	data->STmachine.AddState(DX::StatesRef(new EditorState(data)));
}

void Application::InitImgui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO(); (void)io;
	io->DisplaySize = ImVec2((float)data->window->GetWidth(), (float)data->window->GetHeight());
	io->IniFilename = "Settings/ImGui.ini";
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard
		| ImGuiConfigFlags_DockingEnable;
		//| ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsClassic();

	ImGuiStyle* style = &ImGui::GetStyle();
	style->Colors[ImGuiCol_Text] = ImVec4(1.0, 1.0, 1.0, 1.0);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 1);
	style->Colors[ImGuiCol_Button] = ImVec4(0, 0.5, 0, 1);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0, 1, 0, 1);
	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style->WindowRounding = 0.0f;
		style->Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	style->ScrollbarSize = 20;
	style->ScrollbarRounding = 0;
	ImGui_ImplWin32_Init(data->window->GetHandle());
	ImGui_ImplDX11_Init(data->D3Dgraphic->getDevice().Get(), data->D3Dgraphic->getDeviceContext().Get());

	DX::LogInfo("ImGui Initialized!");
}

void Application::InitConfig()
{
	data->GraphicSettings.OpenFile("Settings/DefaultGraphicSettings.ini");

	DX::LogInfo("Config File loaded correctly!");
}

Application::Application(HINSTANCE hInstance)
{
	try {
		InitConfig();
		InitWinGraphic(hInstance);
		InitImgui();
		data->physicWorld.Initialize(-9.81f);
		data->FrameLimit = data->GraphicSettings.get_float("Graphics", "fps");
#if defined(_DEBUG)
		DX::LogWarning("THIS VERSION OF THE APPLICATION IS IN DEBUG MODE! This mean that the Application could have bugs.");
#endif
		data->assetManager.InitResources("Assets/Data.json");
		srand((unsigned)time(NULL));
		data->TimeStep = 1.0f;
		QueryPerformanceCounter(&start_time);
		QueryPerformanceFrequency(&frequency);
	}
	catch (DX::com_exception e) {
		MessageBox(data->window->GetHandle(), DX::toWchar(e.what()), L"ERROR", MB_OK | MB_ICONERROR);
	}
}

Application::~Application()
{
	data->D3Dgraphic->ClearStateFlush();

	DX::LogInfo("Shutting Down ImGui...");
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
	DX::LogInfo("Destroying Window...");
	delete data->window;
	DX::LogInfo("Destroying State objects...");
	data->STmachine.GetActiveState()->destroy();
	DX::LogInfo("Destroying Graphics...");
	data->D3Dgraphic->ClearStateFlush();
	delete data->D3Dgraphic;
}

bool Application::is_open()
{
	return data->window->IsOpen();
}

void Application::update()
{
	data->window->PoolEvents();
	data->STmachine.ProcessChanges();

	QueryPerformanceCounter(&end_time);
	dt = (end_time.QuadPart - start_time.QuadPart) * (1.0f / frequency.QuadPart);

	if (dt >= 1.0f / data->FrameLimit) {
		start_time = end_time;
		data->FPS = 1.0f / dt;
		// Fix the cpu time to 2 decimal precision
		int value = (int)((dt * 1000) * 100 + .5);
		data->CPU_TIME = (float)value / 100;

		float _deltaValue = dt * data->TimeStep;

		data->STmachine.GetActiveState()->update(_deltaValue);
		data->physicWorld.mScene->simulate(_deltaValue);
		data->physicWorld.mScene->fetchResults(true);
	}
}

void Application::draw()
{
	if (dt >= 1.0f / data->FrameLimit) {
		io->DisplaySize = ImVec2((float)data->window->GetWidth(), (float)data->window->GetHeight());

		auto startTime = std::chrono::high_resolution_clock::now();
		float color[] = { 0, 0, 0, 1.0 };
		data->D3Dgraphic->Clear(color);
		data->D3Dgraphic->SetRenderTarget();
		data->D3Dgraphic->UpdateSwapChain();
		data->D3Dgraphic->SetViewport(data->window->GetWidth(), data->window->GetHeight(), 0, 0);
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport(ImGui::GetWindowViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

		// Draw
		data->STmachine.GetActiveState()->draw();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		data->D3Dgraphic->Present(data->vsync);

		data->GPU_TIME = data->D3Dgraphic->GetGPUTime();

		auto endTime = std::chrono::high_resolution_clock::now();
		auto startMicro = std::chrono::time_point_cast<std::chrono::microseconds>(startTime).time_since_epoch().count();
		auto endMicro = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();
		long long int duration = endMicro - startMicro;
		data->RENDER_TIME = static_cast<float>(duration * 0.001f);
	}
}
