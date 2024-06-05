#include "pch.h"
#include "Sampler.h"

Sampler::Sampler(std::shared_ptr<Device>device, D3D11_SAMPLER_DESC desc) : Resource(device)
{
	m_Device.lock()->Get()->CreateSamplerState(&desc, &m_samplerState);
}

Sampler::~Sampler()
{

}

ID3D11SamplerState* Sampler::Get() const
{
	return m_samplerState;
}

ID3D11SamplerState** Sampler::GetAddress()
{
	return &m_samplerState;
}

void Sampler::Release()
{
	m_samplerState->Release();
}
