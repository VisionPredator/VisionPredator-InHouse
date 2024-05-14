#pragma once
#include "Shader.h"

#include <string>

#include "Device.h"


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
	VertexShader(Device* device, std::wstring filename = L"need name");
	VertexShader(Device* device, VERTEXFILTER kind_of_vertex = VERTEXFILTER::BASE, std::wstring filename = L"need name");
	~VertexShader();



	ID3D11PixelShader* GetPS() = delete;
	virtual void Release() override;

	static Resource* Create(Device* device, std::wstring name); //지울 예정

private:
	VERTEXFILTER m_Kind_of_Vertex;
};