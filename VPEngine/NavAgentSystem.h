#pragma once
#include "System.h"
#include "NavMeshData.h"
class NavAgentSystem :public System
{
public:
    NavAgentSystem(std::shared_ptr<SceneManager> sceneManager);
    virtual ~NavAgentSystem()=default;
    virtual void Update();
    void AssignToNavigationField(NavMeshData* navField);
    void SetSpeed(float speed);
    void SetAcceleration(float accel);
    void SetRadius(float radius);
    const VPMath::Vector3& GetTargetPosition();
    float GetSpeed();
    float GetAcceleration();
    float GetRadius();
    void MoveTo(VPMath::Vector3 destination);
private:
    NavAgentData* m_Agentdate;
    bool m_bIsStop;

    friend class SceneManager;
};

