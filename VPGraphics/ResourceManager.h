#pragma once
#include "pch.h"

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
#include "Sampler.h"
#include "Texture2D.h"

#include "VertexData.h"




/// <summary>
/// 2024.04.17
/// Device�� �̿��� ���ҽ��� ����, ���� �� Ŭ����
/// ���� ���ؼ� ��� Create�� Get�� �־ ��
/// Add����
/// </summary>

class ResourceManager
{
public:
	ResourceManager(std::weak_ptr<Device> device);
	~ResourceManager();

	template<typename T, typename... Types>
	std::weak_ptr <T> Create(const std::wstring path, Types...args);

	template<typename T> std::weak_ptr <T> Get(const std::wstring path);

	template<typename T> void Add(const std::wstring path, std::shared_ptr<Resource> resource);


	template<typename T>
	void Erase(const std::wstring path);


	void Initialize();
	void OnResize();

private:
	std::weak_ptr<Device> m_Device;
	std::weak_ptr<ConstantBuffer<CameraData>> m_Camera;
	std::weak_ptr<ConstantBuffer<LightData>> m_DirectionalLight;
	std::weak_ptr<ConstantBuffer<LightArray>> m_UsingLights;

	std::array<std::unordered_map<std::wstring, std::shared_ptr<Resource>>, static_cast<int>(ResourceType::End)> m_ResourceArray;
};


template<typename T, typename...Types>
std::weak_ptr <T> ResourceManager::Create(const std::wstring path, Types...args)
{
	int index = static_cast<int>(Resource::GetResourceType<T>());
	std::unordered_map<std::wstring, std::shared_ptr<Resource>> curMap = m_ResourceArray[index];

	if (curMap.find(path) != curMap.end())
	{
		return std::dynamic_pointer_cast<T>(m_ResourceArray[index][path]);
	}

	std::shared_ptr<T> newResource = std::make_shared<T>(m_Device.lock(), args...);

	m_ResourceArray[index].insert(std::pair<std::wstring, std::shared_ptr<Resource>>(path, newResource));

	return  std::dynamic_pointer_cast<T>(m_ResourceArray[index][path]);
}

template<typename T> std::weak_ptr <T> ResourceManager::Get(const std::wstring path)
{
	int index = static_cast<int>(Resource::GetResourceType<T>());
	std::unordered_map<std::wstring, std::shared_ptr<Resource>>& curMap = m_ResourceArray[index];


	if (curMap.find(path) != curMap.end())
	{
		return  std::dynamic_pointer_cast<T>(curMap[path]);
	}


	return {};
}

template<typename T> void ResourceManager::Add(const std::wstring path, std::shared_ptr<Resource> resource)
{
	int index = static_cast<int>(Resource::GetResourceType<T>());

	std::unordered_map<std::wstring, std::shared_ptr<Resource>>& curMap = m_ResourceArray[index];
	if (curMap.find(path) == curMap.end())
	{
		curMap.insert({ path,resource });
	}
}

template<typename T>
void ResourceManager::Erase(const std::wstring path)
{
	int index = static_cast<int>(Resource::GetResourceType<T>());

	std::unordered_map<std::wstring, std::shared_ptr<Resource>>& curMap = m_ResourceArray[index];
	if (curMap.find(path) != curMap.end())
	{
		curMap[path]->Release();
		//delete curMap[path];
		curMap.erase(path);
	}
}

