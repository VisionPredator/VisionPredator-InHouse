#pragma once
#include "System.h"
#include "NavMeshData.h"
#include "EventSubscriber.h"
class NavAgentSystem :public System,public IStartable,public IPhysicable,public EventSubscriber
{
public:
    NavAgentSystem(std::shared_ptr<SceneManager> sceneManager);
    virtual ~NavAgentSystem()=default;
    void SetSpeed(uint32_t entityid, float speed);
    void SetSpeed(NavAgentComponent* comp, float speed);
    void SetAcceleration(uint32_t entityid, float accel);
    void SetAcceleration(NavAgentComponent* comp, float accel);
    void SetRadius(uint32_t entityid, float radius);
    void SetRadius(NavAgentComponent* comp, float radius);
    void SetHegiht(NavAgentComponent* comp, float Height);
    void SetHegiht(uint32_t entityid, float Height);
    //const VPMath::Vector3& GetTargetPosition();
    //float GetSpeed();
    //float GetAcceleration();
    //float GetRadius();
    void MoveTo(NavAgentComponent* comp,VPMath::Vector3 destination);
    void Stop(NavAgentComponent* comp);
    // IPhysicable을(를) 통해 상속됨
    void PhysicsUpdate(float deltaTime) override;

    // IPhysicable을(를) 통해 상속됨
    void PhysicsLateUpdate(float deltaTime) override;
    void AddAgentToCrowd(NavAgentComponent* comp);


    // IStartable을(를) 통해 상속됨
    void Initialize() override;

    void Start(uint32_t gameObjectId) override;

    void Finish(uint32_t gameObjectId) override;

    void Finalize() override;

    void OnRemoveNavAgent(std::any entityid);
private:

    friend class SceneManager;











};

