#pragma once
#include "Shader.h"
#include <string>


class Device;

enum class VERTEXFILTER
{
	BASE = 0,
	TEXTURE,
	SKINNING,
	QUAD,

	END

};

class VertexShader : public Shader
{
public:
	VertexShader(std::wstring filename = L"need name");
	VertexShader(std::shared_ptr<Device>device, std::wstring filename = L"need name");
	VertexShader(std::shared_ptr<Device>device, VERTEXFILTER kind_of_vertex = VERTEXFILTER::BASE, std::wstring filename = L"need name");
	~VertexShader();



	ID3D11PixelShader* GetPS() = delete;
	virtual void Release() override;

private:
	VERTEXFILTER m_Kind_of_Vertex;
};