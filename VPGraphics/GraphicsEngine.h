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
class DecalManager;

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
	~GraphicsEngine() = default;

	bool Initialize() override;
	void Update(double dt) override;
	void CulingUpdate() override;
	void AnimationUpdate(double dt) override;
	void EndUpdate(double dt) override;
	bool Finalize() override;
	void BeginRender() override;
	void Render() override;
	void ImguiBeginRender() override;
	void ImguiEndRender() override;
	void EndRender() override;

	void OnResize(HWND hwnd, bool isFullScreen) override;

	virtual void DebugRenderONOFF(bool isRender) override;


	void SetCamera(VPMath::Matrix view, VPMath::Matrix proj, const VPMath::Matrix& orthoProj) override;
	void testCulling(VPMath::Matrix view, VPMath::Matrix proj) override;
	/// Model
	bool AddRenderModel(std::shared_ptr<RenderData> data)override;
	void EraseObject(uint32_t EntityID) override;
	void UpdateModel(uint32_t EntityID)override;
	const double GetDuration(std::wstring name, int index) override;
	virtual const VPMath::Matrix Attachment(const uint32_t entityID, const std::wstring socketName) override;
	virtual void SetVP(bool isVP) override;


	/// Particle
	void CreateParticleObject(uint32_t entityID, const effect::ParticleInfo& info) override;
	void UpdateParticleObject(uint32_t entityID, const effect::ParticleInfo& info) override;
	void DeleteParticleObjectByID(uint32_t id) override;

	/// UI
	void CreateImageObject(uint32_t id, const ui::ImageInfo& info) override;
	void UpdateImageObject(uint32_t id, const ui::ImageInfo& info) override;
	void DeleteImageObject(uint32_t id) override;
	void CreateTextObject(uint32_t entityID, const ui::TextInfo& info) override;
	void UpdateTextObject(uint32_t entityID, const ui::TextInfo& info) override;
	void DeleteTextObject(uint32_t entityId) override;
	RECT GetImageRect(uint32_t entityID) const override;

	/// Light
	void AddLight(uint32_t EntityID, LightType kind, LightData data) override;
	void EraseLight(uint32_t EntityID, LightType kind) override;
	void UpdateLightData(uint32_t EntityID, LightType kind, LightData data) override;

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

	///Decal
	virtual void DrawDecal(decal::Info info) override;


	///Editor
	void* GetSRV(std::wstring name) override;

	///물리
	virtual std::vector<VPMath::Vector3> GetVertices(std::string fbx) override;


protected:
	std::vector<std::weak_ptr<RenderTargetView>> m_RTVs;
	std::vector<std::weak_ptr<DepthStencilView>> m_DSVs;

	std::vector<std::shared_ptr<RenderData>> m_RenderVector;	//프레임워크쪽에서 준 데이터들
	std::vector<std::shared_ptr<RenderData>> m_AfterCulling; //컬링해서 그려낼 최종 친구들

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
	std::shared_ptr<class UIManager> m_UIManager;
	std::shared_ptr <DecalManager> m_DecalManager;

private:
	HWND m_hWnd;
	RECT m_wndSize;
	
	//camera
	VPMath::Matrix m_View;
	VPMath::Matrix m_Proj;
	VPMath::Matrix m_ViewProj;
	DirectX::BoundingFrustum m_Frustum;

	// Pipeline
	std::shared_ptr<PassManager> m_PassManager;

	bool isDebug = false;
	
private:
	void Culling();
	std::vector<std::shared_ptr<RenderData>>::iterator FindEntity(uint32_t id);

///editor
private:
	void InitializeImGui();
	void BeginImGui();
	void EndImGui();
	void DestroyImGui();
};
