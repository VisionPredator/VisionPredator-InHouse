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
class PassManager;
#pragma endregion

class LightManager;
class TimeManager;

class ModelData;
struct RenderData;


/// <summary>
/// 2024.03.14
/// 그림을 그려줄 클래스
/// 작성자 : 유승운
/// </summary>

class GraphicsEngine : public Graphics::Interface
{
public:
	GraphicsEngine(HWND hWnd, TimeManager* timeManager);
	~GraphicsEngine();

	virtual bool Initialize() override;
	virtual void Update(double dt) override;
	virtual bool Finalize() override;
	virtual void BeginRender() override;
	virtual void Render() override;
	virtual void EndRender() override;

	virtual void OnResize(HWND hwnd) override;

	virtual bool AddRenderModel(MeshFilter mesh, uint32_t EntityID, std::wstring fbx = L"") override;
	virtual void EraseObject(uint32_t EntityID) override;

	virtual void SetCamera(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	virtual void UpdateModel(uint32_t EntityID, RenderData& data)override;


	virtual void AddLight(uint32_t EntityID, LightType kind, LightData data) override;
	virtual void EraseLight(uint32_t EntityID, LightType kind) override;

	virtual void UpdateLightData(uint32_t EntityID, LightType kind, LightData data) override;

	virtual const double GetDuration(std::wstring name) override;

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

	std::map<uint32_t, std::shared_ptr<RenderData>> m_RenderList;
	std::unordered_map<uint32_t, LightData> m_Lights;

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ViewPort> m_CurViewPort;

private:
	std::shared_ptr<ResourceManager> m_ResourceManager;
	std::shared_ptr<ModelLoader> m_Loader;
	std::shared_ptr<Animator> m_Animator;
	std::shared_ptr<LightManager> m_LightManager;
	std::shared_ptr<class DebugDrawManager> m_DebugDrawManager;	
	std::shared_ptr<class ParticleManager> m_ParticleManager;
	TimeManager* m_TimeManager;

private:
	HWND m_hWnd;
	RECT m_wndSize;
	
	//camera
	DirectX::SimpleMath::Matrix m_View;
	DirectX::SimpleMath::Matrix m_Proj;
	DirectX::SimpleMath::Matrix m_ViewProj;

	// Pipeline
	std::shared_ptr<PassManager> m_PassManager;
	

///editor
private:
	void InitializeImGui();
	void BeginImGui();
	void EndImGui();
	void DestroyImGui();


};