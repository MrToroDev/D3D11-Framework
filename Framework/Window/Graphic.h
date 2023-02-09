#pragma once
#include <d3d11_2.h>
#include <dxgidebug.h>
#include <wrl.h>
#include "Window.h"

namespace DX 
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	class Graphic
	{
	private:
		Window* winRef = nullptr;
		ComPtr<IDXGISwapChain1> swapchain = nullptr;             // the pointer to the swap chain interface
		ComPtr<ID3D11Device> dev = nullptr;                     // the pointer to our Direct3D device interface
		ComPtr<ID3D11DeviceContext> devcon = nullptr;           // the pointer to our Direct3D device context
		ComPtr<IDXGIFactory2> _dxgifactory = nullptr;
		D3D11_TEXTURE2D_DESC backBufferDesc;
		ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
		ComPtr<ID3D11DepthStencilState> depthStencilState = nullptr;
		ComPtr<ID3D11Texture2D> depthStencilBuffer = nullptr;
		ComPtr<ID3D11RasterizerState> rasterizerState = nullptr;
		ComPtr<ID3D11Query> startQuery = nullptr;
		ComPtr<ID3D11Query> endQuery = nullptr;
		ComPtr<ID3D11Query> disjoinQuery = nullptr;
		ComPtr<ID3D11RenderTargetView> backbuffer = nullptr;    // the pointer to our back buffer
#if defined(_DEBUG)
		ComPtr<ID3D11Debug> debuglayer = nullptr;
		ComPtr<ID3D11InfoQueue> debugInfo = nullptr;
		ComPtr<IDXGIInfoQueue> dxgiDebugInfo = nullptr;
#endif

		void CreateDepthStencilBuffer(int width, int height);
		void CreateSwapChain();

		void HandleDebugLayerMessages();

	public:
		Graphic(Window* win, bool fullscreen);
		~Graphic();

		/// <summary>
		/// ATTENTION! call this function ONLY after Present.
		/// </summary>
		/// <returns>UINT64</returns>
		float GetGPUTime();

		void UpdateSwapChain();
		void Clear(float color[4]);
		void SetRenderTarget();
		void UnBoundRenderTarget();
		void Present(int vsync);
		void SetViewport(int width, int height, int x, int y);
		void ClearStateFlush();

		bool isFullscreen() const;
		void SetFullScreen(bool fullscreen);
		ComPtr<ID3D11Device> getDevice();
		ComPtr<ID3D11DeviceContext> getDeviceContext();
		ComPtr<IDXGIFactory2> getFactory();
		ComPtr<ID3D11RenderTargetView> getRenderTarget();
	};
}