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
    friend class NavMeshBakerSystem;
};
