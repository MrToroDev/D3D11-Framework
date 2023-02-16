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
	ID3D11Texture2D* texture[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, };
	ID3D11Texture2D* cubeTexture = nullptr;

	for (int i = 0; i < 6; i++) {
		int width, height, nrChannels;
		unsigned char* data = stbi_load(files[i].c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
		assert(data && " -> Maybe wrong path or wrong name of the file!");

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.ArraySize = 1;
		textureDesc.MipLevels = 1;
		textureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		D3D11_SUBRESOURCE_DATA subresource_data = {};
		subresource_data.pSysMem = data;
		subresource_data.SysMemPitch = width * 4;
		subresource_data.SysMemSlicePitch = 0;
		DX_CHECK(dev->CreateTexture2D(&textureDesc, &subresource_data, &texture[i]));
		assert(texture);
	}


	D3D11_TEXTURE2D_DESC texDesc1 = {};
	texture[0]->GetDesc(&texDesc1);

	D3D11_TEXTURE2D_DESC texDesc = {};
	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc = {};
	texDesc.Width = texDesc1.Width;
	texDesc.Height = texDesc1.Height;
	texDesc.MipLevels = texDesc1.MipLevels;
	texDesc.ArraySize = 6;
	texDesc.Format = texDesc1.Format;
	texDesc.CPUAccessFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	SMViewDesc.Format = texDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = texDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;
	DX_CHECK(dev->CreateTexture2D(&texDesc, nullptr, &cubeTexture));

	for (int i = 0; i < 6; i++)
	{
		for (UINT mipLevel = 0; mipLevel < texDesc.MipLevels; ++mipLevel)
		{
			D3D11_MAPPED_SUBRESOURCE mappedTex2D;
			HRESULT hr = (devcon->Map(texture[i], mipLevel, D3D11_MAP_READ, 0, &mappedTex2D));
			assert(SUCCEEDED(hr));
			devcon->UpdateSubresource(cubeTexture,
				D3D11CalcSubresource(mipLevel, i, texDesc.MipLevels),
				0, mappedTex2D.pData, mappedTex2D.RowPitch, mappedTex2D.DepthPitch);

			devcon->Unmap(texture[i], mipLevel);
		}
	}

	DX_CHECK(dev->CreateShaderResourceView(
		cubeTexture, &SMViewDesc, this->textureSRV.GetAddressOf()
	));

	for (int i = 0; i < 6; i++) texture[i]->Release();
	cubeTexture->Release();
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
