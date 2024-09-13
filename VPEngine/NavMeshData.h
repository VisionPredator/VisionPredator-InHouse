#pragma once
class NavMeshData
{
public:
    NavMeshData()
    {
        navQuery = dtAllocNavMeshQuery();
        crowd = dtAllocCrowd();
        context = std::make_unique<rcContext>(rcContext());
    }
    ~NavMeshData()
    {
        // Free poly meshes
        if (polyMesh)
        {
            rcFreePolyMesh(polyMesh);
            polyMesh = nullptr;
        }

        if (polyMeshDetail)
        {
            rcFreePolyMeshDetail(polyMeshDetail);
            polyMeshDetail = nullptr;
        }

        // Free crowd
        if (crowd)
        {
            dtFreeCrowd(crowd);
            crowd = nullptr;
        }

        // Free nav mesh query
        if (navQuery)
        {
            dtFreeNavMeshQuery(navQuery);
            navQuery = nullptr;
        }

        // Free nav mesh
        if (navMesh)
        {
            dtFreeNavMesh(navMesh);
            navMesh = nullptr;
        }

        // Clear the vector of world vertices
        m_worldVertices.clear();
    }

private:
    std::unique_ptr<rcContext> context{};
    rcPolyMesh* polyMesh{};
    rcConfig config{};
    rcPolyMeshDetail* polyMeshDetail{};
    class dtNavMesh* navMesh{};
    class dtNavMeshQuery* navQuery{};
    class dtCrowd* crowd{};
    std::vector<VPMath::Vector3> m_worldVertices{};
    friend class NavMeshSystem;
    friend class NavAgentSystem;
};

class NavAgentData
{
public:
    NavAgentData() {}
    virtual ~NavAgentData()
    {
        //if (crowd != nullptr && EntityID != 0)
        //    crowd->removeAgent(EntityID);
    }
public:
    int AgentID=-1;
    dtPolyRef targetRef{};
    float targetPos[3] = {};
    
    dtCrowdAgentParams agentParams
    {
        .radius = 1.f,
        .height = 0.3f,
        .maxAcceleration = FLT_MAX,
        .maxSpeed = 5.f,
        .collisionQueryRange = 10.f,
        .pathOptimizationRange = 30.f,
        .separationWeight = 2.f,
        .updateFlags = DT_CROWD_ANTICIPATE_TURNS |  DT_CROWD_SEPARATION| DT_CROWD_OPTIMIZE_TOPO|
        DT_CROWD_OPTIMIZE_VIS |
        DT_CROWD_OBSTACLE_AVOIDANCE,
        .obstacleAvoidanceType = (unsigned char)3,
    };
    bool IsStop{};
};