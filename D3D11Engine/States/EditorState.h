#pragma once
#include <Utils/States.h>
#include "../Application.h"

#include <Graphics/RenderTarget.h>

#include <Graphics/Mesh.h>

class EditorState : public DX::States
{
public:
	EditorState(ApplicationDataRef data);

	void init() override;
	void update(float dt) override;
	void draw() override;
	void destroy() override;

private:
	ApplicationDataRef _data;
	DX::RenderTarget<DX::RenderType::RenderTarget>* _renderTarget;
	DX::Mesh* triangle;
};