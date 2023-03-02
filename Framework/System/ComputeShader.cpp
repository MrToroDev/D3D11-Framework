#include "ComputeShader.h"
#include "../Graphics/GraphicLoader.h"
#include "../COMException.h"

DX::ComputeShader::ComputeShader(ID3D11Device* dev, ID3D11DeviceContext* devcon, std::wstring file, std::string entryPoint, ComputeBuffer* outputBufferData)
{
	devRef = dev;
	devconRef = devcon;

	DX::ComPtr<ID3DBlob> blob;
	if (!DX::CompileShaderFromFile(L"Assets/Shaders/Compute/TestCompute.hlsl", "CSMain", "cs_5_0", blob)) {
		return;
	}

	dev->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, _cShader.GetAddressOf());

	blob.Reset();

	// Create Structured Buffer

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = outputBufferData->size * outputBufferData->count;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = outputBufferData->size;

	if (outputBufferData->ptrData)
	{
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = outputBufferData->ptrData;
		dev->CreateBuffer(&desc, &InitData, this->_outputBuffer.GetAddressOf());
	}
	else
		dev->CreateBuffer(&desc, nullptr, this->_outputBuffer.GetAddressOf());

	// Create Output Buffer UAV

	D3D11_UNORDERED_ACCESS_VIEW_DESC descUAV = {};
	descUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	descUAV.Buffer.FirstElement = 0;

	if (desc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
	{
		// This is a Structured Buffer

		descUAV.Format = DXGI_FORMAT_UNKNOWN;      // Format must be must be DXGI_FORMAT_UNKNOWN, when creating a View of a Structured Buffer
		descUAV.Buffer.NumElements = desc.ByteWidth / desc.StructureByteStride;
	}
	else
	{
		throw DX::com_exception(0xCCC, __FILE__, __LINE__);
	}

	dev->CreateUnorderedAccessView(_outputBuffer.Get(), &descUAV, _outputBufUAV.GetAddressOf());
}

DX::ComputeShader::~ComputeShader()
{
	devRef = nullptr;
	devconRef = nullptr;

	_outputBuffer.Reset();
	_outputBufUAV.Reset();
	_cShader.Reset();
	for (auto buf : _buffers) {
		buf.pBuf.Reset();
		buf.pBufSRV.Reset();
	}
}

void* DX::ComputeShader::GetDataGPU()
{
	D3D11_BUFFER_DESC desvOutput;
	_outputBuffer->GetDesc(&desvOutput);

	ID3D11Buffer* outputBuffer;
	D3D11_BUFFER_DESC outputDesc;
	outputDesc.ByteWidth = desvOutput.ByteWidth;
	outputDesc.StructureByteStride = desvOutput.StructureByteStride;
	outputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	outputDesc.Usage = D3D11_USAGE_STAGING;
	outputDesc.BindFlags = 0;
	outputDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	devRef->CreateBuffer(&outputDesc, nullptr, &outputBuffer);

	devconRef->CopyResource(outputBuffer, _outputBuffer.Get());

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = devconRef->Map(outputBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);
	void* data;
	if (SUCCEEDED(hr))
	{
		data = mappedResource.pData;

		devconRef->Unmap(outputBuffer, 0);
	}

	outputBuffer->Release();
	outputBuffer = 0;

	return data;
}
