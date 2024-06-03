#pragma once
#include "Shader.h"

#include <string>
#include "Device.h"

class PixelShader : public Shader
{
public:
	PixelShader(std::wstring filename = L"need name");
	PixelShader(std::shared_ptr<Device> device, std::wstring filename = L"need name");
	~PixelShader();

	ID3D11PixelShader* GetVS() const = delete;
	ID3D11InputLayout* InputLayout() const = delete;
	virtual void Release() override;


private:
};

