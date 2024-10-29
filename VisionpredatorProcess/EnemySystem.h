#pragma once
#include "../VPEngine/System.h"
#include "VisPredComponents.h"
#include "EventSubscriber.h"
class EnemySystem :
	public System
	, public IFixedUpdatable
	, public IUpdatable
	, public IPhysicable
	, public IRenderable
	, public IStartable
	,public EventSubscriber
{
public:
	EnemySystem(const std::shared_ptr<SceneManager>& sceneManager) : System(sceneManager) {}
	
public:
	// IStartable
	void Initialize();
	void Start(uint32_t gameObjectId);
	void Finish(uint32_t gameObjectId) {}
	void Finalize() {}

	// IRenderable
	// TODO: �׷��Ƚ� �̿��� �޼ҵ� ȣ���Ϸ��� �ϴ� ����� ���µ�. �����ؾ���.
	void BeginRenderUpdate(float deltaTime) override {}
	void RenderUpdate(float deltaTime) override {}
	void LateRenderUpdate(float deltaTime) override {}
	void EditorRenderUpdate(float deltaTime) override {}

	// IUpdatable��(��) ���� ��ӵ�
	void Update(float deltaTime) override {}
    // IFixedUpdatable��(��) ���� ��ӵ�
	void FixedUpdate(float deltaTime) override;

	// IPhysicable��(��) ���� ��ӵ�
	void PhysicsUpdate(float deltaTime) override {}
	void PhysicsLateUpdate(float deltaTime) override {}
	void OnDamaged(std::any entityid_Damage);
	PlayerComponent* m_playercomponent =nullptr;

	
};

