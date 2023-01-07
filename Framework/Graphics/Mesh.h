#pragma once
#include <wrl.h>
#include <d3d11_2.h>
#include <string>
#include <vector>
#include "Vertex.h"

namespace DX
{
	class Mesh
	{
	protected:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader = nullptr;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBlob = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderBlob = nullptr;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Buffer> Vbuffer = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Buffer> Ibuffer = nullptr;
		bool isVertexRawFloat = false;

	private:
		void initShaders(Microsoft::WRL::ComPtr<ID3D11Device> dev, std::wstring p, std::wstring v, std::string vmain, std::string pmain);
		void initBuffer(Microsoft::WRL::ComPtr<ID3D11Device> dev, std::vector<VertexTexture> v, std::vector<int> indices);
		void initBuffer(Microsoft::WRL::ComPtr<ID3D11Device> dev, float* v, std::vector<int> indices);
		void initInputLayout(Microsoft::WRL::ComPtr<ID3D11Device> dev);
		void initInputLayout_V(Microsoft::WRL::ComPtr<ID3D11Device> dev);

	public:
		Mesh(
			Microsoft::WRL::ComPtr<ID3D11Device> dev,
			std::wstring pixelShader, std::string psmain,
			std::wstring vertexShader, std::string vsmain,
			std::vector<VertexTexture> v, std::vector<int> indices);
		Mesh(
			Microsoft::WRL::ComPtr<ID3D11Device> dev,
			std::wstring ShaderFile, std::string psmain, std::string vsmain,
			std::vector<VertexTexture> v, std::vector<int> indices);
		Mesh(
			Microsoft::WRL::ComPtr<ID3D11Device> dev,
			std::wstring ShaderFile, std::string psmain, std::string vsmain,
			float* v, std::vector<int> indices);

		~Mesh();

		void prepareDraw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> devcon);
		void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> devcon, int vertCount);
	};
}