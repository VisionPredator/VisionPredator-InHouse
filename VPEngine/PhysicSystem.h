#pragma once
#include "System.h"
#include "EventSubscriber.h"
namespace Physic
{
    class IPhysx;
}

class PhysicSystem :
    public System, public IUpdatable, public IFixedUpdatable,public IStartable, public EventSubscriber
{
public:
    PhysicSystem(SceneManager* sceneManager);
    ~PhysicSystem() = default;

    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;
    // IFixedUpdatable��(��) ���� ��ӵ�
    void FixedUpdate(float deltaTime) override;
    // IStartable��(��) ���� ��ӵ�

    void Initialize() override;
    void Start(uint32_t gameObjectId) override;
    void Finish(uint32_t gameObjectId) override;
    void Finalize() override;
	void SetPhysics(Physic::IPhysx* interfaces) { m_PhysxEngine = interfaces; }
private: 
    Physic::IPhysx* m_PhysxEngine;
};

