#include "pch.h"
#include "PostProcessManager.h"
#include "Device.h"

void PostProcessManager::Initialize(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager)
{
    m_Device = device;
    m_ResourceManager = resourceManager;


}

void PostProcessManager::Render()
{
}

void PostProcessManager::OnResize()
{
}

std::shared_ptr<ShaderResourceView> PostProcessManager::Blur(const std::shared_ptr<ShaderResourceView>& inputSRV, const int& blurCount)
{
    ID3D11ShaderResourceView* nullSRV[1] = { 0 };
    ID3D11UnorderedAccessView* nullUAV[1] = { 0 };

    for (size_t i = 0; i < blurCount; ++i)
    {
	    // Horizontal blur pass
        //m_Device->Context()->CSSetShader()
    }

    return nullptr;
}
