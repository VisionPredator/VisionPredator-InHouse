#pragma once
#include <queue>
#include <vector>

#include "MeshFilter.h"
class Device;

class ModelData;
class Mesh;

class VertexShader;
class PixelShader;
class RenderState;

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
///  
/// </summary>


struct D3D11_VIEWPORT;


class ResourceManager;
class RenderTargetView;
class DepthStencilView;

class RenderPass
{
public:
	RenderPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manger);
	~RenderPass();

	virtual void Render() abstract;
	virtual void StaticRender() abstract;
	virtual void SkinnedRender() abstract;
	void AddModelData(std::shared_ptr<ModelData> model);
	void AddModelData(std::map<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>& model_list);

protected:
	std::queue<std::weak_ptr<ModelData>> m_RenderModelQueue;

	std::weak_ptr<Device> m_Device;

	std::weak_ptr<VertexShader> m_VS;
	std::weak_ptr<PixelShader> m_PS;
	std::weak_ptr<RenderState> m_RS;


	std::weak_ptr<ResourceManager> m_ResourceManager;


	PassState m_state = PassState::None;
};

class ForwardPass : public RenderPass
{
public:
	ForwardPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager, D3D11_VIEWPORT* vp);
	~ForwardPass();

	virtual void Render() override;
	virtual void StaticRender() override;
	virtual void SkinnedRender() override;

private:
	std::weak_ptr<RenderTargetView> m_RTV;
	std::weak_ptr<DepthStencilView> m_DSV;
	D3D11_VIEWPORT* m_VP;
};



class SkinnigPass : public RenderPass
{
public:
	SkinnigPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager, D3D11_VIEWPORT* vp);
	~SkinnigPass();

	virtual void Render() override;
	virtual void StaticRender() override;
	virtual void SkinnedRender() override;

private:
	std::weak_ptr<RenderTargetView> m_RTV;
	std::weak_ptr<DepthStencilView> m_DSV;
	D3D11_VIEWPORT* m_VP;
};



class TexturePass : public RenderPass
{
public:
	TexturePass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager, D3D11_VIEWPORT* vp);
	~TexturePass();

	virtual void Render() override;
	virtual void StaticRender() override;
	virtual void SkinnedRender() override;

private:
	std::weak_ptr<RenderTargetView> m_RTV;
	std::weak_ptr<DepthStencilView> m_DSV;
	D3D11_VIEWPORT* m_VP;
};