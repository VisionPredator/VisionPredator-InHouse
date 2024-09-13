#pragma once
#include "Shader.h"
#include <string>


class Device;

enum class VERTEXFILTER	// TODO: 昏力
{
	STATIC = 0,
	SKINNING,
	QUAD,

	END
};

class VertexShader : public Shader
{
public:
	//VertexShader(std::wstring filename = L"need name");
	//VertexShader(std::shared_ptr<Device>device, std::wstring filename = L"need name");
	//VertexShader(std::shared_ptr<Device>device, VERTEXFILTER kind_of_vertex = VERTEXFILTER::STATIC, std::wstring filename = L"need name");

	// 窜老 积己磊
	VertexShader(const std::shared_ptr<Device>& device,
		const std::wstring& filename,
		const std::string& entryPoint,
		const D3D_SHADER_MACRO* macro = nullptr);
		
	~VertexShader() override = default;

	ID3D11PixelShader* GetPS() = delete;
	ID3D11VertexShader* GetShader() { return m_VS.Get(); }
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetComPtr() { return m_VS; }

	void Release() override;

private:
	VERTEXFILTER m_Kind_of_Vertex = {};
};