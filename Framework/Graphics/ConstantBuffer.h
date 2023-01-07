#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "../COMException.h"

namespace DX {
	enum class ConstantBuffer_BindType
	{
		VertexShader,
		PixelShader
	};

	template<class T>
	class ConstantBuffer
	{
	private:
		ConstantBuffer(const ConstantBuffer<T>& rhs);

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

	public:
		ConstantBuffer() {}

		T data;

		ID3D11Buffer* Get()const;
		ID3D11Buffer* const* GetAddressOf()const;
		HRESULT Initialize(ID3D11Device* device);
		void ApplyChanges(ID3D11DeviceContext* deviceContext);

		void Bind(ID3D11DeviceContext* devcon, int count, ConstantBuffer_BindType type);
	};
}

#include "ConstantBuffer.inl"