#pragma once
#include "Resource.h"


class Sampler : public Resource
{
public:
	Sampler(std::shared_ptr<Device>device, D3D11_SAMPLER_DESC desc);
	~Sampler();


	ID3D11SamplerState* Get() const;
	ID3D11SamplerState** GetAddress();

	virtual void Release() override;

private:

	ID3D11SamplerState* m_samplerState;

};

