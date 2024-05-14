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
/// graphics에서 그려낼 오브젝트
/// gameframework에서 meshrender componenet를 이용해 
/// 해당 오브젝트를 추가하는 방식으로 생각해보았다
/// 이유는 framework에서 어떤 방식으로 그릴지 설정을 해주면
/// Graphics 내부에서는 생성된 설정에 맞는 오브젝트가 있어야하기 때문
/// 
/// 밖에서 업데이트 된 값을 내부적으로 상수 버퍼 갱신 해야한다
/// 
/// 2024.04.11
/// 모델 로더를 통해 읽어온 데이터를 가공하려하니
/// 구조가 아예 안맞는다 어지럽다...
/// 
/// 2024.04.17
/// device가 바인드 resource manager가 생성 및 관리를 할 것이라 이제 내부에 있던 render는 필요가 없어졌다
/// 동시에 device를 멤버 변수로 갖고 있을 필요도 없다
/// update는 필요하겠지..? 이건 고민중
/// 
/// </summary>


//assimp로 읽어온 데이터 구조체
struct ModelData : public Resource
{
	ModelData() : m_name(L"need name"), m_RootNode()
	{
	}

	virtual void Release() override {}


	std::wstring m_name;

	Node* m_RootNode;	//node 전체를 담고 있는 컨테이너

	std::vector<Mesh*> m_Meshes; // VB + IB + BONE + PRIMITIVE
	std::vector<Material*> m_Materials; //SRV
	std::vector<Animation*> m_Animations; //애니메이션 전체를 담고있는 컨테이너
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


	//수정 필요 내가 편하게 쓰려고 만들긴 했는데 내부에서 쓸 object라면 가릴 필요가 없다 생각함
	//밖에서 이 object를 접근하나? 그건 아니란 말이지

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
	double playTime; //현재 애니메이션 플레이 시간

private:

	VertexShader* m_VS;
	PixelShader* m_PS;
	RenderState* m_renderState;

	ModelData* m_LoadData;	//Mesh + Material + Animation

	std::map<std::wstring, Buffer*> m_CBList; //상수버퍼를 담고있는 컨테이너
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