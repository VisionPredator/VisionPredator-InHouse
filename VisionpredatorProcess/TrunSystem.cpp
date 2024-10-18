#include "pch.h"
#include "TrunSystem.h"
#include "VisPredComponents.h"
TrunSystem::TrunSystem(std::shared_ptr<SceneManager> SceneManager) :System(SceneManager)
{
}

void TrunSystem::FixedUpdate(float deltaTime)
{
	COMPLOOP(TrunComponent, truncomp)
	{
        if (truncomp.Finished)
        {
            GetSceneManager()->DestroyEntity(truncomp.GetEntityID());
        }
		if (truncomp.ProgressTime>= truncomp.MoveTime )
		{
			truncomp.Finished = true;
			truncomp.ProgressTime = truncomp.MoveTime;
		}
		auto quat =VPMath::Quaternion::Slerp(truncomp.StartPoint, truncomp.EndPoint, (truncomp.ProgressTime / truncomp.MoveTime));

		truncomp.GetComponent<TransformComponent>()->SetLocalQuaternion(quat);
		truncomp.ProgressTime+=deltaTime;
	}

}

void TrunSystem::Initialize()
{
	COMPLOOP(TrunComponent, truncomp)
	{
		Start(truncomp.GetEntityID());
	}

}
void TrunSystem::Start(uint32_t gameObjectId) {
    if (!GetSceneManager()->HasComponent<TrunComponent>(gameObjectId))
        return;

    auto truncomp = GetSceneManager()->GetComponent<TrunComponent>(gameObjectId);
    auto transcomp = GetSceneManager()->GetComponent<TransformComponent>(gameObjectId);

    // Determine direction based on Is_Left (left is negative, right is positive)
    float direction = truncomp->Is_Left ? 1.0f : -1.0f;

    // We always work in local space, get the proper axis for rotation
    VPMath::Vector3 rotationAxis;

    if (truncomp->Is_X) {
        // Rotate around the local up vector (Y-axis)
        rotationAxis = VPMath::Vector3(0.0f, 1.0f, 0.0f);
    }
    else {
        // Rotate around the local right vector (X-axis)
        rotationAxis = VPMath::Vector3(1.0f, 0.0f, 0.0f);
    }

    // Normalize the rotation axis
    rotationAxis.Normalize();

    // Get the local rotation quaternion
    VPMath::Quaternion localRotation = transcomp->Local_Quaternion;

    // Determine the start and end angles (adjust for direction)
    float startAngle = direction * -DegToRad(truncomp->Angle / 2);
    float endAngle = direction * DegToRad(truncomp->Angle / 2);

    // Create quaternions for the start and end rotations
    VPMath::Quaternion rotationStart = VPMath::Quaternion::CreateFromAxisAngle(rotationAxis, startAngle);
    VPMath::Quaternion rotationEnd = VPMath::Quaternion::CreateFromAxisAngle(rotationAxis, endAngle);
    // Apply the rotations relative to the current local quaternion
    truncomp->StartPoint = rotationStart * localRotation;
    truncomp->EndPoint = rotationEnd * localRotation;
    transcomp->SetWorldQuaternion(truncomp->StartPoint);
}

void TrunSystem::Finish(uint32_t gameObjectId)
{
}

void TrunSystem::Finalize()
{
}
