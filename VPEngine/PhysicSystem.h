#pragma once
#include "System.h"
#include "EventSubscriber.h"
namespace Physic
{
    class IPhysx;
}

class PhysicSystem :
    public System, 
    public IStartable,
    public IPhysicable, 
    public IRenderable,
    public EventSubscriber
{
public:
    PhysicSystem(SceneManager* sceneManager);
    ~PhysicSystem() = default;


    void Initialize() override;
    void Start(uint32_t EntityID) override;
    void Finish(uint32_t EntityID) override;
    void Finalize() override;
    void CreateRigidBody(uint32_t EntityID);
    void CreateCapsuleController(uint32_t EntityID);
    void ReleaseRigidBody(uint32_t EntityID);
    void ReleaseCapsuleController(uint32_t EntityID);
private: 
    const float m_location_threshold = 0.000001f;
    const float m_rotation_threshold_degrees= 0.000001f;
    // IRenderable을(를) 통해 상속됨
    void RenderUpdate(float deltaTime) override;

    // IPhysicable을(를) 통해 상속됨
    void PhysicsUpdate(float deltaTime) override;
};

