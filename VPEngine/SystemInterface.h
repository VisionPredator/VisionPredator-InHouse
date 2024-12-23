#pragma once
#include "../PhysxEngine/IPhysx.h"
#include "../SoundEngine/ISound.h"
class Entity;
namespace Graphics
{
	class Interface;
}

namespace Physic
{
	class IPhysx;
}
struct Component;
namespace Sound
{
	class ISound;
}
class IPhysicable
{
public:
	virtual void SetPhysicEngine(Physic::IPhysx* PhysicEngine) { m_PhysicsEngine = PhysicEngine; }
	virtual void PhysicsUpdate(float deltaTime) = 0;
	virtual void PhysicsLateUpdate(float deltaTime) = 0;

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
	virtual void BeginRenderUpdate(float deltaTime) = 0;
	virtual void RenderUpdate(float deltaTime) = 0;
	virtual void LateRenderUpdate(float deltaTime) = 0;
	virtual void EditorRenderUpdate(float deltaTime) = 0;
	virtual void SetGraphics(Graphics::Interface* Graphics) { m_Graphics= Graphics; }
	Graphics::Interface* m_Graphics = nullptr;
};
class ICompAddable
{
public:
	virtual void ComponentAdded(Component* comp) = 0;
	virtual void ComponentReleased(Component* comp) = 0;
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
class ITriggerable
{
public:
	virtual void EnterTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second) = 0;
	virtual void ExitTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second) = 0;
};
class ISoundable
{
public:
	virtual void SetSoundEngine(Sound::ISound* soundEngine) { m_SoundEngine = soundEngine; }

	Sound::ISound* m_SoundEngine = nullptr;

	virtual void SoundUpdate(float deltaTime) = 0;
};