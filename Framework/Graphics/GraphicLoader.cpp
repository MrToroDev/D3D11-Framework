#include "GraphicLoader.h"
#include <string_view>
#include <string>
#include <sstream>
#include <d3dcompiler.h>
#include <wrl.h>
#include <iostream>
#include <fstream>
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
}