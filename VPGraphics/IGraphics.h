#pragma once
#include <windows.h>

#pragma region STL
#include <string>
#include <memory> //smart pointer
#include <vector>
#include <map>
#include <queue>
#pragma endregion STL


#include "MeshFilter.h"
#include "CBuffer.h"
#include "vpGraphics.h"


///editor 전용으로 쓸때만 쓸거니까 나중에 어케 처리해주자
#include <d3d11.h>

/// <summary>
/// 게임 엔진에서 사용할 인터페이스 클래스
/// </summary>
namespace Graphics
{
	class Interface
	{
	public:
		Interface() {};
		virtual ~Interface() {};

		Interface(Interface& other) = delete;
		Interface& operator=(Interface& other) = delete;
		Interface(Interface&& other) = delete;
		Interface&& operator=(Interface&& other) = delete;


		virtual bool Initialize() abstract;
		virtual void CulingUpdate() abstract;
		virtual void AnimationUpdate(double dt) abstract;
		virtual void Update(double dt) abstract;
		virtual void EndUpdate(double dt) abstract;
		virtual bool Finalize() abstract;
		virtual void BeginRender() abstract;
		virtual void Render() abstract;
		virtual void ImguiBeginRender() abstract;
		virtual void ImguiEndRender() abstract;
		virtual void EndRender() abstract;
		virtual void OnResize(HWND hwnd) abstract;


		//엔티티가 사라지면 그래픽스 안에 있는 해당 오브젝트도 지워주는 함수
		virtual void EraseObject(uint32_t EntityID) abstract;

		///추가해야할거
		//엔티티의 데이터가 업데이트 되면 그값을 renderlist의 값에 갱신 시켜줘야하는 함수 근데 이게 맞아? 매번 엔티티마다 이함수를 부를거야? 그건 좀..
		virtual void UpdateModel(uint32_t EntityID)abstract;
		
		//카메라의 상태 업데이트
		virtual void SetCamera(VPMath::Matrix view, VPMath::Matrix proj, const VPMath::Matrix& orthoProj) abstract;
		//컬링용 카메라 가시성 보려고 만듦
		virtual void testCulling(VPMath::Matrix view, VPMath::Matrix proj) abstract;

		//렌더링
		virtual bool AddRenderModel(std::shared_ptr<RenderData> data) abstract;

		//애니메이션의 재생시간
		virtual const double GetDuration(std::wstring name, int index) abstract;

		//소켓 - 본의 위치를 받는다
		//virtual const VPMath::Matrix Attachment(const uint32_t entityID) abstract;
		virtual const VPMath::Matrix Attachment(const uint32_t entityID, const std::wstring socketName) abstract;

		//vp 스킬 쓰면 렌더 바꾸기
		virtual void SetVP(bool isVP) abstract;

		/// Effect
		virtual void CreateParticleObject(uint32_t entityID, const effect::ParticleInfo& info) abstract;
		virtual void UpdateParticleObject(uint32_t entityID, const effect::ParticleInfo& info) abstract;
		virtual void DeleteParticleObjectByID(uint32_t id) abstract;

		/// Light
		virtual void AddLight(uint32_t EntityID,LightType kind, LightData data) abstract;
		virtual void EraseLight(uint32_t EntityID, LightType kind) abstract;
		virtual void UpdateLightData(uint32_t EntityID, LightType kind, LightData data) abstract;

		/// Debug Draw
		virtual void DrawSphere(const debug::SphereInfo& info) abstract;
		virtual void DrawBox(const debug::AABBInfo& info) abstract;
		virtual void DrawOBB(const debug::OBBInfo& info) abstract;
		virtual void DrawFrustum(const debug::FrustumInfo& info) abstract;
		virtual void DrawGrid(const debug::GridInfo& info) abstract;
		virtual void DrawRing(const debug::RingInfo& info) abstract;
		virtual void DrawTriangle(const debug::TriangleInfo& info) abstract;
		virtual void DrawQuad(const debug::QuadInfo& info) abstract;
		virtual void DrawRay(const debug::RayInfo& info) abstract;

		/// UI
		virtual void CreateImageObject(uint32_t id, const ui::ImageInfo& info) abstract;
		virtual void UpdateImageObject(uint32_t id, const ui::ImageInfo& info) abstract;
		virtual void DeleteImageObject(uint32_t id) abstract;

		///Editor 전용
		virtual ID3D11ShaderResourceView* GetSRV(std::wstring name) abstract;

		///물리 전용
		virtual std::vector<VPMath::Vector3> GetVertices(std::string fbx) abstract;
	};
}
