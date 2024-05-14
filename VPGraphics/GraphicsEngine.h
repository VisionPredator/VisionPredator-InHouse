#pragma once
#include "IGraphics.h"

#pragma region STL
#include <memory> //smart pointer

#pragma endregion STL

#include <vector>
#include <map>
#include "MeshFilter.h"

/// <summary>
/// 2024.03.14
/// 그림을 그려줄 클래스
/// 작성자 : 유승운
/// </summary>

struct D3D11_VIEWPORT;

class Device;
class ResourceManager;
class ModelLoader;
class Animator;
class RenderTargetView;
class DepthStencilView;
class ShaderResourceView;

class Object;

class GraphicsEngine : public Graphics::Interface
{
public:
	GraphicsEngine(HWND hWnd);
	~GraphicsEngine();

	virtual bool Initialize() override;
	virtual void Update(double dt) abstract;
	virtual bool Finalize() override;
	virtual void Render() override;
	virtual void OnResize() override;



	void DeferredRender();
	void Update(double dt, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj);
	virtual bool LoadResource(MeshFilter mesh, std::wstring name, std::wstring fbx, std::wstring VSname, std::wstring PSname) override;
	virtual void UpdateCB(std::wstring name, std::wstring cbname, WorldTransformCB constantstruct) override;
	virtual void UpdateCB(std::wstring name, std::wstring cbname, DirectionLightCB constantstruct)override;

protected:
	//objects들을 cb만 업데이트하고 리소스매니저에서 모든 리소스 쭉 업데이트? -> 지금 공용 상수버퍼로 쓰는 부분이 다 분리 되어야할거같은데?
	// MatrixPallete부터 등등 생각할 수록 어지럽네
	//인스턴싱까지 고려하면?
	//정렬 후 상수버퍼만 다르게 그리기

	//forward
	//RTV + DSV + objects

	//deferred
	//RTVs + DSVs + objects

	std::vector<RenderTargetView*> m_RTVs;
	std::vector<DepthStencilView*> m_DSVs;

	std::map<std::wstring, Object*> m_DeferredRenderObjects;
	std::map<std::wstring, Object*> m_FowardRenderObjects;


	void DrawQuad(ShaderResourceView* srv);


private:
	Device* m_device;
	D3D11_VIEWPORT* m_VP;

private:
	ResourceManager* m_ResourceManager;
	ModelLoader* m_Loader;
	Animator* m_Animator;

private:
	HWND m_hWnd;
	RECT m_wndSize;
};