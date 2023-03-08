#include "ComputeShader.h"
#include "../Graphics/GraphicLoader.h"
#include "../Utils/Utils.h"
#include "../COMException.h"

DX::ComputeShader::ComputeShader(DX::ComPtr <ID3D11Device> dev, DX::ComPtr <ID3D11DeviceContext> devcon, std::wstring file, std::string entryPoint, ComputeBuffer* outputBufferData)
{
	devRef = dev;
	devconRef = devcon;
	_buffersSRV.clear();

	DX::ComPtr<ID3DBlob> blob;
	if (!DX::CompileShaderFromFile(L"Assets/Shaders/Compute/TestCompute.hlsl", "CSMain", "cs_5_0", blob)) {
		return;
	}

	DX_CHECK(dev->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, _cShader.GetAddressOf()));

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
		DX_CHECK(dev->CreateBuffer(&desc, &InitData, this->_outputBuffer.GetAddressOf()));
	}
	else
		DX_CHECK(dev->CreateBuffer(&desc, nullptr, this->_outputBuffer.GetAddressOf()));

	assert(_outputBuffer.Get());

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

	DX_CHECK(dev->CreateUnorderedAccessView(_outputBuffer.Get(), &descUAV, _outputBufUAV.GetAddressOf()));
	assert(_outputBufUAV.Get());
}

DX::ComputeShader::~ComputeShader()
{
	_outputBuffer.Reset();
	_outputBufUAV.Reset();
	_cShader.Reset();

	devRef = nullptr;
	devconRef = nullptr;
}

void DX::ComputeShader::AddBuffer(ComputeBuffer* buf)
{
	ID3D11Buffer* _buffer;
	ID3D11ShaderResourceView* _srv;
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = buf->size * buf->count;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = buf->size;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = buf->ptrData;
	DX_CHECK(devRef->CreateBuffer(&desc, &InitData, &_buffer));
	assert(_buffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV = {};
	descSRV.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	descSRV.BufferEx.FirstElement = 0;
	// This is a Structured Buffer
	descSRV.Format = DXGI_FORMAT_UNKNOWN;
	descSRV.BufferEx.NumElements = desc.ByteWidth / desc.StructureByteStride;

	DX_CHECK(devRef->CreateShaderResourceView(_buffer, &descSRV, &_srv));
	assert(_srv);

	_buffersSRV.push_back(_srv);

	_buffer->Release();
	_srv->Release();
}

void DX::ComputeShader::RunProgram(UINT Tx, UINT Ty, UINT Tz)
{
	devconRef->CSSetShader(this->_cShader.Get(), nullptr, 0);
	//if (!_buffersSRV.empty()) {
		//ID3D11ShaderResourceView* srv[16];
		//std::copy(_buffersSRV.begin(), _buffersSRV.end(), srv);

		for (int i = 0; i < _buffersSRV.size(); i++) {
			devconRef->CSSetShaderResources(i, 1, _buffersSRV[i].GetAddressOf());
		}
	//}

	//devconRef->CSSetShaderResources(0, _buffersSRV.size(), _buffersSRV);
	devconRef->CSSetUnorderedAccessViews(0, 1, _outputBufUAV.GetAddressOf(), 0);
	devconRef->Dispatch(Tx, Ty, Tz);
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
	DX_CHECK(devRef->CreateBuffer(&outputDesc, nullptr, &outputBuffer));
	assert(outputBuffer);

	devconRef->CopyResource(outputBuffer, _outputBuffer.Get());

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = devconRef->Map(outputBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);
	void* data = malloc(mappedResource.RowPitch);
	if (SUCCEEDED(hr))
	{
		data = mappedResource.pData;

		devconRef->Unmap(outputBuffer, 0);
	}

	outputBuffer->Release();
	outputBuffer = 0;

	return data;
}
