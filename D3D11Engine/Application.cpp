#include "Application.h"
#include <sstream>
#include <fstream>
#include <filesystem>
#include <COMException.h>
#include <Utils/Utils.h>
#include "States/EditorState.h"
#include <time.h>

#pragma comment(lib, "dxgi.lib")

void Application::HandleDebugMessage()
{
	{
		UINT64 messages = debugInfo->GetNumStoredMessages();
		for (int i = 0; i < messages; i++) {
			SIZE_T mSize = 0;
			debugInfo->GetMessageW(i, nullptr, &mSize);

			D3D11_MESSAGE* message = (D3D11_MESSAGE*)malloc(mSize);
			DX_CHECK(debugInfo->GetMessageW(i, message, &mSize));

			auto severity = message->Severity;
			auto category = message->Category;
			std::string strMessage = message->pDescription;

			std::stringstream ss;
			ss << "[D3D11] [";
			switch (severity)
			{
			case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_INFO:
				ss << "INFO]";
				break;
			case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_MESSAGE:
				ss << "MESSAGE]";
				break;
			case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_WARNING:
				ss << "WARNING]";
				break;
			case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_ERROR:
				ss << "ERROR]";
				break;
			case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_CORRUPTION:
				ss << "CORRUPTION]";
				break;
			}

			switch (category)
			{
			case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED:
				ss << " APPLICATION: ";
				break;
			case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_CLEANUP:
				ss << " CLEANUP: ";
				break;
			case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_COMPILATION:
				ss << " COMPILATION: ";
				break;
			case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_EXECUTION:
				ss << " EXECUTION: ";
				break;
			case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_INITIALIZATION:
				ss << " INITIALIZATION: ";
				break;
			case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_MISCELLANEOUS:
				ss << " MISCELANEOUS: ";
				break;
			case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_RESOURCE_MANIPULATION:
				ss << " RESOURCE MANIPULATION: ";
				break;
			case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_SHADER:
				ss << " SHADER: ";
				break;
			case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_STATE_CREATION:
				ss << " STATE CREATION: ";
				break;
			case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_STATE_GETTING:
				ss << " STATE GETTING: ";
				break;
			case D3D11_MESSAGE_CATEGORY::D3D11_MESSAGE_CATEGORY_STATE_SETTING:
				ss << " STATE SETTING: ";
				break;
			}

			ss << strMessage << " {Error ID: " << message->ID << "}";

			DX::LogDebugMessage(ss.str());
			free(message);
		}

		debugInfo->ClearStoredMessages();
	}
	{
		UINT64 messages = dxgiDebugInfo->GetNumStoredMessages(DXGI_DEBUG_DXGI);
		for (int i = 0; i < messages; i++) {
			SIZE_T mSize = 0;
			dxgiDebugInfo->GetMessageW(DXGI_DEBUG_DXGI, i, nullptr, &mSize);

			DXGI_INFO_QUEUE_MESSAGE* message = (DXGI_INFO_QUEUE_MESSAGE*)malloc(mSize);
			DX_CHECK(dxgiDebugInfo->GetMessageW(DXGI_DEBUG_DXGI, i, message, &mSize));

			auto severity = message->Severity;
			auto category = message->Category;
			std::string strMessage = message->pDescription;

			std::stringstream ss;
			ss << "[DXGI] [";
			switch (severity)
			{
			case DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_INFO:
				ss << "INFO]";
				break;
			case DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_MESSAGE:
				ss << "MESSAGE]";
				break;
			case DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING:
				ss << "WARNING]";
				break;
			case DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR:
				ss << "ERROR]";
				break;
			case DXGI_INFO_QUEUE_MESSAGE_SEVERITY::DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION:
				ss << "CORRUPTION]";
				break;
			}

			switch (category)
			{
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_CLEANUP:
				ss << " CLEANUP: ";
				break;
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_COMPILATION:
				ss << " COMPILATION: ";
				break;
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_EXECUTION:
				ss << " EXECUTION: ";
				break;
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_INITIALIZATION:
				ss << " INITIALIZATION: ";
				break;
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_MISCELLANEOUS:
				ss << " MISCELANEOUS: ";
				break;
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_RESOURCE_MANIPULATION:
				ss << " RESOURCE MANIPULATION: ";
				break;
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_SHADER:
				ss << " SHADER: ";
				break;
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_CREATION:
				ss << " STATE CREATION: ";
				break;
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_GETTING:
				ss << " STATE GETTING: ";
				break;
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_SETTING:
				ss << " STATE SETTING: ";
				break;
			case DXGI_INFO_QUEUE_MESSAGE_CATEGORY::DXGI_INFO_QUEUE_MESSAGE_CATEGORY_UNKNOWN:
				ss << " UNKNOWN: ";
				break;
			}

			ss << strMessage;
			DX::LogDebugMessage(ss.str());
			free(message);
		}

		dxgiDebugInfo->ClearStoredMessages(DXGI_DEBUG_DXGI);
	}
}

void Application::InitWinGraphic(HINSTANCE hinstance)
{
	data->window = new DX::Window(
		"D3D11 - Engine",
		data->GraphicSettings["Window"]["width"],
		data->GraphicSettings["Window"]["height"], true
	);
	data->window->SetIconImage("Assets/gfx/dxImage.jpg");
	DX::LogInfo("Window Created!");

	data->D3Dgraphic = new DX::Graphic(&data->window, data->GraphicSettings["Window"]["fullscreen"]);
	DX::LogInfo("Graphic Initialized! {DXGI - D3D11}");

#if defined(_DEBUG)
	DX_CHECK(data->D3Dgraphic->getDevice().As(&debuglayer));
	assert(debuglayer);
	DX_CHECK(debuglayer.As(&debugInfo));
	assert(debugInfo);

	DX_CHECK(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiDebugInfo.GetAddressOf())));
	assert(dxgiDebugInfo);
#endif

	data->vsync = data->GraphicSettings["Graphic"]["vsync"];
	data->STmachine.AddState(DX::StatesRef(new EditorState(data)));
}

void Application::InitImgui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO(); (void)io;
	io->DisplaySize = ImVec2(data->window->GetWidth(), data->window->GetHeight());
	io->IniFilename = "Settings/ImGui.ini";
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard
		| ImGuiConfigFlags_DockingEnable
		| ImGuiConfigFlags_ViewportsEnable;
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

	ImGui_ImplGlfw_InitForOther(data->window->GetWindow(), true);
	ImGui_ImplDX11_Init(data->D3Dgraphic->getDevice().Get(), data->D3Dgraphic->getDeviceContext().Get());

	DX::LogInfo("ImGui Initialized!");
}

void Application::InitConfig()
{
	{
		std::ifstream graphicFile("Settings/Graphics.json");
		if (graphicFile.good() && graphicFile.is_open()) {
			try {
				data->GraphicSettings = json::parse(graphicFile);
			}
			catch (json::parse_error& err) {
				DX::LogWarning("JSON PARSER error at byte " + std::to_string(err.byte) + " | id " + std::to_string(err.id));
			}
		}
		else {
			DX::LogWarning("There is no Graphic settings file!");
		}
		graphicFile.close();
	}
	{
		std::ifstream File("Settings/Audio.json");
		if (File.good() && File.is_open()) {
			try {
				data->AudioSettings = json::parse(File);
			}
			catch (json::parse_error& err) {
				DX::LogWarning("JSON PARSER error at byte " + std::to_string(err.byte) + " | id " + std::to_string(err.id));
			}
		}
		else {
			DX::LogWarning("There is no audio settings file!");
		}
		File.close();
	}
	{
		std::ifstream File("Settings/Editor.json");
		if (File.good() && File.is_open()) {
			try {
				data->EditorSettings = json::parse(File);
			}
			catch (json::parse_error& err) {
				DX::LogWarning("JSON PARSER error at byte " + std::to_string(err.byte) + " | id " + std::to_string(err.id));
			}
		}
		else {
			DX::LogWarning("There is no audio settings file!");
		}
		File.close();
	}
	DX::LogInfo("Config File loaded correctly!");
}

Application::Application(HINSTANCE hInstance)
{
	try {
		InitConfig();
		InitWinGraphic(hInstance);
		InitImgui();
		data->physicWorld.Initialize(-9.81f);
		data->FrameLimit = data->GraphicSettings["Graphic"]["fpsLimit"];
#if defined(_DEBUG)
		DX::LogWarning("THIS VERSION OF THE APPLICATION IS IN DEBUG MODE! This mean that the Application could have bugs.");
		DX::LogDebugMessage(std::string(glfwGetVersionString()));
#endif
		srand((unsigned)time(NULL));
	}
	catch (DX::com_exception e) {
		MessageBox(data->window->GetHandle(), DX::toWchar(e.what()), L"ERROR", MB_OK | MB_ICONERROR);
	}
}

Application::~Application()
{
	// Set the changed Settings before closing the app
	data->GraphicSettings["Window"]["width"] = data->window->GetWidth();
	data->GraphicSettings["Window"]["height"] = data->window->GetHeight();
	data->GraphicSettings["Window"]["fullscreen"] = data->D3Dgraphic->isFullscreen();
	data->GraphicSettings["Graphic"]["vsync"] = data->vsync;
	data->GraphicSettings["Graphic"]["fpsLimit"] = data->FrameLimit;
	std::fstream graphicFile("Settings/Graphics.json", std::ios::out | std::ios::trunc);
	graphicFile << data->GraphicSettings;
	graphicFile.close();

	delete data->D3Dgraphic;
	delete data->window;
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	data->STmachine.GetActiveState()->destroy();
#if defined(_DEBUG)
	debuglayer.Reset();
	debugInfo.Reset();
#endif
}

bool Application::is_open()
{
	return data->window->IsOpen();
}

void Application::update()
{
	data->window->PollEvents();
	data->STmachine.ProcessChanges();

#if defined(_DEBUG)
	HandleDebugMessage();
#endif

	float time = float(glfwGetTime());
	dt = time - lasttime;

	if (dt >= 1.0f / data->FrameLimit && data->window->IsFocusing()) {
		data->FPS = 1.0f / dt;
		data->CPU_TIME = dt * 1000;
		lasttime = time;
		data->STmachine.GetActiveState()->update(dt);
		data->physicWorld.mScene->simulate(dt);
		data->physicWorld.mScene->fetchResults(true);
	}
}

void Application::draw()
{
	if (dt >= 1.0f / data->FrameLimit) {
		auto startTime = std::chrono::high_resolution_clock::now();
		float color[] = { 0, 0, 0, 1.0 };
		data->D3Dgraphic->Clear(color);
		data->D3Dgraphic->SetRenderTarget();
		data->D3Dgraphic->UpdateSwapChain();
		data->D3Dgraphic->SetViewport(data->window->GetWidth(), data->window->GetHeight(), 0, 0);
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport(ImGui::GetWindowViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

		// Draw
		data->STmachine.GetActiveState()->draw();

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