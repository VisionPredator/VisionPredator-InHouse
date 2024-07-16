#pragma once
#include "System.h"
#include "EventSubscriber.h"
namespace Physic
{
    class IPhysx;
}

class PhysicSystem :
    public System, 
    public IUpdatable, 
    public IFixedUpdatable,
    public IStartable,
    public IPhysicable, 
    public IRenderable,
    public EventSubscriber
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
private: 
    const float m_location_threshold = 0.001f;
    const float m_rotation_threshold_degrees= 0.001f;
    // IRenderable��(��) ���� ��ӵ�
    void RenderUpdate(float deltaTime) override;
};

