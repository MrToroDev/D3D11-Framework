#pragma once
#include <Utils/States.h>
#include "../Application.h"

#include <Graphics/RenderTarget.h>
#include <Graphics/ConstantBuffer.h>
#include <Graphics/Mesh.h>
#include <Graphics/Camera.h>
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

	float tX;

private:
	bool showEditor;
	ApplicationDataRef _data;
	DX::RenderTarget<DX::RenderType::RenderTarget>* _renderTarget;
	DX::Mesh* triangle;
	DX::Mesh* cube;
	DX::ConstantBuffer<MVPData> cbuffer_Cube;
	DX::Camera camera;
	DirectX::DX11::SpriteBatch* spriteBatch;
	DirectX::DX11::SpriteFont* spriteFont;
};