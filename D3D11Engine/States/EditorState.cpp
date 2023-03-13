#include "EditorState.h"
#include <Graphics\GraphicLoader.h>
#include <Utils\Utils.h>

void CreateSphere(int LatLines, int LongLines, ID3D11Device* d3d11Device, ID3D11Buffer* sphereIndexBuffer, ID3D11Buffer* sphereVertBuffer)
{
	int NumSphereVertices;
	int NumSphereFaces;
	XMMATRIX Rotationx;
	XMMATRIX Rotationy;
	XMMATRIX Rotationz;
	XMMATRIX sphereWorld;

	NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
	NumSphereFaces = ((LatLines - 3) * (LongLines) * 2) + (LongLines * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	struct Vertex    //Overloaded Vertex Structure
	{
		Vertex() {}
		Vertex(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz)
			: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
	};


	std::vector<Vertex> vertices(NumSphereVertices);

	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].pos.x = 0.0f;
	vertices[0].pos.y = 0.0f;
	vertices[0].pos.z = 1.0f;

	for (DWORD i = 0; i < LatLines - 2; ++i)
	{
		spherePitch = (i + 1) * (3.14 / (LatLines - 1));
		Rotationx = XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < LongLines; ++j)
		{
			sphereYaw = j * (6.28 / (LongLines));
			Rotationy = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
			currVertPos = XMVector3Normalize(currVertPos);
			vertices[i * LongLines + j + 1].pos.x = XMVectorGetX(currVertPos);
			vertices[i * LongLines + j + 1].pos.y = XMVectorGetY(currVertPos);
			vertices[i * LongLines + j + 1].pos.z = XMVectorGetZ(currVertPos);
		}
	}

	vertices[NumSphereVertices - 1].pos.x = 0.0f;
	vertices[NumSphereVertices - 1].pos.y = 0.0f;
	vertices[NumSphereVertices - 1].pos.z = -1.0f;


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * NumSphereVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];
	hr = d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);


	std::vector<DWORD> indices(NumSphereFaces * 3);

	int k = 0;
	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = LongLines;
	indices[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < LatLines - 3; ++i)
	{
		for (DWORD j = 0; j < LongLines - 1; ++j)
		{
			indices[k] = i * LongLines + j + 1;
			indices[k + 1] = i * LongLines + j + 2;
			indices[k + 2] = (i + 1) * LongLines + j + 1;

			indices[k + 3] = (i + 1) * LongLines + j + 1;
			indices[k + 4] = i * LongLines + j + 2;
			indices[k + 5] = (i + 1) * LongLines + j + 2;

			k += 6; // next quad
		}

		indices[k] = (i * LongLines) + LongLines;
		indices[k + 1] = (i * LongLines) + 1;
		indices[k + 2] = ((i + 1) * LongLines) + LongLines;

		indices[k + 3] = ((i + 1) * LongLines) + LongLines;
		indices[k + 4] = (i * LongLines) + 1;
		indices[k + 5] = ((i + 1) * LongLines) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = NumSphereVertices - 1;
		indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
		indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
		k += 3;
	}

	indices[k] = NumSphereVertices - 1;
	indices[k + 1] = (NumSphereVertices - 1) - LongLines;
	indices[k + 2] = NumSphereVertices - 2;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];
	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndexBuffer);

}






using namespace DirectX;

EditorState::EditorState(ApplicationDataRef data)
	: _data(data)
{
}

void EditorState::init()
{
	showEditor = true;
	_renderTarget = new DX::RenderTarget(
		_data->D3Dgraphic->getDevice().Get(), _data->D3Dgraphic->getDeviceContext().Get(),
		500, 500, DX::to_wstring(_data->assetManager.GetShader("renderTarget")),
		DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT);
	_renderTarget->SetObjectName("RenderTarget");

	cameraData = new DX::ConstantBuffer<MVPData>(_data->D3Dgraphic->getDevice().Get());
	cameraData->SetObjectName("cbuffer_Cube");

	skyboxData = new DX::ConstantBuffer<MVPData>(_data->D3Dgraphic->getDevice().Get());
	skyboxData->SetObjectName("cbuffer_skybox");
	
	auto test = DX::LoadMeshFile("Assets/Models/Cube.mesh");
	
	mesh_test = new DX::Mesh(_data->D3Dgraphic->getDevice().Get() , DX::to_wstring(_data->assetManager.GetShader("cube")), "PSMain", "VSMain", test.vertices, test.indices);
	mesh_test->SetObjectName("mesh_object");
	
	skybox = new DX::Mesh(_data->D3Dgraphic->getDevice().Get() , DX::to_wstring(_data->assetManager.GetShader("skybox")), "PSMain", "VSMain", test.vertices, test.indices);
	skybox->SetObjectName("skybox_object");
	

	DX::Pixel pixels[] = {
		{ 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1},
		{ 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1},
		{ 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1},
		{ 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1},
		{ 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1},
		{ 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1},
		{ 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1},
		{ 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1},
		{ 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1},
		{ 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 1, 1, 1, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}, { 0, 0, 0, 1}
	};

	t = new DX::Texture(pixels, 10, 10, _data->D3Dgraphic->getDevice().Get());

	std::vector<std::string> files{
		"Assets/Gfx/skybox/right.jpg",
		"Assets/Gfx/skybox/left.jpg",
		"Assets/Gfx/skybox/top.jpg",
		"Assets/Gfx/skybox/bottom.jpg",
		"Assets/Gfx/skybox/front.jpg",
		"Assets/Gfx/skybox/back.jpg"
	};

	skybox_texture = new DX::Texture(files, _data->D3Dgraphic->getDevice().Get(), _data->D3Dgraphic->getDeviceContext().Get());

	point_sampler = new DX::SamplerState(_data->D3Dgraphic->getDevice(),
		D3D11_TEXTURE_ADDRESS_MIRROR,
		D3D11_TEXTURE_ADDRESS_MIRROR,
		D3D11_TEXTURE_ADDRESS_MIRROR,
		D3D11_FILTER_MIN_MAG_MIP_POINT
	);
	linear_sampler = new DX::SamplerState(_data->D3Dgraphic->getDevice(),
		D3D11_TEXTURE_ADDRESS_MIRROR,
		D3D11_TEXTURE_ADDRESS_MIRROR,
		D3D11_TEXTURE_ADDRESS_MIRROR,
		D3D11_FILTER_MIN_MAG_MIP_LINEAR
	);

	isTriangleList = true;
}

void EditorState::update(float dt)
{
	if (this->UpdateViewport) {
		float speed = 10;
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
			camera.AdjustRotation(0, -1 * dt * 0.2f, 0);
		}
		if (_data->window->IsKeyPressed(DIK_RIGHT)) {
			camera.AdjustRotation(0, 1 * dt * 0.2f, 0);
		}
	}

	_renderTarget->SetConstantBufferData(_data->D3Dgraphic->getDeviceContext().Get(), false, 1.0f, 2.1f, false, 128, dt);

	DirectX::XMMATRIX _p = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(45.0f),
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

	DirectX::XMStoreFloat4x4(&cameraData->data.projection, DirectX::XMMatrixTranspose(_p));
	DirectX::XMStoreFloat4x4(&cameraData->data.view, DirectX::XMMatrixTranspose(camera.GetViewMatrix()));
	DirectX::XMStoreFloat4x4(&cameraData->data.worldPos, DirectX::XMMatrixTranspose(_m));
	cameraData->ApplyChanges(_data->D3Dgraphic->getDeviceContext().Get());

	DirectX::XMStoreFloat4x4(&skyboxData->data.projection, DirectX::XMMatrixTranspose(_p));
	DirectX::XMStoreFloat4x4(&skyboxData->data.view, DirectX::XMMatrixTranspose(camera.GetViewMatrix()));
	DirectX::XMStoreFloat4x4(&skyboxData->data.worldPos, DirectX::XMMatrixTranspose(
			DirectX::XMMatrixScaling(4.0f, 4.0f, 4.0f)
		)
	);
	skyboxData->ApplyChanges(_data->D3Dgraphic->getDeviceContext().Get());
}

void EditorState::draw()
{
	_data->D3Dgraphic->UnBoundRenderTarget();
	float color[4] = { 0.3, 0.3, 0.3, 1 };

	_renderTarget->SetRenderTarget(_data->D3Dgraphic->getDevice().Get(), _data->D3Dgraphic->getDeviceContext().Get(), _data->window->GetWidth(), _data->window->GetHeight());
	_renderTarget->Clear(_data->D3Dgraphic->getDeviceContext().Get(), color);
	
	if (isLineList) _data->D3Dgraphic->SetPrimitiveMode(DX::PrimitiveMode::Line);
	else if (isTriangleList) _data->D3Dgraphic->SetPrimitiveMode(DX::PrimitiveMode::Triangle);
	else if (isPointList) _data->D3Dgraphic->SetPrimitiveMode(DX::PrimitiveMode::Point);
	else _data->D3Dgraphic->SetPrimitiveMode(DX::PrimitiveMode::Undefined);

	_data->D3Dgraphic->SetDepthStencilMode(DX::DepthStencilMode::DepthFirst);
	skyboxData->Bind(_data->D3Dgraphic->getDeviceContext().Get(), 0, DX::ConstantBuffer_BindType::VertexShader);
	skybox_texture->Bind(_data->D3Dgraphic->getDeviceContext().Get(), 0);
	linear_sampler->Bind(_data->D3Dgraphic->getDeviceContext().Get(), 0);
	skybox->Draw(_data->D3Dgraphic->getDeviceContext().Get(), 36, DX::PrimitiveMode::Undefined);
	_data->D3Dgraphic->SetDepthStencilMode(DX::DepthStencilMode::DepthOff);

	_data->D3Dgraphic->SetDepthStencilMode(DX::DepthStencilMode::DepthOn);
	cameraData->Bind(_data->D3Dgraphic->getDeviceContext().Get(), 0, DX::ConstantBuffer_BindType::VertexShader);
	t->Bind(_data->D3Dgraphic->getDeviceContext().Get(), 0);
	point_sampler->Bind(_data->D3Dgraphic->getDeviceContext().Get(), 0);
	mesh_test->Draw(_data->D3Dgraphic->getDeviceContext().Get(), 36, DX::PrimitiveMode::Undefined);
	_data->D3Dgraphic->SetDepthStencilMode(DX::DepthStencilMode::DepthOff);

	_renderTarget->UnBoundTarget(_data->D3Dgraphic->getDeviceContext().Get());

	_data->D3Dgraphic->SetRenderTarget();
	
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
		ImGui::BeginTabBar("#Tabbar1");
		if (ImGui::BeginTabItem("Scene")) {
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
		
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Rendering")) {
			ImGui::Text("D3D11 Primitive Topology");
			ImGui::Checkbox("Triangle List", &isTriangleList);
			ImGui::Checkbox("Line List", &isLineList);
			ImGui::Checkbox("Point List", &isPointList);

			if (isTriangleList) { isLineList = false; isPointList = false; }
			if (isLineList) { isTriangleList = false; isPointList = false; }
			if (isPointList) { isTriangleList = false; isLineList = false; }

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
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
		_renderTarget->Draw(_data->D3Dgraphic->getDeviceContext().Get());
		this->UpdateViewport = true;
	}

	_data->debugConsole.Draw();
}

void EditorState::destroy()
{
	delete mesh_test;
	delete skybox;
	delete cameraData;
	delete skyboxData;
	delete _renderTarget;
	delete t;
	delete skybox_texture;
	delete point_sampler;
	delete linear_sampler;
}