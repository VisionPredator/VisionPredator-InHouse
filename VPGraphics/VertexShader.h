#pragma once
#include "Shader.h"
#include <string>


class Device;

enum class VERTEXFILTER	// TODO: 삭제
{
	STATIC = 0,
	SKINNING,
	QUAD,

	END
};

class VertexShader : public Shader
{
public:
	// 단일 생성자
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