#include "pch.h"
#include "CapsuleController.h"
#include "VPPhysicsStructs.h"
#include "ControllerHitCallback.h"


CapsuleController::CapsuleController():Controller{}
{
}

bool CapsuleController::Initialize(CapsuleControllerInfo info, physx::PxControllerManager* controllerManager, physx::PxMaterial* material, PhysicsInfo physicinfo)
{
    ControllerInit(info.Info, material, physicinfo);

    // Set up capsule controller description
    physx::PxCapsuleControllerDesc desc;
    desc.height = info.height;
    desc.radius = info.radius;
    desc.contactOffset = info.contactOffset;
    desc.stepOffset = info.stepOffset;
    desc.nonWalkableMode = physx::PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
    desc.slopeLimit = info.slopeLimit;
    desc.maxJumpHeight = 10.f;
    // Directly use the position from info.position without adjustment
    desc.position = physx::PxExtendedVec3(info.position.x, info.position.y, info.position.z);
    desc.userData = &m_UserData;

    desc.material = material;
    //std::shared_ptr<MyControllerFilterCallback> controllerFilterCallback = std::make_shared<MyControllerFilterCallback>();
    m_ControllerHitCallback = std::make_shared<ControllerHitCallback>();
    desc.reportCallback = dynamic_cast<physx::PxUserControllerHitReport*>(m_ControllerHitCallback.get());
    m_Controller = controllerManager->createController(desc);
    
    if (!m_Controller)
        return false;

    // Get the rigid body actor and shape
    physx::PxRigidDynamic* body = m_Controller->getActor();
    if (!body)
        return false;

    physx::PxShape* shape = nullptr;
    int shapeCount = body->getNbShapes();
    body->getShapes(&shape, shapeCount);
    if (!shape)
        return false;

    // Set solver iteration counts
    body->setSolverIterationCounts(8, 4);
    body->userData = &m_UserData;
    shape->userData = &m_UserData;

    // Set contact and rest offsets for the shape
    shape->setContactOffset(0.02f);
    shape->setRestOffset(0.01f);




    // Set collision filter data
    physx::PxFilterData filterData;
    filterData.word0 = static_cast<int>(info.Info.LayerNumber);
    filterData.word1 = physicinfo.CollisionMatrix[static_cast<int>(info.Info.LayerNumber)];
    shape->setSimulationFilterData(filterData);





    // Apply shape offset based on pivot and local offset
    //SetShapeOffset(info.Info.Pivot, info.LocalOffset);

    return true;
}

void CapsuleController::ResizeCapsuleControllerSize(float radius, float height)
{
    if (m_Controller)
    {
        // Cast to PxCapsuleController to access specific capsule properties
        physx::PxCapsuleController* capsuleController = static_cast<physx::PxCapsuleController*>(m_Controller);

        if (capsuleController)
        {
            // Set the new radius and height for the capsule controller
            capsuleController->setRadius(radius);
            capsuleController->setHeight(height);
        }
    }
}



void CapsuleController::UpdateCapsuleDimensions(CapsuleControllerInfo info)
{
	if (m_Controller)
	{
        physx::PxCapsuleController* capsuleController = static_cast<physx::PxCapsuleController*>(m_Controller);
		if (capsuleController)
		{
			// Ä¸½¶ Å©±â ¾÷µ¥ÀÌÆ®
			capsuleController->setRadius(info.radius);
			capsuleController->setHeight(info.height);
		}
	}
}

void CapsuleController::UpdateCapsuleSize(const CapsuleControllerInfo& newInfo)
{
    if (m_Controller)
    {
        // Current capsule controller
        physx::PxCapsuleController* capsuleController = static_cast<physx::PxCapsuleController*>(m_Controller);
        if (capsuleController)
        {
            // Get current height and radius
            float originalHeight = capsuleController->getHeight();
            float originalRadius = capsuleController->getRadius();

            // New height and radius
            float newHeight = newInfo.height;
            float newRadius = newInfo.radius;

            if (originalHeight == newHeight && originalRadius == newRadius)
                return;  // No change in size

            // Update capsule dimensions
            UpdateCapsuleDimensions(newInfo);

            // Apply pivot logic and LocalOffset in shape offset
            SetShapeOffset(newInfo.Info.Pivot,newInfo.Info.LocalOffset);
        }
    }
}

void CapsuleController::SetShapeOffset(ControllerPivot pivot, VPMath::Vector3 offset)
{
    if (m_Controller)
    {
        // Cast to PxCapsuleController
        physx::PxCapsuleController* capsuleController = static_cast<physx::PxCapsuleController*>(m_Controller);
        if (capsuleController)
        {
            // Get the current height and radius from the capsule controller
            float height = capsuleController->getHeight();  // Full height of the cylindrical part
            float radius = capsuleController->getRadius();

            // Default shape offset
            VPMath::Vector3 shapeOffset = offset;

            // Calculate full capsule height (radius + height/2)
            float fullCapsuleHeight = radius + (height * 0.5f);

            // Adjust the local pose based on the pivot type
            switch (pivot)
            {
            case ControllerPivot::FOOT:
                // Align the shape's bottom (foot) with the position
                // Since Y-up in left-hand system, we move the shape UP to align the bottom
                shapeOffset.y += fullCapsuleHeight;
                break;

            case ControllerPivot::CENTER:
                // No adjustment needed; position is already at the center of the capsule
                break;

            case ControllerPivot::END:
                // If END is encountered, we simply ignore it (no action required)
                return;

            default:
                // Handle any other cases that may not be valid
                return;
            }

            // Apply the local pose offset to the shape
            physx::PxRigidDynamic* body = m_Controller->getActor();
            physx::PxShape* shape = nullptr;
            int shapeCount = body->getNbShapes();
            body->getShapes(&shape, shapeCount);

            if (shape)
            {
                physx::PxTransform localPose(physx::PxVec3(shapeOffset.y, -shapeOffset.x, -shapeOffset.z));
                auto a = shape->getLocalPose();

                shape->setLocalPose(localPose);
                auto b = shape->getLocalPose();

            }
        }
    }
}


CapsuleController::~CapsuleController()
{
}
