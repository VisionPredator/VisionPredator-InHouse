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
	// TODO: 그래픽스 이용한 메소드 호출하려고 일단 상속해 놨는데. 수정해야함.
	void BeginRenderUpdate(float deltaTime) override {}
	void RenderUpdate(float deltaTime) override {}
	void LateRenderUpdate(float deltaTime) override {}
	void EditorRenderUpdate(float deltaTime) override {}

	// IUpdatable을(를) 통해 상속됨
	void Update(float deltaTime) override {}
    // IFixedUpdatable을(를) 통해 상속됨
	void FixedUpdate(float deltaTime) override;

	// IPhysicable을(를) 통해 상속됨
	void PhysicsUpdate(float deltaTime) override {}
	void PhysicsLateUpdate(float deltaTime) override {}
	void OnDamaged(std::any entityid_Damage);
	PlayerComponent* m_playercomponent =nullptr;

	
};

