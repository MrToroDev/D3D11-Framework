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
		ComputeShader(ID3D11Device* dev, ID3D11DeviceContext* devcon, std::wstring file, std::string entryPoint, ComputeBuffer* outputBufferData);
		~ComputeShader();

		void AddBuffer(ComputeBuffer* buf);

		void RunProgram();

		// Need to be reinterpret cast
		void* GetDataGPU();

	private:
		DX::ComPtr<ID3D11ComputeShader> _cShader;
		DX::ComPtr <ID3D11Buffer> _outputBuffer = nullptr;
		DX::ComPtr < ID3D11UnorderedAccessView> _outputBufUAV = nullptr;
		ID3D11Device* devRef;
		ID3D11DeviceContext* devconRef;

		struct _Buf
		{
			DX::ComPtr <ID3D11Buffer> pBuf;
			DX::ComPtr <ID3D11ShaderResourceView> pBufSRV;
		};
		std::vector<_Buf> _buffers;
	};
}