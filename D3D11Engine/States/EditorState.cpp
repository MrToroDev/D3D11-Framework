#include "EditorState.h"
#include <Utils\Utils.h>

EditorState::EditorState(ApplicationDataRef data)
	: _data(data)
{
}

void EditorState::init()
{
	showEditor = true;
	_renderTarget = new DX::RenderTarget<DX::RenderType::RenderTarget>(
		_data->D3Dgraphic->getDevice(), _data->D3Dgraphic->getDeviceContext(),
		500, 500, DX::to_wstring(_data->assetManager.GetShader("renderTarget")),
		DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT);

	spriteBatch = new DirectX::DX11::SpriteBatch(_data->D3Dgraphic->getDeviceContext().Get());
	spriteFont = new DirectX::DX11::SpriteFont(_data->D3Dgraphic->getDevice().Get(),
		DX::to_wstring(_data->assetManager.GetFont("consola")).c_str());

	std::vector<DX::VertexTexture> triangle_v = {
		DX::VertexTexture(-0.5f, -0.5f, 0.0f, 0, 0, 0, 0, 0),
		DX::VertexTexture(0.5f, -0.5f, 0.0f, 1, 0, 0, 0, 0),
		DX::VertexTexture(0.0f,  0.5f, 0.0f, 0.5, 1, 0, 0, 0),
	};

	std::vector<int> triangle_i = {
		0, 1, 2
	};

	std::vector<DX::VertexTexture> cube_v = {
		// Front Face
			DX::VertexTexture(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
			DX::VertexTexture(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
			DX::VertexTexture(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
			DX::VertexTexture(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),

			// Back Face
			DX::VertexTexture(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),
			DX::VertexTexture(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
			DX::VertexTexture(1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
			DX::VertexTexture(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),

			// Top Face
			DX::VertexTexture(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
			DX::VertexTexture(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
			DX::VertexTexture(1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
			DX::VertexTexture(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),

			// Bottom Face
			DX::VertexTexture(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),
			DX::VertexTexture(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
			DX::VertexTexture(1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
			DX::VertexTexture(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),

			// Left Face
			DX::VertexTexture(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
			DX::VertexTexture(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
			DX::VertexTexture(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
			DX::VertexTexture(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),

			// Right Face
			DX::VertexTexture(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
			DX::VertexTexture(1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
			DX::VertexTexture(1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
			DX::VertexTexture(1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),
	};

	std::vector<int> cube_i = {
		// Front Face
			0,  1,  2,
			0,  2,  3,

			// Back Face
			4,  5,  6,
			4,  6,  7,

			// Top Face
			8,  9, 10,
			8, 10, 11,

			// Bottom Face
			12, 13, 14,
			12, 14, 15,

			// Left Face
			16, 17, 18,
			16, 18, 19,

			// Right Face
			20, 21, 22,
			20, 22, 23
	};

	triangle = new DX::Mesh(_data->D3Dgraphic->getDevice(), DX::to_wstring(_data->assetManager.GetShader("triangle")), "PSMain", "VSMain", triangle_v, triangle_i);
	cube = new DX::Mesh(_data->D3Dgraphic->getDevice(), DX::to_wstring(_data->assetManager.GetShader("cube")), "PSMain", "VSMain", cube_v, cube_i);
	cbuffer_Cube.Initialize(_data->D3Dgraphic->getDevice().Get());
	
	camera.SetPosition(-10, 0, 0);
}

void EditorState::update(float dt)
{
	camera.SetRotation(0, DirectX::XMConvertToRadians(viewRotation), 0);
	_renderTarget->SetConstantBufferData(_data->D3Dgraphic->getDeviceContext(), false, 1.0f, 2.1f, false, 128, dt);

	DirectX::XMMATRIX _p = DirectX::XMMatrixPerspectiveFovLH(
		(60.0f * DirectX::XM_PI) / 360,
		static_cast<float>(_data->window->GetWidth() / _data->window->GetHeight()),
		0.1f, 100.0f);

	DirectX::XMStoreFloat4x4(&cbuffer_Cube.data.projection, _p);
	DirectX::XMStoreFloat4x4(&cbuffer_Cube.data.view, camera.GetViewMatrix());
	DirectX::XMStoreFloat4x4(&cbuffer_Cube.data.worldPos, DirectX::XMMatrixIdentity());
	cbuffer_Cube.ApplyChanges(_data->D3Dgraphic->getDeviceContext().Get());
}

void EditorState::draw()
{
	if (showEditor) {
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
	}

	_data->D3Dgraphic->UnBoundRenderTarget();

	float color[4] = { 0, 0, 1, 1 };
	_renderTarget->Clear(_data->D3Dgraphic->getDeviceContext(), color);
	_renderTarget->SetRenderTarget(_data->D3Dgraphic->getDevice(), _data->D3Dgraphic->getDeviceContext(), _data->window->GetWidth(), _data->window->GetHeight());

	triangle->prepareDraw(_data->D3Dgraphic->getDeviceContext());
	cbuffer_Cube.Bind(_data->D3Dgraphic->getDeviceContext().Get(), 0, DX::ConstantBuffer_BindType::VertexShader);
	triangle->Draw(_data->D3Dgraphic->getDeviceContext(), 3);

	cube->prepareDraw(_data->D3Dgraphic->getDeviceContext());
	cbuffer_Cube.Bind(_data->D3Dgraphic->getDeviceContext().Get(), 0, DX::ConstantBuffer_BindType::VertexShader);
	cube->Draw(_data->D3Dgraphic->getDeviceContext(), 36);

	spriteBatch->Begin();
	std::stringstream ss;
	ss << (int)_data->FPS << " FPS" << std::endl << _data->CPU_TIME << " ms";

	spriteFont->DrawString(spriteBatch, ss.str().c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0, 0), 2);
	spriteBatch->End();

	_renderTarget->UnBoundTarget(_data->D3Dgraphic->getDeviceContext());

	_data->D3Dgraphic->SetRenderTarget();

	if (showEditor) {
		int flags = ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::Begin("Viewport", (bool*)0, flags);
		ImGui::Image((ImTextureID)_renderTarget->GetTextureSRV().Get(), ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 30));
		ImGui::End();

		ImGui::Begin("Editor");
		ImGui::SliderAngle("X View", &viewRotation, 0.0f);
		ImGui::End();
	}

	int wFlags = ImGuiWindowFlags_::ImGuiWindowFlags_NoDocking;
	ImGui::Begin("DefaultWindow", (bool*)0, wFlags);
	ImGui::Checkbox("Show Editor", &showEditor);
	ImGui::End();

	if (!showEditor)
		_renderTarget->Draw(_data->D3Dgraphic->getDeviceContext());

	_data->debugConsole.Draw();
}

void EditorState::destroy()
{
	delete spriteBatch;
	delete spriteFont;
	delete triangle;
	delete cube;
	delete _renderTarget;
}
