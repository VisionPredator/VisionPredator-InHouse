#pragma once
#include <queue>

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
	RenderPass(Device* device, ResourceManager* manger);
	~RenderPass();

	virtual void Render() abstract;
	virtual void StaticRender() abstract;
	virtual void SkinnedRender() abstract;
	void AddModelData(ModelData* model);

protected:
	std::queue<ModelData*> m_RenderModelQueue;

	Device* m_Device;

	VertexShader* m_VS;
	PixelShader* m_PS;
	RenderState* m_RS;


	ResourceManager* m_ResourceManager;

};

class ForwardPass : public RenderPass
{
public:
	ForwardPass(Device* device, ResourceManager* manager, D3D11_VIEWPORT* vp);
	~ForwardPass();

	virtual void Render() override;
	virtual void StaticRender() override;
	virtual void SkinnedRender() override;

private:
	RenderTargetView* m_RTV;
	DepthStencilView* m_DSV;
	D3D11_VIEWPORT* m_VP;
};