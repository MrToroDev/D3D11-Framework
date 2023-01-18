#include "GraphicLoader.h"
#include <string_view>
#include <string>
#include <sstream>
#include <d3dcompiler.h>
#include <wrl.h>
#include <iostream>
#include <fstream>
#include "../System/Math.h"
#include "../COMException.h"
#include "../Logger.h"

#pragma comment(lib, "d3dcompiler.lib")

namespace DX
{
	bool CompileShaderFromFile(
		const std::wstring_view fileName,
		const std::string_view entryPoint,
		const std::string_view profile,
		Microsoft::WRL::ComPtr<ID3DBlob>& shaderBlob)
	{
		UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
		compileFlags |= D3DCOMPILE_DEBUG;
#endif
		Microsoft::WRL::ComPtr<ID3DBlob> tempShaderBlob = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
		HRESULT hr = D3DCompileFromFile(
			fileName.data(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint.data(),
			profile.data(),
			compileFlags,
			0,
			tempShaderBlob.GetAddressOf(),
			errorBlob.GetAddressOf());

		if (FAILED(hr)) {
			if (errorBlob.Get())
			{
				const char* errorMsg = static_cast<const char*>(errorBlob->GetBufferPointer());
				DX::LogError(errorMsg);
				MessageBoxA(NULL, errorMsg, "Shading Compile Error", MB_ICONWARNING | MB_OK);
				errorBlob->Release();
			}
			else {
				const char* errorMsg = "Error! The Shader loader had an error!, maybe wrong path of the shader?";
				DX::LogError(errorMsg);
				MessageBoxA(NULL, errorMsg, "Shading Compile Error", MB_ICONWARNING | MB_OK);
			}

			if (tempShaderBlob.Get())
				tempShaderBlob->Release();

			return false;
		}

		shaderBlob = std::move(tempShaderBlob);
		return true;
	}

	bool CompileShaderFromStream(
		const std::vector<unsigned char> fileData,
		const std::string_view entryPoint,
		const std::string_view profile,
		Microsoft::WRL::ComPtr<ID3DBlob>& shaderBlob)
	{
		UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
		compileFlags |= D3DCOMPILE_DEBUG;
#endif
		Microsoft::WRL::ComPtr<ID3DBlob> tempShaderBlob = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

		HRESULT hr = D3DCompile(
			fileData.data(), fileData.size(), NULL, nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.data(), profile.data(),
			compileFlags, 0, tempShaderBlob.GetAddressOf(), errorBlob.GetAddressOf());

		if (FAILED(hr)) {
			if (errorBlob.Get())
			{
				const char* errorMessage = static_cast<const char*>(errorBlob->GetBufferPointer());
				DX::LogError(errorMessage);
				MessageBoxA(NULL, errorMessage, "Shading Compile Error", MB_ICONWARNING | MB_OK);
				errorBlob->Release();
			}
			else {
				const char* errorMsg = "Error! The Shader loader had an error!, maybe wrong path of the shader?";
				DX::LogError(errorMsg);
				MessageBoxA(NULL, errorMsg, "Shading Compile Error", MB_ICONWARNING | MB_OK);
			}

			if (tempShaderBlob.Get())
				tempShaderBlob->Release();

			return false;
		}
		shaderBlob = std::move(tempShaderBlob);
		return true;
	}

	std::vector<unsigned char> ReadFile(std::string file)
	{
		std::vector<unsigned char> Text;
		std::fstream File(file, std::ios::in | std::ios::ate | std::ios::binary);
		if (File.is_open()) {
			Text.resize(File.tellg());
			File.seekg(0, std::ios::beg);
			File.read(reinterpret_cast<char*>(&Text[0]), Text.size());
			File.close();
		}
		return Text;
	}

	MeshData LoadMeshFile(std::string filename)
	{
		std::ifstream file(filename);
		MeshData _data{};
		int vSize = 0, iSize = 0;
		bool initializedAttributes = false;
		std::vector<hlsl::float3> vert, color, normal;
		std::vector<hlsl::float2> uv;
		std::vector<int> index;

		if (file.is_open())
		{
			std::string line;
			while (true) {
				std::getline(file, line);

				if (line.find("//") != std::string::npos) {
					// Do nothing, is a comment.
				}

				if (line.find("MESH_ATTRIBUTES") != std::string::npos && !initializedAttributes) {
					std::string attrName; int attrNum;
					while (true) {
						file >> attrName >> attrNum;
						if (attrName == "_vertices") {
							vSize = attrNum;
						}
						if (attrName == "_index") {
							iSize = attrNum;
						}

						if (attrName == "MESH_END_ATTRIBUTES") {
							initializedAttributes = true;
							file.clear();
							file.seekg(0, std::ios::beg);
							break;
						}
					}
				}

				if (line.find("Vertices") != std::string::npos && vert.empty()) {
					for (int i = 0; i < vSize; i++) {
						float x, y, z;
						file >> x >> y >> z;
						vert.push_back({ x, y, z });
					}
					file.clear();
					file.seekg(0, std::ios::beg);
				}

				if (line.find("Normals") != std::string::npos && normal.empty()) {
					for (int i = 0; i < vSize; i++) {
						float x, y, z;
						file >> x >> y >> z;
						normal.push_back({ x, y, z });
					}
					file.clear();
					file.seekg(0, std::ios::beg);
				}

				if (line.find("UV") != std::string::npos && uv.empty()) {
					for (int i = 0; i < vSize; i++) {
						float _u, _v;
						file >> _u >> _v;
						uv.push_back({ _u, _v });
					}
					file.clear();
					file.seekg(0, std::ios::beg);
				}

				if (line.find("Color") != std::string::npos && color.empty()) {
					for (int i = 0; i < vSize; i++) {
						float x, y, z;
						file >> x >> y >> z;
						color.push_back({ x, y, z });
					}
					file.clear();
					file.seekg(0, std::ios::beg);
				}

				if (line.find("Indices") != std::string::npos && index.empty()) {
					for (int i = 0; i < iSize; i++) {
						int _index1, _index2, _index3;

						file >> _index1 >> _index2 >> _index3;

						index.push_back(_index1);
						index.push_back(_index2);
						index.push_back(_index3);
					}
					file.clear();
					file.seekg(0, std::ios::beg);
				}

				if (line.find("END_MESH_FILE") != std::string::npos) break;
			}

			for (int i = 0; i < vSize; i++) {
				hlsl::float3 n = (normal.empty()) ? hlsl::float3(0.0f, 0.0f, 0.0f) : normal[i];
				hlsl::float3 c = (color.empty()) ? hlsl::float3(0.0f, 0.0f, 0.0f) : color[i];
				hlsl::float2 _uv = (uv.empty()) ? hlsl::float2(0.0f, 0.0f) : uv[i];

				_data.vertices.push_back(VertexTexture(vert[i], _uv, n, c));
			}

			for (int i = 0; i < iSize; i++) {
				_data.indices.push_back(index[i]);
			}
		}
		else {
			DX::LogCritic("ERROR PARSING MESH FILE: FILE NOT FOUND, MAYBE WRONG PATH?");
		}
		file.close();
		return _data;
	}
}