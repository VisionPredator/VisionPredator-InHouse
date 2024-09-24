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
class IPhysicable
{
public:
	virtual void SetPhysicEngine(Physic::IPhysx* PhysicEngine) { m_PhysicsEngine = PhysicEngine; }
	virtual void PhysicsUpdate(float deltaTime) = 0;

	Physic::IPhysx* m_PhysicsEngine = nullptr;
};
class IFixedUpdatable
{
public:
	virtual void FixedUpdate(float deltaTime) = 0;
};

class IUpdatable
{
public:
	virtual void Update(float deltaTime) = 0;
};

class ILateUpdatable
{
public:
	virtual void LateUpdate(float deltaTime) = 0;
};

class IRenderable
{
public:
	virtual void RenderUpdate(float deltaTime) = 0;
	virtual void EditorRenderUpdate(float deltaTime) = 0;


	virtual void SetGraphics(Graphics::Interface* Graphics) { m_Graphics= Graphics; }
	Graphics::Interface* m_Graphics = nullptr;
};


class IStartable
{
public:
	virtual void Initialize() = 0;
	virtual void Start(uint32_t gameObjectId) = 0;
	virtual void Finish(uint32_t gameObjectId) = 0;
	virtual void Finalize() = 0;
};


class IContactable
{
public:
	virtual void EnterCollision(std::pair<uint32_t,uint32_t> entitypair) = 0;
	virtual void ExitCollision(std::pair<uint32_t, uint32_t> entitypair) = 0;

};