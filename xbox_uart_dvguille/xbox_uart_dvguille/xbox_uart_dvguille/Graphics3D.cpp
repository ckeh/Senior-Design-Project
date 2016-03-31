#include "Graphics3D.h"



Graphics3D::Graphics3D() {
}


Graphics3D::~Graphics3D(){
}

void Graphics3D::Init3D(HWND hwnd) {

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	swapChainDesc.BufferDesc.Width						= 0;								// Match the size of the window.
	swapChainDesc.BufferDesc.Height						= 0;
	swapChainDesc.BufferDesc.Format						= DXGI_FORMAT_B8G8R8A8_UNORM;		// This is the most common swap chain format.
	swapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= 1;
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= 60;
	swapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count						= 1;									// Don't use multi-sampling.
	swapChainDesc.SampleDesc.Quality					= 0;
	swapChainDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount							= 2;									// Use double-buffering to minimize latency.
	swapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;	// All Windows Store apps must use this SwapEffect.
	swapChainDesc.OutputWindow							= hwnd;
	swapChainDesc.Windowed								= true;
	swapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags									= 0;

	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		featureLevels,
		4,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain,
		&device,
		&featureLevel,
		&context
		);

}

void Graphics3D::Clean() {
	if (swapChain) swapChain->Release();
	if (device) device->Release();
	if (context) context->Release();
}