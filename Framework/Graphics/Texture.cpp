#include "Texture.h"
#include "../COMException.h"
#include "../Logger.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <future>

void DX::Texture::initSRV(Microsoft::WRL::ComPtr<ID3D11Device> dev, const char* file)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, STBI_rgb_alpha);
	assert(data && " -> Maybe wrong path or wrong name of the file!");
	DX::Log("Loaded texture: " + std::string(file));

	ID3D11Texture2D* texture = nullptr;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA subresource_data = {};
	subresource_data.pSysMem = data;
	subresource_data.SysMemPitch = width * 4;

	DX_CHECK(dev->CreateTexture2D(&textureDesc, &subresource_data, &texture));
	assert(texture);
	DX_CHECK(dev->CreateShaderResourceView(
		texture, nullptr, this->textureSRV.GetAddressOf()
	));

	texture->Release();
}

void DX::Texture::initSRV(Microsoft::WRL::ComPtr<ID3D11Device> dev, unsigned char*& bytes, int w, int h)
{
	ID3D11Texture2D* texture = nullptr;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = w;
	textureDesc.Height = h;
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA subresource_data = {};
	subresource_data.pSysMem = bytes;
	subresource_data.SysMemPitch = w;
	subresource_data.SysMemSlicePitch = 0;
	DX_CHECK(dev->CreateTexture2D(&textureDesc, &subresource_data, &texture));
	assert(texture);
	DX_CHECK(dev->CreateShaderResourceView(
		texture, nullptr, this->textureSRV.GetAddressOf()
	));

	texture->Release();
}

void DX::Texture::initSRV(Microsoft::WRL::ComPtr<ID3D11Device> dev, Pixel*& bytes, int w, int h)
{
	ID3D11Texture2D* texture = nullptr;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = w;
	textureDesc.Height = h;
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA subresource_data = {};
	subresource_data.pSysMem = bytes;
	subresource_data.SysMemPitch = w * sizeof(Pixel);
	subresource_data.SysMemSlicePitch = 0;
	DX_CHECK(dev->CreateTexture2D(&textureDesc, &subresource_data, &texture));
	assert(texture);
	DX_CHECK(dev->CreateShaderResourceView(
		texture, nullptr, this->textureSRV.GetAddressOf()
	));

	texture->Release();
}

DX::Texture::Texture(const char* file, Microsoft::WRL::ComPtr<ID3D11Device> dev)
{
	this->initSRV(dev, file);
}

DX::Texture::Texture(unsigned char* bytes, int w, int h, Microsoft::WRL::ComPtr<ID3D11Device> dev)
{
	this->initSRV(dev, bytes, w, h);
}

DX::Texture::Texture(Pixel* pixels, int width, int height, Microsoft::WRL::ComPtr<ID3D11Device> dev)
{
	this->initSRV(dev, pixels, width, height);
}

DX::Texture::~Texture()
{
	this->textureSRV.Reset();
}

void DX::Texture::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> devcon, int count)
{
	devcon->PSSetShaderResources(count, 1, textureSRV.GetAddressOf());
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> DX::Texture::GetShaderResourceView() const
{
	return this->textureSRV;
}
