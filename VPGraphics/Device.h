#pragma once

#include <d3d11.h>
#include <vector>


class Object;
class RenderTargetView;
class DepthStencilView;
class ShaderResourceView;

class Device
{
public:
	Device(HWND hWnd);
	~Device();

	void Initialize();
	void OnResize();

	ID3D11DeviceContext* Context() const;
	ID3D11Device* Get() const;
	IDXGISwapChain* SwapChain() const;
	RECT GetWndSize() const;

	void BeginRender(ID3D11RenderTargetView* RTV, ID3D11DepthStencilView* DSV, const float* color);
	void EndRender();
	void ForwardRender(Object* object);




	void BeginDeferredRender(std::vector<RenderTargetView*>& RTVs, ID3D11DepthStencilView* DSVs);
	void DeferredRender(Object* object, std::vector<RenderTargetView*>& RTVs, std::vector < DepthStencilView*>& DSVs);

	void DrawQuard(ShaderResourceView* srv);


	bool ableMSAA;
	UINT MSAAQuality;

private:
	bool CreateSwapChain();

	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_Context;

	D3D_FEATURE_LEVEL m_FeatureLevel;

	IDXGISwapChain* m_SwapChain;
	HWND m_hWnd;
	RECT m_wndSize;


	ID3D11RenderTargetView* MRT[5];

};

