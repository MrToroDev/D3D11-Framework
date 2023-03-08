#include "TestsState.h"
#include <Graphics/GraphicLoader.h>

TestsState::TestsState(ApplicationDataRef data)
	: _data(data)
{
}

void TestsState::init()
{
	for (int i = 0; i < 1024; ++i)
	{
		g_vBuf0[i].j = i;
		g_vBuf0[i].f = (float)i;

		g_vBuf1[i].j = i;
		g_vBuf1[i].f = (float)i;
	}

	DX::ComputeBuffer outBuf;
	outBuf.count = 1024;
	outBuf.size = sizeof(BufType);
	outBuf.ptrData = nullptr;

	compute = new DX::ComputeShader(_data->D3Dgraphic->getDevice().Get(), _data->D3Dgraphic->getDeviceContext().Get(), L"Assets/Shaders/Compute/TestCompute.hlsl", "CSMain", &outBuf);
	DX::ComputeBuffer buf1, buf2;
	buf1.count = 1024;
	buf1.size = sizeof(BufType);
	buf1.ptrData = &g_vBuf0;
	buf2.count = 1024;
	buf2.size = sizeof(BufType);
	buf2.ptrData = &g_vBuf1;
	compute->AddBuffer(&buf1);
	compute->AddBuffer(&buf2);

	compute->RunProgram(1, 1, 1);

	BufType* data = reinterpret_cast<BufType*>(compute->GetDataGPU());
	std::string dataString = "Some Data: ";
	dataString.append(std::to_string(data[2].f));
	DX::LogInfo(dataString);
}

void TestsState::update(float dt)
{

}

void TestsState::draw()
{
	_data->debugConsole.Draw();
	compute->RunProgram(1, 1, 1);
}

void TestsState::destroy()
{
	delete compute;
}