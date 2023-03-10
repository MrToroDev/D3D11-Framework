#include "../Logger.h"
#include <sstream>
#include "../Memory.h"
#include "../Utils/Utils.h"
#include "ConstantBuffer.h"

template<class T>
inline DX::ConstantBuffer<T>::~ConstantBuffer()
{
	Memory::Destroy(buffer);
}

template<class T>
inline DX::ConstantBuffer<T>::ConstantBuffer(ID3D11Device* device)
{
	std::stringstream ss;
	ss << "[COSTANT-BUFFER] Allocating " << sizeof(T) << " bytes";
	DX::LogDebugMessage(ss.str());

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.ByteWidth = sizeof(T);
	desc.StructureByteStride = 0;

	HRESULT hr = device->CreateBuffer(&desc, 0, buffer.GetAddressOf());
	DX_CHECK(hr);
}

template<class T>
inline void DX::ConstantBuffer<T>::SetObjectName(const char name[256])
{
	char n[256];
	sprintf_s(n, "%s_Buffer", name);
	SetDebugObjectName(buffer.Get(), n);
}

template<class T>
ID3D11Buffer* DX::ConstantBuffer<T>::Get() const
{
	return buffer.Get();
}

template<class T>
ID3D11Buffer* const* DX::ConstantBuffer<T>::GetAddressOf() const
{
	return buffer.GetAddressOf();
}

template<class T>
void DX::ConstantBuffer<T>::ApplyChanges(ID3D11DeviceContext* deviceContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	DX_CHECK(hr);
	memcpy(mappedResource.pData, &data, sizeof(T));
	deviceContext->Unmap(buffer.Get(), 0);	
}

template<class T>
void DX::ConstantBuffer<T>::Bind(ID3D11DeviceContext* devcon, int count, ConstantBuffer_BindType type)
{
	if (type == ConstantBuffer_BindType::VertexShader) {
		devcon->VSSetConstantBuffers(count, 1, buffer.GetAddressOf());
	}
	else if (type == ConstantBuffer_BindType::PixelShader) {
		devcon->PSSetConstantBuffers(count, 1, buffer.GetAddressOf());
	}
	else if (type == ConstantBuffer_BindType::ComputeShader) {
		devcon->CSSetConstantBuffers(count, 1, buffer.GetAddressOf());
	}
}
