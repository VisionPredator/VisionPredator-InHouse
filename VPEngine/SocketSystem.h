#pragma once
#include "System.h"
#include "EventSubscriber.h"
class SocketSystem :
    public System,public IUpdatable,public IRenderable,public IPhysicable,public EventSubscriber
{
public:
    SocketSystem(std::shared_ptr<SceneManager> sceneManager);
    ~SocketSystem() = default;
    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;
    // IRenderable을(를) 통해 상속됨
    void BeginRenderUpdate(float deltaTime) override;
    // IRenderable을(를) 통해 상속됨
    void RenderUpdate(float deltaTime) override;

    void LateRenderUpdate(float deltaTime) override;
    void UpdateSocketRenderData(TransformComponent& entity);
    // IRenderable을(를) 통해 상속됨
    void EditorRenderUpdate(float deltaTime) override;
	void TargetConnectedID(SocketComponent& socketcomp);
    void OnSocketUpdate(std::any);



	// IPhysicable을(를) 통해 상속됨
	void PhysicsUpdate(float deltaTime) override { };
	void PhysicsLateUpdate(float deltaTime)  override {};

};

