#pragma once
#include <d3d11.h>

class Graphics3D
{

private:
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	IDXGISwapChain* swapChain;
	ID3D11Device *device;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11DeviceContext *context;
	ID3D11RenderTargetView *renderTargetView;

public:
	Graphics3D();
	~Graphics3D();

	void Init3D(HWND hwnd);
	void Clean();
};

