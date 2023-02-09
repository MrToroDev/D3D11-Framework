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

	std::vector<DX::VertexTexture> plane_v = {
		DX::VertexTexture(-0.5f, -0.5f, 0.5f, 0, 0, 0.0f, 1.0f, 0.0f),
		DX::VertexTexture( 0.5f, -0.5f, 0.5f, 0, 0, 0.0f, 1.0f, 0.0f),
		DX::VertexTexture(-0.5f, -0.5f, -0.5f, 0, 0, 0.0f, 1.0f, 0.0f),
		DX::VertexTexture( 0.5f, -0.5f, -0.5f, 0, 0, 0.0f, 1.0f, 0.0f),
	};

	std::vector<int> plane_i = {
		0, 1, 2, 3
	};

	sampler = new DX::SamplerState(_data->D3Dgraphic->getDevice(), D3D11_TEXTURE_ADDRESS_BORDER, D3D11_TEXTURE_ADDRESS_BORDER, D3D11_TEXTURE_ADDRESS_BORDER, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	cbuffer_Cube.Initialize(_data->D3Dgraphic->getDevice().Get());
	cbuffer_Cube2.Initialize(_data->D3Dgraphic->getDevice().Get());
	auto test = DX::LoadMeshFile("Assets/Models/Cube.mesh");
	mesh_test = new DX::Mesh(_data->D3Dgraphic->getDevice(), DX::to_wstring(_data->assetManager.GetShader("cube")), "PSMain", "VSMain", test.vertices, test.indices);
	
	plane = new DX::Mesh(_data->D3Dgraphic->getDevice(), DX::to_wstring(_data->assetManager.GetShader("cube")), "PSMain", "VSMain", plane_v, plane_i);
	
	billboard = new DX::Mesh(_data->D3Dgraphic->getDevice(), DX::to_wstring(_data->assetManager.GetShader("billboard")), "PS", "VS", plane_v, plane_i);
	billboard_texture = new DX::Texture(_data->assetManager.GetTexture("dximage").c_str(), _data->D3Dgraphic->getDevice());
}

void EditorState::update(float dt)
{
	if (this->UpdateViewport) {
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
			camera.AdjustRotation(0, -1 * dt * 0.3f, 0);
		}
		if (_data->window->IsKeyPressed(DIK_RIGHT)) {
			camera.AdjustRotation(0, 1 * dt * 0.3f, 0);
		}
	}

	_renderTarget->SetConstantBufferData(_data->D3Dgraphic->getDeviceContext(), false, 1.0f, 2.1f, false, 128, dt);

	DirectX::XMMATRIX _p = DirectX::XMMatrixPerspectiveFovLH(
		(60.0f / 360.0f) / DirectX::XM_2PI,
		static_cast<float>(_data->window->GetWidth()) / static_cast<float>(_data->window->GetHeight()),
		0.1f, 1000.0f);

	DirectX::XMMATRIX _m = 
		DirectX::XMMatrixScaling(_cubeT.Scale.x, _cubeT.Scale.y, _cubeT.Scale.z) * 
		DirectX::XMMatrixRotationRollPitchYaw(
			DirectX::XMConvertToRadians(_cubeT.Rotation.x),
			DirectX::XMConvertToRadians(_cubeT.Rotation.y),
			DirectX::XMConvertToRadians(_cubeT.Rotation.z)
		) * 
		DirectX::XMMatrixTranslation(_cubeT.Position.x, _cubeT.Position.y, _cubeT.Position.z) * dt;
	
	DirectX::XMStoreFloat4x4(&cbuffer_Cube.data.projection, DirectX::XMMatrixTranspose(_p));
	DirectX::XMStoreFloat4x4(&cbuffer_Cube.data.view, DirectX::XMMatrixTranspose(camera.GetViewMatrix()));
	DirectX::XMStoreFloat4x4(&cbuffer_Cube.data.worldPos, DirectX::XMMatrixTranspose(_m));
	cbuffer_Cube.ApplyChanges(_data->D3Dgraphic->getDeviceContext().Get());
	
	DirectX::XMStoreFloat4x4(&cbuffer_Cube2.data.projection, DirectX::XMMatrixTranspose(_p));
	DirectX::XMStoreFloat4x4(&cbuffer_Cube2.data.view, DirectX::XMMatrixTranspose(camera.GetViewMatrix()));
	auto e = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	DirectX::XMStoreFloat4x4(&cbuffer_Cube2.data.worldPos, DirectX::XMMatrixTranspose(
		DX::Math::Billboard(camera.GetPositionFloat3(), DirectX::XMFLOAT3(0, 1, 0), &e, DirectX::XMFLOAT3(0.0f, 0.0f, -3.0f))
	));
	cbuffer_Cube2.ApplyChanges(_data->D3Dgraphic->getDeviceContext().Get());
}

void EditorState::draw()
{
	_data->D3Dgraphic->UnBoundRenderTarget();
	float color[4] = { 0, 1, 1, 1 };

	_renderTarget->SetRenderTarget(_data->D3Dgraphic->getDevice(), _data->D3Dgraphic->getDeviceContext(), _data->window->GetWidth(), _data->window->GetHeight());
	_renderTarget->Clear(_data->D3Dgraphic->getDeviceContext(), color);

	mesh_test->prepareDraw(_data->D3Dgraphic->getDeviceContext());
	sampler->Bind(_data->D3Dgraphic->getDeviceContext(), 0);
	billboard_texture->Bind(_data->D3Dgraphic->getDeviceContext(), 0);
	cbuffer_Cube.Bind(_data->D3Dgraphic->getDeviceContext().Get(), 0, DX::ConstantBuffer_BindType::VertexShader);
	mesh_test->Draw(_data->D3Dgraphic->getDeviceContext(), 36);

	spriteBatch->Begin();
	std::stringstream ss;
	ss << (int)_data->FPS << " FPS"
		<< std::endl << _data->CPU_TIME << " ms";

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

	if (showEditor) {
		int flags = ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::Begin("Viewport", (bool*)0, flags);
		ImGui::Image((ImTextureID)_renderTarget->GetTextureSRV().Get(), ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 35));
		bool isItemFocused = ImGui::IsItemHovered();
		if (isItemFocused) {
			this->UpdateViewport = true;
		}
		else this->UpdateViewport = false;
		ImGui::End();

		ImGui::Begin("Editor");
		ImGui::SliderFloat("Cube_Rot.X", &_cubeT.Rotation.x, 0, 360);
		ImGui::SliderFloat("Cube_Rot.Y", &_cubeT.Rotation.y, 0, 360);
		ImGui::SliderFloat("Cube_Rot.Z", &_cubeT.Rotation.z, 0, 360);
		ImGui::SliderFloat("Cube_Pos.X", &_cubeT.Position.x, -400, 400);
		ImGui::SliderFloat("Cube_Pos.Y", &_cubeT.Position.y, -400, 400);
		ImGui::SliderFloat("Cube_Pos.Z", &_cubeT.Position.z, -400, 400);
		ImGui::SliderFloat("Cube_Scale.X", &_cubeT.Scale.x, 0.1f, 4.0f);
		ImGui::SliderFloat("Cube_Scale.Y", &_cubeT.Scale.y, 0.1f, 4.0f);
		ImGui::SliderFloat("Cube_Scale.Z", &_cubeT.Scale.z, 0.1f, 4.0f);
		ImGui::SliderFloat("Timestep", &_data->TimeStep, 0.001f, 2.0f);
		ImGui::End();

		ImGui::Begin("Stats");
		{
			std::stringstream ss;
			ss << "FPS: " << _data->FPS << std::endl
				<< "CPU: " << _data->CPU_TIME << " ms" << std::endl
				<< "GPU: " << _data->GPU_TIME << " ms" << std::endl
				<< "RENDER: " << _data->RENDER_TIME << " ms" << std::endl;

			ImGui::Text(ss.str().c_str());
		}
		ImGui::End();
	}

	int wFlags = ImGuiWindowFlags_::ImGuiWindowFlags_NoDocking;
	ImGui::Begin("DefaultWindow", (bool*)0, wFlags);
	ImGui::Checkbox("Show Editor", &showEditor);
	ImGui::End();

	if (!showEditor) {
		_renderTarget->Draw(_data->D3Dgraphic->getDeviceContext());
		this->UpdateViewport = true;
	}

	_data->debugConsole.Draw();

	{
		spriteBatch->Begin();
		std::stringstream ss;
		ss << (int)_data->FPS << " FPS" << std::endl
			<< _data->CPU_TIME << " ms" << std::endl;

		spriteFont->DrawString(spriteBatch, ss.str().c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0, 0), 2);
		spriteBatch->End();
	}
}

void EditorState::destroy()
{
	delete spriteBatch;
	delete spriteFont;
	delete _renderTarget;
}
