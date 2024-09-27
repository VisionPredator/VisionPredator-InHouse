#pragma once
#include "System.h"
class SocketSystem :
    public System,public IUpdatable,public IRenderable, public IPhysicable
{
public:
    SocketSystem(std::shared_ptr<SceneManager> sceneManager);
    ~SocketSystem() = default;
    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;
    // IRenderable��(��) ���� ��ӵ�
    void BeginRenderUpdate(float deltaTime) override;
    // IRenderable��(��) ���� ��ӵ�
    void RenderUpdate(float deltaTime) override;

    void LateRenderUpdate(float deltaTime) override;
    void UpdateSocketRenderData(TransformComponent& entity);
    // IRenderable��(��) ���� ��ӵ�
    void EditorRenderUpdate(float deltaTime) override;
	void TargetConnectedID(SocketComponent& socketcomp);


    // IPhysicable��(��) ���� ��ӵ�
    void PhysicsUpdate(float deltaTime) override;

};

