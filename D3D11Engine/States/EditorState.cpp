#include "EditorState.h"
#include <Graphics\GraphicLoader.h>
#include <Utils\Utils.h>

using namespace DirectX;

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

	cbuffer_Cube.Initialize(_data->D3Dgraphic->getDevice().Get());
	auto test = DX::LoadMeshFile("Assets/Models/Cube.mesh");
	mesh_test = new DX::Mesh(_data->D3Dgraphic->getDevice(), DX::to_wstring(_data->assetManager.GetShader("cube")), "PSMain", "VSMain", test.vertices, test.indices);
}
void EditorState::update(float dt)
{
	float speed = 20;
	if (_data->window->IsKeyPressed(DIK_W)) {
		camera.AdjustPosition(camera.GetForwardVector() * dt * speed);
	}
	if (_data->window->IsKeyPressed(DIK_S)) {
		camera.AdjustPosition(camera.GetBackwardVector() * dt * speed);
	}
	if (_data->window->IsKeyPressed(DIK_A)) {
		camera.AdjustPosition(camera.GetLeftVector() * dt * speed);
	}
	if (_data->window->IsKeyPressed(DIK_D)) {
		camera.AdjustPosition(camera.GetRightVector() * dt * speed);
	}

	if (_data->window->IsKeyPressed(DIK_LEFT)) {
		camera.AdjustRotation(0, -1 * dt * 0.3, 0);
	}
	if (_data->window->IsKeyPressed(DIK_RIGHT)) {
		camera.AdjustRotation(0, 1 * dt * 0.3, 0);
	}

	// TODO: NEED FIX
	/*int x, y;
	_data->window->GetMousePosition(x, y);
	x *= 0.1;
	y *= 0.1;
	camera.SetRotation(y, x, 0);*/


	_renderTarget->SetConstantBufferData(_data->D3Dgraphic->getDeviceContext(), false, 1.0f, 2.1f, false, 128, dt);

	DirectX::XMMATRIX _p = DirectX::XMMatrixPerspectiveFovLH(
		(60.0f / 360.0f) / DirectX::XM_2PI,
		static_cast<float>(_data->window->GetWidth()) / static_cast<float>(_data->window->GetHeight()),
		0.1f, 1000.0f);

	DirectX::XMMATRIX _m = DirectX::XMMatrixRotationRollPitchYaw(
		DirectX::XMConvertToRadians(tX),
		DirectX::XMConvertToRadians(tY),
		DirectX::XMConvertToRadians(tZ)
	);

	DirectX::XMStoreFloat4x4(&cbuffer_Cube.data.projection, DirectX::XMMatrixTranspose(_p));
	DirectX::XMStoreFloat4x4(&cbuffer_Cube.data.view, DirectX::XMMatrixTranspose(camera.GetViewMatrix()));
	DirectX::XMStoreFloat4x4(&cbuffer_Cube.data.worldPos, DirectX::XMMatrixTranspose(_m));
	cbuffer_Cube.ApplyChanges(_data->D3Dgraphic->getDeviceContext().Get());
}

void EditorState::draw()
{

	_data->D3Dgraphic->UnBoundRenderTarget();
	float color[4] = { 0, 0, 1, 1 };
	_renderTarget->Clear(_data->D3Dgraphic->getDeviceContext(), color);
	_renderTarget->SetRenderTarget(_data->D3Dgraphic->getDevice(), _data->D3Dgraphic->getDeviceContext(), _data->window->GetWidth(), _data->window->GetHeight());

	mesh_test->prepareDraw(_data->D3Dgraphic->getDeviceContext());
	cbuffer_Cube.Bind(_data->D3Dgraphic->getDeviceContext().Get(), 0, DX::ConstantBuffer_BindType::VertexShader);
	mesh_test->Draw(_data->D3Dgraphic->getDeviceContext(), 36);

	spriteBatch->Begin();
	std::stringstream ss;
	ss << (int)_data->FPS << " FPS" << std::endl << _data->CPU_TIME << " ms";

	spriteFont->DrawString(spriteBatch, ss.str().c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0, 0), 2);
	spriteBatch->End();

	_renderTarget->UnBoundTarget(_data->D3Dgraphic->getDeviceContext());

	_data->D3Dgraphic->SetRenderTarget();
	
	if (showEditor) {
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

		int flags = ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::Begin("Viewport", (bool*)0, flags);
		ImGui::Image((ImTextureID)_renderTarget->GetTextureSRV().Get(), ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 35));
		ImGui::End();

		ImGui::Begin("Editor");
		ImGui::SliderFloat("Cube X", &tX, 0, 360);
		ImGui::SliderFloat("Cube Y", &tY, 0, 360);
		ImGui::SliderFloat("Cube Z", &tZ, 0, 360);

		ss.clear();
		int mx, my;
		_data->window->GetMousePosition(mx, my);
		ss << "Mouse X: " << mx << std::endl
			<< "Mouse Y: " << my;
		ImGui::Text(ss.str().c_str());
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
	delete _renderTarget;
}
