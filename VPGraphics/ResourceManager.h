#pragma once
#include <string>
#include <map>
#include <array>
#include <unordered_map>
#include <vector>

#include "SimpleMath.h"

#include "MeshFilter.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment (lib, "D3DCompiler.lib")

#include "Device.h"

#include "RenderState.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ShaderResourceView.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"

#include "Object.h"
#include "VertexData.h"


enum class SamplerState
{
	LINEAR = 0,

	END
};


/// <summary>
/// 2024.04.17
/// Device를 이용해 리소스를 생성, 관리 할 클래스
/// 쉽게 말해서 얘는 Create랑 Get만 있어도 됨
/// Add까지
/// </summary>

class ResourceManager
{
public:
	ResourceManager(Device* device);
	~ResourceManager();

	template<typename T, typename... Types>
	T* Create(std::wstring path, Types...args);

	template<typename T>
	T* Get(std::wstring path);

	template<typename T>
	void Add(std::wstring path, Resource* resource);


	template<typename T>
	void Erase(std::wstring path);


#pragma region delete funtion


	void Update(double dt, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj);


	void Initialize();

#pragma endregion delete funtion

	void OnResize();

private:
	Device* m_device;



	ConstantBuffer<CameraCB>* m_Camera;
	ConstantBuffer<DirectionLightCB>* m_DirectionalLight;

	std::array<std::unordered_map<std::wstring, Resource*>, static_cast<int>(ResourceType::End)> m_ResourceArray;
};


template<typename T, typename...Types>
T* ResourceManager::Create(std::wstring path, Types...args)
{
	int index = static_cast<int>(Resource::GetResourceType<T>());
	std::unordered_map<std::wstring, Resource*> curMap = m_ResourceArray[index];

	if (curMap.find(path) != curMap.end())
	{
		return dynamic_cast<T*>(m_ResourceArray[index][path]);
	}

	m_ResourceArray[index].insert(std::pair<std::wstring, Resource*>(path, new T(m_device, args...)));

	return dynamic_cast<T*>(m_ResourceArray[index][path]);
}

template<typename T>
T* ResourceManager::Get(std::wstring path)
{
	int index = static_cast<int>(Resource::GetResourceType<T>());
	std::unordered_map<std::wstring, Resource*>& curMap = m_ResourceArray[index];


	if (curMap.find(path) != curMap.end())
	{
		return dynamic_cast<T*>(curMap[path]);
	}

	return nullptr;
}

template<typename T>
void ResourceManager::Add(std::wstring path, Resource* resource)
{
	int index = static_cast<int>(Resource::GetResourceType<T>());

	std::unordered_map<std::wstring, Resource*>& curMap = m_ResourceArray[index];
	if (curMap.find(path) == curMap.end())
	{
		curMap.insert({ path,resource });
	}
}

template<typename T>
void ResourceManager::Erase(std::wstring path)
{
	int index = static_cast<int>(Resource::GetResourceType<T>());

	std::unordered_map<std::wstring, Resource*>& curMap = m_ResourceArray[index];
	if (curMap.find(path) != curMap.end())
	{
		curMap[path]->Release();
		delete curMap[path];
		curMap.erase(path);
	}
}

