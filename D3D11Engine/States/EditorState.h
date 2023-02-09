#pragma once
#include <Utils/States.h>
#include "../Application.h"

#include <Graphics/RenderTarget.h>
#include <Graphics/ConstantBuffer.h>
#include <Graphics/Mesh.h>
#include <Graphics/Camera.h>
#include <Graphics/Texture.h>
#include <Graphics/SamplerState.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

class EditorState : public DX::States
{
public:
	EditorState(ApplicationDataRef data);

	void init() override;
	void update(float dt) override;
	void draw() override;
	void destroy() override;

private:
	struct MVPData
	{
		DirectX::XMFLOAT4X4 projection, view, worldPos;
	};

	struct CubeTransform
	{
		DX::hlsl::float3 Rotation = { 0, 0, 0 }, Position = { 0, 0, 0 }, Scale = { 1, 1, 1 };
	};

private:
	CubeTransform _cubeT;
	bool showEditor;
	bool UpdateViewport;
	ApplicationDataRef _data;
	DX::RenderTarget<DX::RenderType::RenderTarget>* _renderTarget;
	DX::ConstantBuffer<MVPData> cbuffer_Cube;
	DX::ConstantBuffer<MVPData> cbuffer_Cube2;
	DX::Camera camera;
	DX::Mesh* mesh_test;
	DX::Mesh* billboard;
	DX::Mesh* plane;
	DX::SamplerState* sampler;
	DX::Texture* billboard_texture;
	DirectX::DX11::SpriteBatch* spriteBatch;
	DirectX::DX11::SpriteFont* spriteFont;
};