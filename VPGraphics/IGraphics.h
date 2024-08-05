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


///editor �������� ������ ���Ŵϱ� ���߿� ���� ó��������
#include <d3d11.h>

/// <summary>
/// ���� �������� ����� �������̽� Ŭ����
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
		virtual void Update(double dt) abstract;
		virtual bool Finalize() abstract;
		virtual void BeginRender() abstract;
		virtual void Render() abstract;
		virtual void EndRender() abstract;
		virtual void OnResize(HWND hwnd) abstract;


		//��ƼƼ�� ������� �׷��Ƚ� �ȿ� �ִ� �ش� ������Ʈ�� �����ִ� �Լ�
		virtual void EraseObject(uint32_t EntityID) abstract;

		///�߰��ؾ��Ұ�
		//��ƼƼ�� �����Ͱ� ������Ʈ �Ǹ� �װ��� renderlist�� ���� ���� ��������ϴ� �Լ� �ٵ� �̰� �¾�? �Ź� ��ƼƼ���� ���Լ��� �θ��ž�? �װ� ��..
		virtual void UpdateModel(uint32_t EntityID, RenderData& data)abstract;
		
		//ī�޶��� ���� ������Ʈ
		virtual void SetCamera(VPMath::Matrix view, VPMath::Matrix proj, const VPMath::Matrix& orthoProj) abstract;

		//������
		virtual bool AddRenderModel(MeshFilter mesh, uint32_t EntityID, std::wstring fbx = L"") abstract;

		virtual const double GetDuration(std::wstring name) abstract;

		/// Effect
		virtual void CreateParticleObject(const uint32_t& entityID, const effect::ParticleInfo& info) abstract;
		virtual void UpdateParticleObject(const uint32_t& entityID, const effect::ParticleInfo& info) abstract;
		virtual void DeleteParticleObjectByID(const uint32_t& id) abstract;

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
		
		///Editor ����
		virtual ID3D11ShaderResourceView* GetSRV(std::wstring name) abstract;


		///���� ����
		virtual std::vector<VPMath::Vector3> GetVertices(std::string fbx) abstract;
	};

}
