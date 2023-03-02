#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "../COMException.h"

namespace DX {
	enum class ConstantBuffer_BindType
	{
		VertexShader,
		PixelShader,
		ComputeShader
	};

	template<class T>
	class ConstantBuffer
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

	public:
		ConstantBuffer(ID3D11Device* device);
		~ConstantBuffer();

		void SetObjectName(const char name[256]);

		T data;

		ID3D11Buffer* Get()const;
		ID3D11Buffer* const* GetAddressOf()const;
		void ApplyChanges(ID3D11DeviceContext* deviceContext);

		void Bind(ID3D11DeviceContext* devcon, int count, ConstantBuffer_BindType type);
	};
}

#include "ConstantBuffer.inl"