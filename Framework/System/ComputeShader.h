#pragma once

#include <d3d11.h>
#include <vector>
#include <string>
#include "../Window/Graphic.h"

namespace DX
{
	struct ComputeBuffer
	{
		unsigned int size, count;
		void* ptrData;
	};

	class ComputeShader
	{
	public:
		ComputeShader(DX::ComPtr <ID3D11Device> dev, DX::ComPtr <ID3D11DeviceContext> devcon, std::wstring file, std::string entryPoint, ComputeBuffer* outputBufferData);
		~ComputeShader();

		void AddBuffer(ComputeBuffer* buf);

		void RunProgram(UINT Tx, UINT Ty, UINT Tz);

		// Need to be reinterpret cast
		void* GetDataGPU();

	private:
		DX::ComPtr<ID3D11ComputeShader> _cShader;
		DX::ComPtr <ID3D11Buffer> _outputBuffer = nullptr;
		DX::ComPtr < ID3D11UnorderedAccessView> _outputBufUAV = nullptr;
		DX::ComPtr<ID3D11Device> devRef;
		DX::ComPtr<ID3D11DeviceContext> devconRef;

		std::vector<DX::ComPtr<ID3D11ShaderResourceView>> _buffersSRV;
	};
}