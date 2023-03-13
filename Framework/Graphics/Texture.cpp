#include "Texture.h"
#include "../COMException.h"
#include "../Logger.h"
#include "../Utils/Utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <future>

void DX::Texture::initSRV(ID3D11Device* dev, const char* file)
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

void DX::Texture::initSRV(ID3D11Device* dev, unsigned char*& bytes, int w, int h)
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

void DX::Texture::initSRV(ID3D11Device* dev, Pixel*& bytes, int w, int h)
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

void DX::Texture::initSRV(ID3D11Device* dev, ID3D11DeviceContext* devcon, std::vector<std::string>& files)
{
	struct
	{
		int width, height, nrChannels;
		unsigned char* data;
	} fileData[6];

	for (int i = 0; i < 6; i++) {
		fileData[i].data = stbi_load(files[0].c_str(), &fileData[i].width, &fileData[i].height, &fileData[i].nrChannels, STBI_rgb_alpha);
		assert(fileData[i].data && "Couldn't load file, maybe wrong path?");
	}

	ID3D11Texture2D* texture = nullptr;
	
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = fileData[0].width;
	textureDesc.Height = fileData[0].height;
	textureDesc.ArraySize = 6;
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	D3D11_SUBRESOURCE_DATA subresource_data[6];
	for (int i = 0; i < 6; i++) {
		subresource_data[i].pSysMem = fileData[i].data;
		subresource_data[i].SysMemPitch = fileData[i].width * 4;
		subresource_data[i].SysMemSlicePitch = 0;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = textureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = textureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	DX_CHECK(dev->CreateTexture2D(&textureDesc, subresource_data, &texture));
	assert(texture);
	DX_CHECK(dev->CreateShaderResourceView(
		texture, &SMViewDesc, this->textureSRV.GetAddressOf()
	));

	for (auto f : fileData) {
		stbi_image_free(f.data);
	}
	texture->Release();
}

DX::Texture::Texture(const char* file, ID3D11Device* dev)
{
	this->initSRV(dev, file);
}

DX::Texture::Texture(unsigned char* bytes, int w, int h, ID3D11Device* dev)
{
	this->initSRV(dev, bytes, w, h);
}

DX::Texture::Texture(Pixel* pixels, int width, int height, ID3D11Device* dev)
{
	this->initSRV(dev, pixels, width, height);
}

DX::Texture::Texture(std::vector<std::string> files, ID3D11Device* dev, ID3D11DeviceContext* devcon)
{
	this->initSRV(dev, devcon, files);
}

DX::Texture::~Texture()
{
	if (textureSRV) this->textureSRV.Reset();
}

void DX::Texture::SetObjectName(const char name[256])
{
	char n[256];
	sprintf_s(n, "%s_SRV", name);
	SetDebugObjectName(textureSRV.Get(), n);
}

void DX::Texture::Bind(ID3D11DeviceContext* devcon, int count)
{
	devcon->PSSetShaderResources(count, 1, textureSRV.GetAddressOf());
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> DX::Texture::GetShaderResourceView() const
{
	return this->textureSRV;
}
