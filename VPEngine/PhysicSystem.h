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
	PhysicSystem(std::shared_ptr<SceneManager> sceneManager);
	~PhysicSystem() = default;


	void Initialize() override;
	void Start(uint32_t EntityID) override;
	void Finish(uint32_t EntityID) override;
	void Finalize() override;
	void CreateRigidBody(uint32_t EntityID);
	void CreateStatic(RigidBodyComponent* staticbody);
	void CreateDynamic(RigidBodyComponent* dynamicbody);
	void CreateCapsuleController(uint32_t EntityID);
	void ReleaseRigidBody(uint32_t EntityID);
	void ReleaseCapsuleController(uint32_t EntityID);
private:
	const float m_location_threshold = 0.000001f;
	const float m_rotation_threshold_degrees = 0.000001f;
	// IRenderable��(��) ���� ��ӵ�
    void RenderUpdate(float deltaTime) override;

	// IRenderable��(��) ���� ��ӵ�
	void EditorRenderUpdate(float deltaTime) override;

    // IPhysicable��(��) ���� ��ӵ�
    void PhysicsUpdate(float deltaTime) override;

	VPMath::Vector3 ApplyPivotAndOffset(const ControllerComponent& controllerComponent, VPMath::Vector3 baseLocation);
	VPMath::Vector3 DisApplyPivotAndOffset(const ControllerComponent& controllerComponent, VPMath::Vector3 baseLocation);



};

