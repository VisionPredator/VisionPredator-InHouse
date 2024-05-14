#pragma once
//#include "Resource.h"
#include <vector>
#include <map>
#include <string>
#include <d3d11.h>

#include "Resource.h"

#include "Material.h"
#include "Mesh.h"
#include "Node.h"
#include "Animation.h"

#include "ConstantBuffer.h"

#include "Object.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "RenderState.h"

class VertexShader;
class PixelShader;
class RenderState;


/// <summary>
/// graphics���� �׷��� ������Ʈ
/// gameframework���� meshrender componenet�� �̿��� 
/// �ش� ������Ʈ�� �߰��ϴ� ������� �����غ��Ҵ�
/// ������ framework���� � ������� �׸��� ������ ���ָ�
/// Graphics ���ο����� ������ ������ �´� ������Ʈ�� �־���ϱ� ����
/// 
/// �ۿ��� ������Ʈ �� ���� ���������� ��� ���� ���� �ؾ��Ѵ�
/// 
/// 2024.04.11
/// �� �δ��� ���� �о�� �����͸� �����Ϸ��ϴ�
/// ������ �ƿ� �ȸ´´� ��������...
/// 
/// 2024.04.17
/// device�� ���ε� resource manager�� ���� �� ������ �� ���̶� ���� ���ο� �ִ� render�� �ʿ䰡 ��������
/// ���ÿ� device�� ��� ������ ���� ���� �ʿ䵵 ����
/// update�� �ʿ��ϰ���..? �̰� �����
/// 
/// </summary>


//assimp�� �о�� ������ ����ü
struct ModelData : public Resource
{
	ModelData() : m_name(L"need name"), m_RootNode()
	{
	}

	virtual void Release() override {}


	std::wstring m_name;

	Node* m_RootNode;	//node ��ü�� ��� �ִ� �����̳�

	std::vector<Mesh*> m_Meshes; // VB + IB + BONE + PRIMITIVE
	std::vector<Material*> m_Materials; //SRV
	std::vector<Animation*> m_Animations; //�ִϸ��̼� ��ü�� ����ִ� �����̳�
};

class Object : public Resource
{
public:
	Object();
	~Object();

	template<typename T>
	void Update();

	template<typename T>
	ConstantBuffer<T>* GetCB(std::wstring name);

	virtual void Release() override {}

	template<typename T>
	bool Set(T* data);

	template<typename T>
	bool Set(ConstantBuffer<T>* data, std::wstring key);


	//���� �ʿ� ���� ���ϰ� ������ ����� �ߴµ� ���ο��� �� object��� ���� �ʿ䰡 ���� ������
	//�ۿ��� �� object�� �����ϳ�? �װ� �ƴ϶� ������

	std::vector<Mesh*>& Meshes();
	std::vector<Animation*>& Animations();
	std::vector<Material*>& Materials();

	ID3D11RasterizerState* RS();
	ID3D11InputLayout* InputLayOut();
	ID3D11PixelShader* PS();
	ID3D11PixelShader** AddressPS();
	ID3D11VertexShader* VS();
	ID3D11VertexShader** AddressVS();

	ShaderResourceView* DiffuseSRV(int index);
	ShaderResourceView* NormalSRV(int index);
	ShaderResourceView* SpecularSRV(int index);

	Node* RootNode();


	Animation* CurAnimation(int index);

	std::wstring m_name;
	double playTime; //���� �ִϸ��̼� �÷��� �ð�

private:

	VertexShader* m_VS;
	PixelShader* m_PS;
	RenderState* m_renderState;

	ModelData* m_LoadData;	//Mesh + Material + Animation

	std::map<std::wstring, Buffer*> m_CBList; //������۸� ����ִ� �����̳�
};

template<typename T>
void Object::Update()
{
	for (auto& cb : m_CBList)
	{
		T* curCB;
		curCB = dynamic_cast<T*>(cb.second);
		if (curCB != nullptr)
		{
			cb.second->Update();
		}
	}
}

template<typename T>
ConstantBuffer<T>* Object::GetCB(std::wstring name)
{
	return dynamic_cast<ConstantBuffer<T>*>(m_CBList[name]);
}

template<typename T>
bool Object::Set(T* data)
{

	ResourceType index = Resource::GetResourceType<T>();


	switch (index)
	{
		case ResourceType::None:
			break;
		case ResourceType::Shader:
			break;
		case ResourceType::Buffer:
			break;
		case ResourceType::RTV:
			break;
		case ResourceType::DSV:
			break;
		case ResourceType::SRV:
			break;
		case ResourceType::RS:
			m_renderState = dynamic_cast<RenderState*>(data);
			return true;
			break;
		case ResourceType::Object:
			break;
		case ResourceType::Mesh:
			break;
		case ResourceType::Material:
			break;
		case ResourceType::ModelData:
			m_LoadData = dynamic_cast<ModelData*>(data);
			return true;
			break;
		case ResourceType::PixelShader:
			m_PS = dynamic_cast<PixelShader*>(data);
			return true;
			break;
		case ResourceType::VertexShader:
			m_VS = dynamic_cast<VertexShader*>(data);
			return true;
			break;
		case ResourceType::End:
			break;
		default:
			break;
	}

	return false;
}




template<typename T>
bool Object::Set(ConstantBuffer<T>* data, std::wstring key)
{
	if (std::is_base_of_v<ConstantBuffer<T>, ConstantBuffer<T>>)
	{
		m_CBList.insert(std::pair<std::wstring, ConstantBuffer<T>*>(key, data));
		return true;
	}
}