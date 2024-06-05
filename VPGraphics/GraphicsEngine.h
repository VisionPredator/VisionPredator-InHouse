#pragma once
#include "IGraphics.h"

#pragma region DX
struct D3D11_VIEWPORT;

class Device;
class ResourceManager;
class ModelLoader;
class Animator;
class RenderTargetView;
class DepthStencilView;
class ShaderResourceView;
#pragma endregion DX

#pragma region Pipeline
class DeferredShadingPipeline;
// class ForwardShadingPipeline;
#pragma endregion

#pragma region Pass
class StaticPass;
class SkinningPass;
class DebugPass;

class PassManager;

#pragma endregion Pass

#include "MeshFilter.h"

class Camera;
class ModelData;



/// <summary>
/// 2024.03.14
/// 그림을 그려줄 클래스
/// 작성자 : 유승운
/// </summary>

class GraphicsEngine : public Graphics::Interface
{
public:
	GraphicsEngine(HWND hWnd);
	~GraphicsEngine();

	virtual bool Initialize() override;
	virtual void Update(double dt) override;
	virtual bool Finalize() override;
	virtual void BeginRender() override;
	virtual void Render() override;
	virtual void EndRender() override;

	virtual void OnResize() override;

	virtual bool AddRenderModel(MeshFilter mesh, std::wstring name, std::wstring fbx = L"") override;
	virtual void EraseObject(std::wstring name) override;

	virtual void SetCamera(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	virtual void UpdateModelTransform(std::wstring name, DirectX::SimpleMath::Matrix world) override;

	virtual void AddLight(Kind_of_Light kind, LightData data) override;


protected:
	std::vector<std::weak_ptr<RenderTargetView>> m_RTVs;
	std::vector<std::weak_ptr<DepthStencilView>> m_DSVs;

	//pass manager 안에 들어 있어야할까?
	std::map<std::wstring, std::pair<PassState,std::shared_ptr<ModelData>>> m_RenderList;

	void DrawQuad(ShaderResourceView* srv);

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<D3D11_VIEWPORT> m_VP;

private:
	std::shared_ptr<ResourceManager> m_ResourceManager;
	std::shared_ptr<ModelLoader> m_Loader;
	std::shared_ptr<Animator> m_Animator;
	std::shared_ptr<PassManager> m_PassManager;

private:
	HWND m_hWnd;
	RECT m_wndSize;
	
	//camera
	DirectX::SimpleMath::Matrix m_View;
	DirectX::SimpleMath::Matrix m_Proj;
	DirectX::SimpleMath::Matrix m_ViewProj;

	// Pipeline
	std::shared_ptr<DeferredShadingPipeline> m_DeferredShadingPipeline;

	StaticPass* m_BasePass;
	SkinningPass* m_SkinningPass;
	DebugPass* m_DebugPass;


	//test
	Camera* m_Camera;

	LightData Dir;
	LightData Spot;
	LightData Point;
	


};