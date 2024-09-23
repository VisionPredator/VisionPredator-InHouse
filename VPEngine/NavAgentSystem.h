#pragma once
#include "System.h"
#include "NavMeshData.h"
class NavAgentSystem :public System,public IStartable,public IFixedUpdatable
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
    // IUpdatable��(��) ���� ��ӵ�
    void FixedUpdate(float deltaTime) override;
    void AddAgentToCrowd(NavAgentComponent* comp);


    // IStartable��(��) ���� ��ӵ�
    void Initialize() override;

    void Start(uint32_t gameObjectId) override;

    void Finish(uint32_t gameObjectId) override;

    void Finalize() override;


private:

    friend class SceneManager;






};
