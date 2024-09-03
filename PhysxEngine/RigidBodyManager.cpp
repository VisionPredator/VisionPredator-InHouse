#include "pch.h"
#include "RigidBodyManager.h"
#include "StaticRigidBody.h"
#include "EventManager.h"
#include "DynamicRigidBody.h"
#include "ConvexMeshResource.h"
#include "PhysichResourceManager.h"
#include <cmath> // For std::abs

RigidBodyManager::RigidBodyManager()
{
    EventManager::GetInstance().Subscribe("OnAddBodyScene", CreateSubscriber(&RigidBodyManager::OnAddBodyScene));
    EventManager::GetInstance().Subscribe("OnReleaseBodyScene", CreateSubscriber(&RigidBodyManager::OnReleaseBodyScene));
}

RigidBodyManager::~RigidBodyManager()
{
    m_RigidBodyMap.clear(); // 확인해보기
}

bool RigidBodyManager::Initialize(physx::PxPhysics* physics, physx::PxScene* scene, std::shared_ptr<PhysichResourceManager> resourceManager)
{
    m_Physics = physics;
    m_Scene = scene;
    m_ResourceManager = resourceManager;
    return true;
}

void RigidBodyManager::Update()
{
}

void RigidBodyManager::CreateStaticBody(const BoxColliderInfo& boxinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
    // Create the material for the box
    physx::PxMaterial* pxMaterial = m_Physics->createMaterial(boxinfo.colliderInfo.StaticFriction, boxinfo.colliderInfo.DynamicFriction, boxinfo.colliderInfo.Restitution);

    // Create the shape for the box collider with absolute scale values
    physx::PxBoxGeometry mesh(
        std::abs(boxinfo.Extent.x * boxinfo.colliderInfo.WorldScale.x),
        std::abs(boxinfo.Extent.y * boxinfo.colliderInfo.WorldScale.y),
        std::abs(boxinfo.Extent.z * boxinfo.colliderInfo.WorldScale.z)
    );

    physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

    // Set up the static body
    std::shared_ptr<StaticRigidBody> rigidBody = SettingStaticBody(shape, boxinfo.colliderInfo, collidertype, engininfo);
   
    // Initialize the rigid body
    if (rigidBody->Initialize(boxinfo.colliderInfo, shape, m_Physics))
    {
        rigidBody->SetExtent(boxinfo.Extent);

        // Insert the rigid body into the map
        m_RigidBodyMap[boxinfo.colliderInfo.EntityID] = rigidBody;

        // Add the rigid body to the scene
        AddBodyScene(rigidBody);
        shape->release();
    }
    else
    {
        assert(false);
    }
}

void RigidBodyManager::CreateStaticBody(const SphereColliderInfo& sphereinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
    // Create the material for the Sphere
    physx::PxMaterial* pxMaterial = m_Physics->createMaterial(sphereinfo.colliderInfo.StaticFriction, sphereinfo.colliderInfo.DynamicFriction, sphereinfo.colliderInfo.Restitution);

    // Create the shape for the sphere collider with the maximum absolute scale
    float maxScale = std::abs(sphereinfo.colliderInfo.WorldScale.GetMaxComponent());
    float newRadius = sphereinfo.Radius * maxScale;
    physx::PxSphereGeometry mesh(newRadius);
    physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

    // Set up the static body
    std::shared_ptr<StaticRigidBody> rigidBody = SettingStaticBody(shape, sphereinfo.colliderInfo, collidertype, engininfo);

    // Initialize the rigid body
    if (rigidBody->Initialize(sphereinfo.colliderInfo, shape, m_Physics))
    {
        rigidBody->SetRadius(sphereinfo.Radius);

        // Insert the rigid body into the map
        m_RigidBodyMap[sphereinfo.colliderInfo.EntityID] = rigidBody;

        // Add the rigid body to the scene
        AddBodyScene(rigidBody);
        shape->release();
    }
    else
    {
        assert(false);
    }
}

void RigidBodyManager::CreateStaticBody(const CapsuleColliderInfo& capsuleinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
    // Create the material for the collider
    physx::PxMaterial* pxMaterial = m_Physics->createMaterial(capsuleinfo.colliderInfo.StaticFriction, capsuleinfo.colliderInfo.DynamicFriction, capsuleinfo.colliderInfo.Restitution);

    // Create the shape for the capsule collider with absolute scale values
    VPMath::Vector3 scale = capsuleinfo.colliderInfo.WorldScale;
    VPMath::Vector2 size;
    size.y = std::abs(capsuleinfo.HalfHeight * scale.y);
    scale.y = 0;
    float maxScale = std::abs(scale.GetMaxComponent());
    size.x = capsuleinfo.Radius * maxScale;
    physx::PxShape* shape = m_Physics->createShape(physx::PxCapsuleGeometry(size.x, size.y), *pxMaterial);

    // Set up the static body
    std::shared_ptr<StaticRigidBody> rigidBody = SettingStaticBody(shape, capsuleinfo.colliderInfo, collidertype, engininfo);

    // Initialize the rigid body
    if (rigidBody->Initialize(capsuleinfo.colliderInfo, shape, m_Physics))
    {
        // Insert the rigid body into the map
        m_RigidBodyMap[capsuleinfo.colliderInfo.EntityID] = rigidBody;

        // Add the rigid body to the scene
        AddBodyScene(rigidBody);
        shape->release();
    }
    else
    {
        assert(false);
    }
}

void RigidBodyManager::CreateStaticBody(const VPPhysics::ConvexColliderInfo& convexMeshinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
    // Create the material for the collider
    physx::PxMaterial* pxMaterial = m_Physics->createMaterial(convexMeshinfo.colliderInfo.StaticFriction, convexMeshinfo.colliderInfo.DynamicFriction, convexMeshinfo.colliderInfo.Restitution);

    // Create the shape for the Convex collider with absolute scale values
    std::weak_ptr<ConvexMeshResource> convexMesh = m_ResourceManager.lock()->GetConvexMeshResource(convexMeshinfo.FBXName);
    physx::PxConvexMesh* pxConvexMesh = convexMesh.lock()->GetConvexMesh();
    auto mesh = physx::PxConvexMeshGeometry(pxConvexMesh);
    mesh.scale.scale = {
        std::abs(convexMeshinfo.colliderInfo.WorldScale.x),
        std::abs(convexMeshinfo.colliderInfo.WorldScale.y),
        std::abs(convexMeshinfo.colliderInfo.WorldScale.z)
    };
    physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

    // Set up the static body
    std::shared_ptr<StaticRigidBody> rigidBody = SettingStaticBody(shape, convexMeshinfo.colliderInfo, collidertype, engininfo);

    // Initialize the rigid body
    if (rigidBody->Initialize(convexMeshinfo.colliderInfo, shape, m_Physics))
    {
        // Insert the rigid body into the map
        m_RigidBodyMap[convexMeshinfo.colliderInfo.EntityID] = rigidBody;

        // Add the rigid body to the scene
        AddBodyScene(rigidBody);
        shape->release();
    }
    else
    {
        assert(false);
    }
}

void RigidBodyManager::CreateDynamicBody(const VPPhysics::BoxColliderInfo& boxinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
    physx::PxMaterial* pxMaterial = m_Physics->createMaterial(boxinfo.colliderInfo.StaticFriction, boxinfo.colliderInfo.DynamicFriction, boxinfo.colliderInfo.Restitution);

    // Create the shape for the box collider with absolute scale values
    physx::PxBoxGeometry mesh(
        std::abs(boxinfo.Extent.x * boxinfo.colliderInfo.WorldScale.x),
        std::abs(boxinfo.Extent.y * boxinfo.colliderInfo.WorldScale.y),
        std::abs(boxinfo.Extent.z * boxinfo.colliderInfo.WorldScale.z)
    );

    physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

    // Set up the dynamic body
    std::shared_ptr<DynamicRigidBody> rigidBody = SettingDynamicBody(shape, boxinfo.colliderInfo, collidertype, engininfo);

    // Initialize the rigid body
    if (rigidBody->Initialize(boxinfo.colliderInfo, shape, m_Physics))
    {
        m_RigidBodyMap[boxinfo.colliderInfo.EntityID] = rigidBody;
        AddBodyScene(rigidBody);
        shape->release();
        rigidBody->SetExtent(boxinfo.Extent);
    }
    else
    {
        assert(false);
    }
}

void RigidBodyManager::CreateDynamicBody(const VPPhysics::SphereColliderInfo& sphereinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
    physx::PxMaterial* pxMaterial = m_Physics->createMaterial(sphereinfo.colliderInfo.StaticFriction, sphereinfo.colliderInfo.DynamicFriction, sphereinfo.colliderInfo.Restitution);

    // Create the shape for the sphere collider with the maximum absolute scale
    float maxScale = std::abs(sphereinfo.colliderInfo.WorldScale.GetMaxComponent());
    float newRadius = sphereinfo.Radius * maxScale;
    physx::PxSphereGeometry mesh(newRadius);
    physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

    // Set up the dynamic body
    std::shared_ptr<DynamicRigidBody>  rigidBody = SettingDynamicBody(shape, sphereinfo.colliderInfo, collidertype, engininfo);

    // Initialize the rigid body
    if (rigidBody->Initialize(sphereinfo.colliderInfo, shape, m_Physics))
    {
        m_RigidBodyMap[sphereinfo.colliderInfo.EntityID] = rigidBody;
        AddBodyScene(rigidBody);
        shape->release();
        rigidBody->SetRadius(sphereinfo.Radius);
    }
    else
    {
        assert(false);
    }
}

void RigidBodyManager::CreateDynamicBody(const VPPhysics::CapsuleColliderInfo& capsuleinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
    physx::PxMaterial* pxMaterial = m_Physics->createMaterial(capsuleinfo.colliderInfo.StaticFriction, capsuleinfo.colliderInfo.DynamicFriction, capsuleinfo.colliderInfo.Restitution);

    // Create the shape for the capsule collider with absolute scale values
    VPMath::Vector3 scale = capsuleinfo.colliderInfo.WorldScale;
    VPMath::Vector2 size;
    size.y = std::abs(capsuleinfo.HalfHeight * scale.y);
    scale.y = 0;
    float maxScale = std::abs(scale.GetMaxComponent());
    size.x = capsuleinfo.Radius * maxScale;
    physx::PxShape* shape = m_Physics->createShape(physx::PxCapsuleGeometry(size.x, size.y), *pxMaterial);

    // Set up the dynamic body
    std::shared_ptr<DynamicRigidBody> rigidBody = SettingDynamicBody(shape, capsuleinfo.colliderInfo, collidertype, engininfo);

    // Initialize the rigid body
    if (rigidBody->Initialize(capsuleinfo.colliderInfo, shape, m_Physics))
    {
        m_RigidBodyMap[capsuleinfo.colliderInfo.EntityID] = rigidBody;
        AddBodyScene(rigidBody);
        shape->release();
        rigidBody->SetHalfHeight(capsuleinfo.HalfHeight);
        rigidBody->SetRadius(capsuleinfo.Radius);
    }
    else
    {
        assert(false);
    }
}

void RigidBodyManager::CreateDynamicBody(const VPPhysics::ConvexColliderInfo& convexMeshinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
    physx::PxMaterial* pxMaterial = m_Physics->createMaterial(convexMeshinfo.colliderInfo.StaticFriction, convexMeshinfo.colliderInfo.DynamicFriction, convexMeshinfo.colliderInfo.Restitution);

    // Create the shape for the Convex collider with absolute scale values
    std::weak_ptr<ConvexMeshResource> convexMesh = m_ResourceManager.lock()->GetConvexMeshResource(convexMeshinfo.FBXName);
    physx::PxConvexMesh* pxConvexMesh = convexMesh.lock()->GetConvexMesh();
    auto mesh = physx::PxConvexMeshGeometry(pxConvexMesh);
    mesh.scale.scale = {
        std::abs(convexMeshinfo.colliderInfo.WorldScale.x),
        std::abs(convexMeshinfo.colliderInfo.WorldScale.y),
        std::abs(convexMeshinfo.colliderInfo.WorldScale.z)
    };
    physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

    // Set up the dynamic body
    std::shared_ptr<DynamicRigidBody> rigidBody = SettingDynamicBody(shape, convexMeshinfo.colliderInfo, collidertype, engininfo);

    // Initialize the rigid body
    if (rigidBody->Initialize(convexMeshinfo.colliderInfo, shape, m_Physics))
    {
        m_RigidBodyMap[convexMeshinfo.colliderInfo.EntityID] = rigidBody;
        AddBodyScene(rigidBody);
        shape->release();
    }
    else
    {
        assert(false);
    }
}

std::shared_ptr<StaticRigidBody> RigidBodyManager::SettingStaticBody(physx::PxShape* shape, const ColliderInfo& info, const EColliderType& colliderType, const VPPhysics::PhysicsInfo& engininfo)
{
    physx::PxFilterData filterdata;
    filterdata.word0 = static_cast<int>(info.PhysicsLayer);
    filterdata.word1 = engininfo.CollisionMatrix[static_cast<int>(info.PhysicsLayer)];
    shape->setSimulationFilterData(filterdata);

    auto staticBody = std::make_shared<StaticRigidBody>(colliderType, info.EntityID, info.PhysicsLayer);
    m_RigidBodyMap[staticBody->GetID()] = staticBody;
    return staticBody;
}

std::shared_ptr<DynamicRigidBody> RigidBodyManager::SettingDynamicBody(physx::PxShape* shape, const ColliderInfo& info, const EColliderType& colliderType, const VPPhysics::PhysicsInfo& engininfo)
{
    physx::PxFilterData filterdata;
    filterdata.word0 = static_cast<int>(info.PhysicsLayer);
    filterdata.word1 = engininfo.CollisionMatrix[static_cast<int>(info.PhysicsLayer)];
    shape->setSimulationFilterData(filterdata);

    auto dynamicBody = std::make_shared<DynamicRigidBody>(colliderType, info.EntityID, info.PhysicsLayer);
    m_RigidBodyMap[dynamicBody->GetID()] = dynamicBody;
    return dynamicBody;
}

void RigidBodyManager::OnAddBodyScene(std::any data)
{
    auto tempRigid = std::any_cast<std::shared_ptr<RigidBody>>(data);

    if (auto dynamicBody = std::dynamic_pointer_cast<DynamicRigidBody>(tempRigid))
    {
        m_Scene->addActor(*dynamicBody->GetPxDynamicRigid());
    }
    else if (auto staticBody = std::dynamic_pointer_cast<StaticRigidBody>(tempRigid))
    {
        m_Scene->addActor(*staticBody->GetPxStaticRigid());
    }
}
void RigidBodyManager::OnReleaseBodyScene(std::any data)
{
    auto [rigidBody, isDynamic] = std::any_cast<std::pair<std::shared_ptr<RigidBody>, bool>>(data);

    if (isDynamic)
    {
        if (auto dynamicBody = dynamic_cast<DynamicRigidBody*>(rigidBody.get()))
        {
            m_Scene->removeActor(*dynamicBody->GetPxDynamicRigid());
        }
    }
    else
    {
        if (auto staticBody = dynamic_cast<StaticRigidBody*>(rigidBody.get()))
        {
            m_Scene->removeActor(*staticBody->GetPxStaticRigid());
        }
    }
}

void RigidBodyManager::ReleaseBodyScene(uint32_t entityID)
{
    auto it = m_RigidBodyMap.find(entityID);
    if (it == m_RigidBodyMap.end())
        return;

    auto tempbody = it->second;
    bool isDynamic = false;

    if (auto dynamicBody = dynamic_cast<DynamicRigidBody*>(tempbody.get()))
    {
        if (dynamicBody->GetPxDynamicRigid()->getScene() == m_Scene)
        {
            m_RigidBodyMap.erase(it);
            isDynamic = true;
        }
    }
    else if (auto staticBody = dynamic_cast<StaticRigidBody*>(tempbody.get()))
    {
        if (staticBody->GetPxStaticRigid()->getScene() == m_Scene)
        {
            m_RigidBodyMap.erase(it);
        }
    }

    std::any data = std::make_pair(tempbody, isDynamic);
    EventManager::GetInstance().ScheduleEvent("OnReleaseBodyScene", data);
}

std::shared_ptr<RigidBody> RigidBodyManager::GetRigidBody(uint32_t entityID)
{
    auto it = m_RigidBodyMap.find(entityID);
    if (it == m_RigidBodyMap.end())
        return nullptr;
    return it->second;
}

bool RigidBodyManager::HasRigidBody(uint32_t entityID)
{
    return m_RigidBodyMap.count(entityID) > 0;
}

void RigidBodyManager::ExtractSceneVerticesAndFacesByLayer(PxScene* scene, EPhysicsLayer layer, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices)
{
    if (!scene)
        return;

    // Retrieve only static rigid actors from the scene
    PxU32 actorCount = scene->getNbActors(physx::PxActorTypeFlag::eRIGID_STATIC);
    std::vector<PxRigidStatic*> actors(actorCount);
    scene->getActors(physx::PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(actors.data()), actorCount);

    // Iterate over all static actors
    for (PxRigidStatic* actor : actors)
    {
        if (!actor)
            continue;

        // Retrieve shapes from the PxRigidStatic actor
        PxU32 shapeCount = actor->getNbShapes();
        std::vector<PxShape*> shapes(shapeCount);
        actor->getShapes(shapes.data(), shapeCount);

        for (PxShape* shape : shapes)
        {
            // Check if the shape is a simulation shape and not a trigger shape
            if (shape->getFlags().isSet(physx::PxShapeFlag::eSIMULATION_SHAPE))
            {
                // Check the shape's filter data for the layer
                PxFilterData filterData = shape->getSimulationFilterData();
                if (filterData.word0 == static_cast<PxU32>(layer))
                {
                    // Use the existing function to extract vertices and faces for this actor
                    RigidBodyManager::ExtractVerticesAndFaces(actor, outVertices, outIndices);
                    break; // No need to check other shapes if we've processed the actor
                }
            }
        }
    }
}

void RigidBodyManager::ExtractVerticesAndFaces(uint32_t entityID, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices)
{
    if (!HasRigidBody(entityID))
        return;

    auto rigidBody = GetRigidBody(entityID);
    auto staticBody = dynamic_cast<StaticRigidBody*>(rigidBody.get());
    if (!staticBody)
        return;

    physx::PxRigidStatic* pxRigidStatic = staticBody->GetPxStaticRigid();
    if (!pxRigidStatic)
        return;

    // Use the previously implemented function to extract vertices and faces
    ExtractVerticesAndFaces(pxRigidStatic, outVertices, outIndices);
}

void RigidBodyManager::ExtractVerticesAndFaces(PxRigidStatic* actor, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices)
{
    if (!actor)
        return;

    // Retrieve shapes from the PxRigidStatic actor
    PxU32 shapeCount = actor->getNbShapes();
    std::vector<PxShape*> shapes(shapeCount);
    actor->getShapes(shapes.data(), shapeCount);

    for (PxShape* shape : shapes)
    {
        // Only process shapes that are marked as SIMULATION_SHAPE (i.e., COLLISION)
        if (shape->getFlags().isSet(physx::PxShapeFlag::eSIMULATION_SHAPE))
        {
            // Get the geometry from the shape
            PxGeometryHolder geometryHolder = shape->getGeometry();

            if (geometryHolder.getType() == PxGeometryType::eCONVEXMESH)
            {
                // Access the convex mesh geometry
                PxConvexMeshGeometry convexGeometry = geometryHolder.convexMesh();
                PxConvexMesh* convexMesh = convexGeometry.convexMesh;

                // Extract vertices from the PxConvexMesh
                const PxVec3* meshVertices = convexMesh->getVertices();
                PxU32 vertexCount = convexMesh->getNbVertices();

                // Store the start index for this batch of vertices
                int baseIndex = static_cast<int>(outVertices.size());

                // Convert and append vertices to outVertices
                for (PxU32 i = 0; i < vertexCount; ++i)
                {
                    outVertices.emplace_back(meshVertices[i].x, meshVertices[i].y, meshVertices[i].z);
                }

                // Extract indices (the index buffer refers to the vertex buffer)
                const PxU8* indexBuffer = convexMesh->getIndexBuffer();
                PxU32 polygonCount = convexMesh->getNbPolygons();

                for (PxU32 i = 0; i < polygonCount; ++i)
                {
                    PxHullPolygon polygon;
                    convexMesh->getPolygonData(i, polygon);

                    // Extract the vertex indices for the polygon
                    for (PxU32 j = 0; j < polygon.mNbVerts; ++j)
                    {
                        PxU32 vertexIndex = indexBuffer[polygon.mIndexBase + j];
                        outIndices.push_back(baseIndex + static_cast<int>(vertexIndex));
                    }

                    // If the polygon has more than 3 vertices, triangulate it:
                    for (PxU32 j = 2; j < polygon.mNbVerts; ++j)
                    {
                        outIndices.push_back(baseIndex + static_cast<int>(indexBuffer[polygon.mIndexBase]));
                        outIndices.push_back(baseIndex + static_cast<int>(indexBuffer[polygon.mIndexBase + j - 1]));
                        outIndices.push_back(baseIndex + static_cast<int>(indexBuffer[polygon.mIndexBase + j]));
                    }
                }
            }
        }
    }
}
void RigidBodyManager::SetGobalPose(uint32_t entityID, const VPMath::Vector3& P, const VPMath::Quaternion& Q)
{
    auto temp = GetRigidBody(entityID);
    if (!temp) return;

    PxVec3 tempPos = { P.x, P.y, P.z };
    physx::PxQuat tempQuat = physx::PxQuat(Q.x, Q.y, Q.z, Q.w);
    tempQuat.normalize();

    if (Reflection::GetTypeID<DynamicRigidBody>() == temp->GetTypeID())
    {
        auto dynamicBody = static_cast<DynamicRigidBody*>(temp.get());
        dynamicBody->GetPxDynamicRigid()->setGlobalPose({ tempPos, tempQuat });
    }
    else if (Reflection::GetTypeID<StaticRigidBody>() == temp->GetTypeID())
    {
        auto staticBody = static_cast<StaticRigidBody*>(temp.get());
        staticBody->GetPxStaticRigid()->setGlobalPose({ tempPos, tempQuat });
    }
    else
    {
        assert(false);
    }
}

VPMath::Vector3 RigidBodyManager::GetVelocity(uint32_t entityID)
{
    auto temp = GetRigidBody(entityID);
    if (!temp)
    {
        assert(false);
    }

    if (Reflection::GetTypeID<DynamicRigidBody>() == temp->GetTypeID())
    {
        auto dynamicBody = static_cast<DynamicRigidBody*>(temp.get());
        PxVec3 velocity = dynamicBody->GetPxDynamicRigid()->getLinearVelocity();
        return { velocity.x, velocity.y, velocity.z };
    }
    else if (Reflection::GetTypeID<StaticRigidBody>() == temp->GetTypeID())
    {
        return { 0, 0, 0 };
    }
    else
    {
        assert(false);
    }
}

void RigidBodyManager::AddVelocity(uint32_t entityID, const VPMath::Vector3& dir, float V)
{
    auto temp = GetRigidBody(entityID);
    if (!temp)
    {
        assert(false);
        return;
    }

    if (Reflection::GetTypeID<DynamicRigidBody>() == temp->GetTypeID())
    {
        auto dynamicBody = static_cast<DynamicRigidBody*>(temp.get());
        VPMath::Vector3 Dir = dir;
        Dir.Normalize();
        PxVec3 force = { Dir.x, Dir.y, Dir.z };
        force *= V;
        dynamicBody->GetPxDynamicRigid()->addForce(force, PxForceMode::eACCELERATION);
    }
}

VPMath::Vector3 RigidBodyManager::GetGobalLocation(uint32_t entityID)
{
    auto temp = GetRigidBody(entityID);

    if (Reflection::GetTypeID<DynamicRigidBody>() == temp->GetTypeID())
    {
        auto dynamicbody = static_cast<DynamicRigidBody*>(temp.get());
        auto pose = dynamicbody->GetPxDynamicRigid()->getGlobalPose();
        return { pose.p.x, pose.p.y, pose.p.z };
    }
    else if (Reflection::GetTypeID<StaticRigidBody>() == temp->GetTypeID())
    {
        auto staticbody = static_cast<StaticRigidBody*>(temp.get());
        auto pose = staticbody->GetPxStaticRigid()->getGlobalPose();
        return { pose.p.x, pose.p.y, pose.p.z };
    }
    else
    {
        assert(false); // Add an assert to handle the unexpected case
        return {};
    }
}

VPMath::Quaternion RigidBodyManager::GetGobalQuaternion(uint32_t entityID)
{
    auto temp = GetRigidBody(entityID);

    if (Reflection::GetTypeID<DynamicRigidBody>() == temp->GetTypeID())
    {
        auto dynamicbody = static_cast<DynamicRigidBody*>(temp.get());
        auto pose = dynamicbody->GetPxDynamicRigid()->getGlobalPose();
        VPMath::Quaternion tempQuat = { pose.q.x, pose.q.y, pose.q.z, pose.q.w };
        tempQuat.Normalize(); // Normalize quaternion after getting it
        return tempQuat;
    }
    else if (Reflection::GetTypeID<StaticRigidBody>() == temp->GetTypeID())
    {
        auto staticbody = static_cast<StaticRigidBody*>(temp.get());
        auto pose = staticbody->GetPxStaticRigid()->getGlobalPose();
        VPMath::Quaternion tempQuat = { pose.q.x, pose.q.y, pose.q.z, pose.q.w };
        tempQuat.Normalize(); // Normalize quaternion after getting it
        return tempQuat;
    }
    else
    {
        assert(false); // Add an assert to handle the unexpected case
        return {};
    }
}

void RigidBodyManager::AddBodyScene(std::shared_ptr<RigidBody> body)
{
    EventManager::GetInstance().ScheduleEvent("OnAddBodyScene", body);
}
