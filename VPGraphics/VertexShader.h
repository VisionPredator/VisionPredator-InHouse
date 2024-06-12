#pragma once
#include "Shader.h"
#include <string>


class Device;

enum class VERTEXFILTER
{
	STATIC = 0,
	SKINNING,
	QUAD,

	END

};

class VertexShader : public Shader
{
public:
	VertexShader(std::wstring filename = L"need name");
	VertexShader(std::shared_ptr<Device>device, std::wstring filename = L"need name");
	VertexShader(std::shared_ptr<Device>device, VERTEXFILTER kind_of_vertex = VERTEXFILTER::STATIC, std::wstring filename = L"need name");

	VertexShader(const std::shared_ptr<Device>& device, const std::wstring& filename, const D3D_SHADER_MACRO* macro = nullptr, const int& a = 1);
	~VertexShader();



	ID3D11PixelShader* GetPS() = delete;
	virtual void Release() override;

private:
	VERTEXFILTER m_Kind_of_Vertex;
};