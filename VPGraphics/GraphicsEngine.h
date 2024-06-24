#pragma once
#include "IGraphics.h"
#include "MeshFilter.h"

#pragma region DX
class ViewPort;
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
class ForwardPipeline;
#pragma endregion

class LightManager;

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

	virtual void AddLight(std::wstring name, Kind_of_Light kind, LightData data) override;
	virtual void EraseLight(std::wstring name, Kind_of_Light kind) override;

	virtual void UpdateLightData(std::wstring name, Kind_of_Light kind, LightData data) override;

	/// Debug Draw
	void DrawSphere(const debug::SphereInfo& info) override;
	void DrawBox(const debug::AABBInfo& info) override;
	void DrawOBB(const debug::OBBInfo& info) override;
	void DrawFrustum(const debug::FrustumInfo& info) override;
	void DrawGrid(const debug::GridInfo& info) override;
	void DrawRing(const debug::RingInfo& info) override;
	void DrawTriangle(const debug::TriangleInfo& info) override;
	void DrawQuad(const debug::QuadInfo& info) override;
	void DrawRay(const debug::RayInfo& info) override;

	///Editor
	virtual ID3D11ShaderResourceView* GetSRV(std::wstring name) override;

protected:
	std::vector<std::weak_ptr<RenderTargetView>> m_RTVs;
	std::vector<std::weak_ptr<DepthStencilView>> m_DSVs;

	std::map<std::wstring, std::pair<PassState,std::shared_ptr<ModelData>>> m_RenderList;
	std::array<std::unordered_map<std::wstring, LightData>, static_cast<int>(Kind_of_Light::End)> m_LightList;

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ViewPort> m_CurViewPort;

private:
	std::shared_ptr<ResourceManager> m_ResourceManager;
	std::shared_ptr<ModelLoader> m_Loader;
	std::shared_ptr<Animator> m_Animator;
	std::shared_ptr<LightManager> m_LightManager;
	std::shared_ptr<class DebugDrawManager> m_DebugDrawManager;

private:
	HWND m_hWnd;
	RECT m_wndSize;
	
	//camera
	DirectX::SimpleMath::Matrix m_View;
	DirectX::SimpleMath::Matrix m_Proj;
	DirectX::SimpleMath::Matrix m_ViewProj;

	// Pipeline
	std::shared_ptr<ForwardPipeline> m_ForwardPipeline;
	std::shared_ptr<DeferredShadingPipeline> m_DeferredShadingPipeline;




	//test
	LightData Dir;
	LightData Spot;
	LightData Point;
	

///editor
private:
	void InitializeImGui();
	void BeginImGui();
	void EndImGui();
	void DestroyImGui();


};