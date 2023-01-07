#include "EditorState.h"

EditorState::EditorState(ApplicationDataRef data)
	: _data(data)
{
}

void EditorState::init()
{
	_renderTarget = new DX::RenderTarget<DX::RenderType::RenderTarget>(
		_data->D3Dgraphic->getDevice(), _data->D3Dgraphic->getDeviceContext(),
		500, 500, L"Assets/Shaders/RenderTarget.hlsl", DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT);
	
	std::vector<DX::VertexTexture> vertices = {
		DX::VertexTexture(-0.5f, -0.5f, 0.0f, 0, 0, 0, 0, 0),
		DX::VertexTexture( 0.5f, -0.5f, 0.0f, 1, 0, 0, 0, 0),
		DX::VertexTexture( 0.0f,  0.5f, 0.0f, 0.5, 1, 0, 0, 0),
	};

	std::vector<int> indices = {
		0, 1, 2
	};

	triangle = new DX::Mesh(_data->D3Dgraphic->getDevice(), L"Assets/Shaders/Triangle.hlsl", "PSMain", "VSMain", vertices, indices);
}

void EditorState::update(float dt)
{
	_renderTarget->SetConstantBufferData(false, 1.0f, 2.1f, false, 128, dt);
}

void EditorState::draw()
{
	// Create Predefined ImGui Window
	int windowFlag = ImGuiWindowFlags_::ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar;
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2((float)_data->window->GetWidth(), (float)_data->window->GetHeight()));
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowBorderSize, 0.0f);
	windowFlag |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus
		| ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoScrollbar;
	ImGui::Begin("mainDockSpace", (bool*)0, windowFlag);
	ImGui::DockSpace(ImGui::GetID("Dockspace"), ImVec2(), ImGuiDockNodeFlags_PassthruCentralNode);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Utils")) {
			if (ImGui::Button("Reload Shaders", ImVec2(100, 30))) {
				this->destroy();
				this->init();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	ImGui::PopStyleVar(2);
	ImGui::End();
	_data->D3Dgraphic->UnBoundRenderTarget();

	float color[4] = { 0, 0, 1, 1 };
	_renderTarget->Clear(color);
	_renderTarget->SetRenderTarget(_data->window->GetWidth(), _data->window->GetHeight());

	triangle->prepareDraw(_data->D3Dgraphic->getDeviceContext());
	triangle->Draw(_data->D3Dgraphic->getDeviceContext(), 3);

	_renderTarget->UnBoundTarget();

	_data->D3Dgraphic->SetRenderTarget();

	int flags = ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse;
	ImGui::Begin("Viewport", (bool*)0, flags);
	ImGui::Image((ImTextureID)_renderTarget->GetTextureSRV().Get(), ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 30));
	ImGui::End();

	ImGui::Begin("Editor");
	ImGui::End();

	_renderTarget->Draw();

	_data->debugConsole.Draw();
}

void EditorState::destroy()
{
	delete triangle;
	delete _renderTarget;
}
