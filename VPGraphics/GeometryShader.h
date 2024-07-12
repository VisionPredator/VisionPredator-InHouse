#pragma once
#include "Shader.h"
#include <string>
#include <wrl/client.h>

class GeometryShader : public Shader
{
public:
	GeometryShader(const std::shared_ptr<Device>& device,
		const std::wstring& filename,
		const std::string& entryPoint,
		const std::string& shaderModel,
		const D3D_SHADER_MACRO* macro = nullptr);

	ID3D11GeometryShader* GetShader() { return m_Shader.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_Shader;
};

