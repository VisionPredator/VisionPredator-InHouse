#pragma once
#include "Shader.h"
class ComputeShader :
    public Shader
{
public:
	ComputeShader(std::shared_ptr<Device> device, std::wstring filename = L"need name");
	~ComputeShader();
	virtual void Release() override;

private:

};

