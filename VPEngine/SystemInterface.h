#pragma once
#include "../PhysxEngine/IPhysx.h"

namespace Graphics
{
	class Interface;
}

namespace Physic
{
	class IPhysx;
}
class IUpdatable
{
public:
	virtual void Update(float deltaTime) = 0;
};

class IFixedUpdatable
{
public:
	virtual void FixedUpdate(float deltaTime) = 0;
};

class IRenderable
{
public:
	virtual void RenderUpdate(float deltaTime) = 0;
	virtual void SetGraphics(Graphics::Interface* Graphics) { m_Graphics= Graphics; }
	Graphics::Interface* m_Graphics = nullptr;
};
class IPhysicable
{
public:
	virtual void SetPhysicEngine(Physic::IPhysx* PhysicEngine) { m_PhysicsEngine = PhysicEngine; }
	Physic::IPhysx* m_PhysicsEngine=nullptr;
};

class IStartable
{
public:
	virtual void Initialize() = 0;
	virtual void Start(uint32_t gameObjectId) = 0;
	virtual void Finish(uint32_t gameObjectId) = 0;
	virtual void Finalize() = 0;
};

