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
    virtual ~NavMeshData()
    {
        rcFreePolyMesh(polyMesh);
        rcFreePolyMeshDetail(polyMeshDetail);
        dtFreeCrowd(crowd);
        dtFreeNavMeshQuery(navQuery);
        dtFreeNavMesh(navMesh);
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
    friend class NavMeshBakerSystem;
};
