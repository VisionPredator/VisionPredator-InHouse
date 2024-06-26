#pragma once
#include <queue>
#include <vector>

#include "MeshFilter.h"
class Device;

class ModelData;
class RenderData;
class Mesh;

class VertexShader;
class PixelShader;
class RenderState;

class LightManager;
class ResourceManager;
class RenderTargetView;
class DepthStencilView;
class RenderTargetView;
class ShaderResourceView;
class IndexBuffer;
class VertexBuffer;

struct D3D11_VIEWPORT;

/// <summary>
/// 2024.05.16
/// ������ ���������� ����
///Draw Call�ϴ� �ϳ��� ���� ����
///���� �׸������� ���ε��ϰ� DrawPrimitiveCall�ϴ� �Ϸ��� ����
///inputlay + setVS + setPS ���� + drawindexed������ ����
/// 
/// �⺻���� �н� ���� forward, deferred
/// vs ps������ �׷����°� �ٸ��� �ʿ��� cb�� �޶���
/// �޽��� ���� texture�� �ٸ��� �������� texture�� �ʿ����
/// ���� object�� �̰� �� ������ �ִ� ������? �ٵ� ���� ������Ʈ�� ���� mesh���� ǥ���� �ٸ��Ŷ� ���̾�? �׷��� �޽������� �н��� Ÿ���ϴµ�
/// ���� �ٸ� �н��� �˸°� ������ cb texture�� ���? ���� �־��ٱ� �޽��� �־��ً� �ѽ��� �� cb texture�� ���� ���������
/// �ٵ� �̰� ���� object�� ������ �ִ� �������ϴ°Ŷ� ���� �ٸ���? ���� ���� �𵨿� texture�� �ٲ㼭 �Ѵٰ� �Ҷ��� ���� �����ϴ°� �´µ�
/// ���پ˰� �������ֳİ� -> �� �н��� ������ �ڿ����� ��� �Ұų� ->�ϴ� �� �н����� �����? �н��� �þ ���� Ŭ������ ��� �������? �н��� �׷��� ���� �þ�� �ұ�?
/// ��� �׸��� ���� � �н��� �־�������� �ʿ�
/// �޽� ������ �׸��� �Ǹ� �ִϸ��̼��� ��� �ؾ��ұ�?
/// 
/// 2024.05.17
/// ������� �����غ��� static, skinned meshó���� ���� �ٸ��� render ������ �޽��� ���� �ٲ�
/// �Լ� �ΰ��� �����? �ƴϸ� Ŭ������ �ΰ� �����? �� �н����� ó�� ����� �ٸ�����?
/// 
/// �Ѹ���� �н��� �����
/// �־��� ���ҽ��� ���� �پ��� �н� �����
/// 
/// 
/// ������ ���ĸ� ModelData�� fbx���� �о�� �����͵��ε� ���⿡ Mesh�� ������ ����־�
/// �ٵ� pass������ �ϳ��� mesh  �� �޽��� ����� texture + cb �����Ͱ� �ʿ�
/// Modeldata ������ ������ mesh ������ �׷����� ���� ����
/// 
/// �н����� �� ��ƴ� �� render�� �Ѵٰ� ġ��?
/// �޽����� �� �н��� �ν��Ͻ��� ����µ�
/// �װ� �ƴ϶� �н��� ������ �޽����� ���Ÿ�� �ִٰ� �׸��ǵ�
/// ������Ʈ�� ���� cb�� �ƴ� �͵��� �ܺο��� �־�����Ѵ�
/// 2024.06.04
/// �н����� skinning static�� ���� �ʿ䰡 �ֳ�? �н����ٰ� �ƴ϶� ���� �н� ����ü��
/// </summary>




class RenderPass
{
public:
	RenderPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manger);
	~RenderPass();

	virtual void Render();
	void AddModelData(std::shared_ptr<RenderData> model);

protected:
	void BindStatic(std::shared_ptr<RenderData> curModel);
	void BindSkeletal(std::shared_ptr<RenderData> curModel, std::shared_ptr<Mesh> mesh);


	std::queue<std::weak_ptr<RenderData>> m_RenderDataQueue;

	std::weak_ptr<Device> m_Device;

	std::weak_ptr<PixelShader> m_DebugPS;

	std::weak_ptr<VertexShader> m_SkeletalMeshVS;
	std::weak_ptr<VertexShader> m_StaticMeshVS;
	std::weak_ptr<PixelShader> m_MeshPS;


	std::weak_ptr<ResourceManager> m_ResourceManager;

	std::weak_ptr<RenderTargetView> m_RTV;
	std::weak_ptr<DepthStencilView> m_DSV;


	PassState m_state = PassState::None;
};

class DebugPass : public RenderPass
{
public:
	DebugPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager);
	~DebugPass();

	virtual void Render() override;

};


//forward
class FowardPass : public RenderPass
{
public:
	FowardPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager);
	~FowardPass();

private:
};

class DeferredPass : public RenderPass
{
public:
	DeferredPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager);
	~DeferredPass();
	virtual void Render() override;

private:

	void Geometry();
	void Light();

private:
	std::shared_ptr<DepthStencilView> m_DepthStencilView;

	//Geometry
	std::shared_ptr<PixelShader> m_GeometryPS;

	// Multi Render Target
	std::weak_ptr<RenderTargetView> m_AlbedoRTV;
	std::weak_ptr<RenderTargetView> m_NormalRTV;
	std::weak_ptr<RenderTargetView> m_PositionRTV;
	std::weak_ptr<RenderTargetView> m_DepthRTV;
	std::weak_ptr<RenderTargetView> m_MetalicRTV;
	std::weak_ptr<RenderTargetView> m_RoughnessRTV;
	std::weak_ptr<RenderTargetView> m_AORTV;
	std::weak_ptr<RenderTargetView> m_EmissiveRTV;

	//GBuffer Texture
	std::weak_ptr<ShaderResourceView> m_Albedo;
	std::weak_ptr<ShaderResourceView> m_Normal;
	std::weak_ptr<ShaderResourceView> m_Position;
	std::weak_ptr<ShaderResourceView> m_Depth;
	std::weak_ptr<ShaderResourceView> m_Metalic;
	std::weak_ptr<ShaderResourceView> m_Roughness;
	std::weak_ptr<ShaderResourceView> m_AO;
	std::weak_ptr<ShaderResourceView> m_Emissive;	

	//Light - Quad
	std::weak_ptr<VertexBuffer> m_QuadVB;
	std::weak_ptr<IndexBuffer> m_QuadIB;
	std::weak_ptr<VertexShader> m_QuadVS;
	std::weak_ptr<PixelShader> m_QuadPS;
	std::weak_ptr<PixelShader> m_Deferred;

	//GBuffer Result
	std::weak_ptr<ShaderResourceView> m_GBuffer;
};