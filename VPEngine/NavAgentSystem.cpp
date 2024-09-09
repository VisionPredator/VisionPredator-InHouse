#include "pch.h"
#include "NavAgentSystem.h"

NavAgentSystem::NavAgentSystem(std::shared_ptr<SceneManager> sceneManager):System{ sceneManager }
{
}

void NavAgentSystem::SetSpeed(float speed)
{
    m_Agentdate->agentParams.maxSpeed = speed;
    if (m_Agentdate->crowd != nullptr)
        m_Agentdate->crowd->updateAgentParameters(m_Agentdate->EntityID, &m_Agentdate->agentParams);
}
void NavAgentSystem::SetAcceleration(float accel)
{
    m_Agentdate->agentParams.maxAcceleration = accel;
    if (m_Agentdate->crowd != nullptr)
        m_Agentdate->crowd->updateAgentParameters(m_Agentdate->EntityID, &m_Agentdate->agentParams);
}
void NavAgentSystem::SetRadius(float radius)
{
    m_Agentdate->agentParams.radius = radius;
    if (m_Agentdate->crowd != nullptr)
        m_Agentdate->crowd->updateAgentParameters(m_Agentdate->EntityID, &m_Agentdate->agentParams);
}

void NavAgentSystem::MoveTo(VPMath::Vector3 destination)
{
    const dtQueryFilter* filter{ m_Agentdate->crowd->getFilter(0) };
    const dtCrowdAgent* agent = m_Agentdate->crowd->getAgent(m_Agentdate->EntityID);

    const float* halfExtents = m_Agentdate->crowd->getQueryExtents();
    GetSceneManager()->GetSceneNavMeshData()->navQuery->findNearestPoly(reinterpret_cast<float*>(&destination), halfExtents, filter, &m_Agentdate->targetRef, m_Agentdate->targetPos);
    m_Agentdate->crowd->requestMoveTarget(m_Agentdate->EntityID, m_Agentdate->targetRef, m_Agentdate->targetPos);

    m_bIsStop = false;
} 