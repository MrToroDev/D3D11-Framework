#pragma once

#include <d3d11.h>
#include <wrl.h>
#include "Vertex.h"

namespace DX {
	class Texture
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureSRV = nullptr;
		void initSRV(Microsoft::WRL::ComPtr<ID3D11Device> dev, const char* file);
		void initSRV(Microsoft::WRL::ComPtr<ID3D11Device> dev, unsigned char*& bytes, int width, int height);
		void initSRV(Microsoft::WRL::ComPtr<ID3D11Device> dev, Pixel*& bytes, int width, int height);

	public:
		Texture(const char* file, Microsoft::WRL::ComPtr<ID3D11Device> dev);
		Texture(unsigned char* bytes, int width, int height, Microsoft::WRL::ComPtr<ID3D11Device> dev);
		Texture(Pixel* pixels, int width, int height, Microsoft::WRL::ComPtr<ID3D11Device> dev);
		~Texture();

		void SetObjectName(const char name[256]);

		void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> devcon, int count);
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() const;
	};
}

