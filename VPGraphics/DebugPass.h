#pragma once
#include "RenderPass.h"

class DebugPass : public RenderPass
{
public:
	DebugPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager, std::shared_ptr<DebugDrawManager> debug);
	~DebugPass();

	virtual void Render() override;
private:
	std::weak_ptr<DebugDrawManager> m_DebugDrawManager;
	DirectX::SimpleMath::Matrix m_View;
	DirectX::SimpleMath::Matrix m_Proj;
};
