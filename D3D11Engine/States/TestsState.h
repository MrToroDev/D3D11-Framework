#pragma once

#include <d3d11.h>
#include "../Application.h"
#include <System/ComputeShader.h>

class TestsState : public DX::States
{
public:
	TestsState(ApplicationDataRef data);

	void init() override;
	void update(float dt) override;
	void draw() override;
	void destroy() override;

private:
	ApplicationDataRef _data;

	// https://github.com/walbourn/directx-sdk-samples/blob/main/BasicCompute11/BasicCompute11.cpp

	struct BufType
	{
		float f, j;
	};
	BufType g_vBuf0[1024];
	BufType g_vBuf1[1024];
	DX::ComputeShader* compute;
};