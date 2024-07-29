#pragma once
class ConvexMesh
{
public:
    ConvexMesh();
    ~ConvexMesh();
    inline physx::PxConvexMesh* GetConvexMesh();

    physx::PxConvexMesh* mConvexMesh;

};



inline physx::PxConvexMesh* ConvexMesh::GetConvexMesh()
{
    
    return mConvexMesh;
}