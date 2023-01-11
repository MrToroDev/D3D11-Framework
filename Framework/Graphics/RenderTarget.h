#pragma once

#include <wrl.h>
#include <d3d11.h>
#include "Mesh.h"
#include "ConstantBuffer.h"

namespace DX {
	enum class RenderType
	{
		RenderTarget,
		DepthStencil // DO NOT USE, IT IS BUGGED NEED TO BE FIXED
	};

	template<RenderType T>
	class RenderTarget
	{
	private:
		struct HDRData
		{
			bool hdr;
			float exposure;
			float gamma;
			bool mosaic;
			float mosaicStrong;
			float clock;
			DirectX::XMVECTOR padding;
		};

	private:
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backbuffer = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backbufferTexture = nullptr;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> backbufferSRV = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer = nullptr;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> textureST = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState = nullptr;
		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState = nullptr;
		Mesh* targetQuad = nullptr;
		ConstantBuffer<HDRData> renderData;
		DXGI_FORMAT textureFormat;
		RenderType textureType;

		void CreateTexture(Microsoft::WRL::ComPtr<ID3D11Device> dev, int width, int height, DXGI_FORMAT textureFormat);
		void CreateDepthStencilBuffer(Microsoft::WRL::ComPtr<ID3D11Device> dev, int width, int height);
	public:
		RenderTarget() = delete;
		RenderTarget(Microsoft::WRL::ComPtr<ID3D11Device> dev, Microsoft::WRL::ComPtr<ID3D11DeviceContext> devcon, 
					 int width, int height, std::wstring shader, DXGI_FORMAT textureFormat);
		~RenderTarget();

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTextureSRV();

		void SetConstantBufferData(Microsoft::WRL::ComPtr<ID3D11DeviceContext> devcon, bool hdr, float hdrExposure, float gamma, bool mosaic, float mosaicStrenght, float clock);
		void Clear(Microsoft::WRL::ComPtr<ID3D11DeviceContext> devcon, float color[4]);
		void SetRenderTarget(Microsoft::WRL::ComPtr<ID3D11Device> dev, Microsoft::WRL::ComPtr<ID3D11DeviceContext> devcon, int width, int height);
		void SetBlendState(Microsoft::WRL::ComPtr<ID3D11DeviceContext> devcon, float blendFactor[4]);
		void UnBoundBlendState(Microsoft::WRL::ComPtr<ID3D11DeviceContext> devcon);
		void UnBoundTarget(Microsoft::WRL::ComPtr<ID3D11DeviceContext> devcon);
		void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> devcon);
	};
}

#include "RenderTarget.inl"