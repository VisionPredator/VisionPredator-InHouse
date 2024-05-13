#pragma once
#include "pch.h"

#include "Device.h"


enum class ResourceType
{
	None = 0,
	Shader,
	Buffer,
	RTV,
	DSV,
	SRV,
	RS,
	Object,
	Mesh,
	Material,
	ModelData,
	PixelShader,
	VertexShader,
	Texture2D,

	End
};

class Resource
{
public:
	Resource();
	Resource(std::shared_ptr<Device> device);
	virtual ~Resource();

	virtual void Release() abstract;


	template<typename T>
	static ResourceType GetResourceType();

protected:
	std::weak_ptr<Device> m_Device;
};


template<typename T>
ResourceType Resource::GetResourceType()
{
	/*if (std::is_base_of_v<Buffer, T>)
		return ResourceType::Buffer;
	if (std::is_base_of_v<RenderTargetView, T>)
		return ResourceType::RTV;
	if (std::is_base_of_v<DepthStencilView, T>)
		return ResourceType::DSV;
	if (std::is_base_of_v<ShaderResourceView, T>)
		return ResourceType::SRV;
	if (std::is_base_of_v<RenderState, T>)
		return ResourceType::RS;
	if (std::is_base_of_v<Object, T>)
		return ResourceType::Object;
	if (std::is_base_of_v<ModelData, T>)
		return ResourceType::ModelData;
	if (std::is_base_of_v<PixelShader, T>)
		return ResourceType::PixelShader;
	if (std::is_base_of_v<VertexShader, T>)
		return ResourceType::VertexShader;
	if (std::is_base_of_v<Texture2D, T>)
		return ResourceType::Texture2D;*/

	return ResourceType::None;
}
