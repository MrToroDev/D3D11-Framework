#pragma once

#include <d3d11.h>
#include <vector>
#include <string>
#include <wrl.h>
#include "Vertex.h"

namespace DX {
	class Texture
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureSRV = nullptr;
		void initSRV(ID3D11Device* dev, const char* file);
		void initSRV(ID3D11Device* dev, unsigned char*& bytes, int width, int height);
		void initSRV(ID3D11Device* dev, Pixel*& bytes, int width, int height);
		void initSRV(ID3D11Device* dev, ID3D11DeviceContext* devcon, std::vector<std::string>& files);

	public:
		Texture(const char* file, ID3D11Device* dev);
		Texture(unsigned char* bytes, int width, int height, ID3D11Device* dev);
		Texture(Pixel* pixels, int width, int height, ID3D11Device* dev);
		Texture(std::vector<std::string> files, ID3D11Device* dev, ID3D11DeviceContext* devcon); // Skybox
		~Texture();

		void SetObjectName(const char name[256]);

		void Bind(ID3D11DeviceContext* devcon, int count);
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() const;
	};
}

