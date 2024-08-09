#include "pch.h"
#include "TransformSystem.h"
#include "Components.h"
#include <queue>
#include <iostream>

TransformSystem::TransformSystem(std::shared_ptr<SceneManager> sceneManager)
    : System(sceneManager)
{
    EventManager::GetInstance().Subscribe("OnSetParentAndChild", CreateSubscriber(&TransformSystem::OnSetParentAndChild));
    EventManager::GetInstance().Subscribe("OnRelaseParentAndChild", CreateSubscriber(&TransformSystem::OnRelaseParentAndChild));
    EventManager::GetInstance().Subscribe("OnUpdateTransfomData", CreateSubscriber(&TransformSystem::OnUpdateTransfomData));
}

void TransformSystem::Update(float deltaTime)
{
	std::vector<TransformComponent*> updateList;

	// 최상단의 Entity 만 기억하기
	for (TransformComponent& comp : COMPITER(TransformComponent))
	{
		if (!comp.HasComponent<Parent>())
		{
			updateList.push_back(&comp);
		}
	}
	if (!updateList.empty())
		for (TransformComponent* transformComp : updateList)
		{
			CalculateTransform_new(transformComp, false);
		}
}
void TransformSystem::OnSetParentAndChild(std::any parentChild)
{
    auto [parentID, childID] = std::any_cast<std::pair<uint32_t, uint32_t>>(parentChild);

    if (!GetSceneManager()->HasEntity(childID) || !GetSceneManager()->HasEntity(parentID))
    {
        return;
    }

    TransformComponent* childTransform = GetSceneManager()->GetComponent<TransformComponent>(childID);
    TransformComponent* parentTransform = GetSceneManager()->GetComponent<TransformComponent>(parentID);

    if (childTransform && parentTransform)
    {
        VPMath::Matrix newLocalTransform = childTransform->WorldTransform * parentTransform->WorldTransform.Invert();
        newLocalTransform.Decompose(childTransform->Local_Scale, childTransform->Local_Quaternion, childTransform->Local_Location);
    }

    Update(0);
}

void TransformSystem::OnUpdateTransfomData(std::any data)
{
    Update(0);
}

void TransformSystem::OnRelaseParentAndChild(std::any child)
{
    auto childID = std::any_cast<uint32_t>(child);

    TransformComponent* childTransform = GetSceneManager()->GetComponent<TransformComponent>(childID);
    if (childTransform)
    {
        childTransform->WorldTransform.Decompose(childTransform->Local_Scale, childTransform->Local_Quaternion, childTransform->Local_Location);
    }

    Update(0);
}



void TransformSystem::CalculateTransform_new(TransformComponent* transform, bool IsParentWorldChanged)
{
    if (IsLocalTransformChanged(transform))
    {
        CalculateTransformLocal(transform);
        IsParentWorldChanged = true;
    }
    else if (IsWorldTransformChanged(transform))
    {
        CalculateTransformWorld(transform);
        IsParentWorldChanged = true;
    }
	else if (IsParentWorldChanged)
	{
		TransformComponent* parentComponent = GetSceneManager()->GetComponent<TransformComponent>(transform->GetComponent<Parent>()->ParentID);
		VPMath::Matrix worldTransform = transform->LocalTransform * parentComponent->WorldTransform;
		if (transform->WorldTransform != worldTransform)
		{
			UpdateWorldTransform(transform, worldTransform);
			IsParentWorldChanged = true;
		}
	}

	if (transform->HasComponent<Children>())
    {
        auto children = transform->GetComponent<Children>();
        for (auto childID : children->ChildrenID)
        {
            TransformComponent* childTransform = GetSceneManager()->GetComponent<TransformComponent>(childID);
            if (childTransform)
            {
                CalculateTransform_new(childTransform, IsParentWorldChanged);
            }
        }
    }
}

bool TransformSystem::IsLocalTransformChanged(TransformComponent* transform)
{
    return transform->Previous_Location != transform->Local_Location ||
        transform->Previous_Rotation != transform->Local_Rotation ||
        transform->Previous_Quaternion != transform->Local_Quaternion ||
        transform->Previous_Scale != transform->Local_Scale;
}

bool TransformSystem::IsWorldTransformChanged(TransformComponent* transform)
{
    return transform->World_Location != transform->Previous_WorldLocation ||
        transform->World_Rotation != transform->Previous_WorldRotation ||
        transform->World_Quaternion != transform->Previous_WorldQuaternion ||
        transform->World_Scale != transform->Previous_WorldScale;
}

void TransformSystem::UpdateWorldTransform(TransformComponent* transform, const VPMath::Matrix& worldTransform)
{
    transform->WorldTransform = worldTransform;
    transform->WorldTransform.Decompose(transform->World_Scale, transform->World_Quaternion, transform->World_Location);
    transform->World_Rotation = transform->World_Quaternion.ToEuler() * 180 / VPMath::XM_PI;

    transform->Previous_WorldScale = transform->World_Scale;
    transform->Previous_WorldRotation = transform->World_Rotation;
    transform->Previous_WorldQuaternion = transform->World_Quaternion;
    transform->Previous_WorldLocation = transform->World_Location;

    transform->FrontVector = -transform->WorldTransform.Forward();
    transform->RightVector = transform->WorldTransform.Right();
    transform->UpVector = transform->WorldTransform.Up();

    transform->FrontVector.Normalize();
    transform->RightVector.Normalize();
    transform->UpVector.Normalize();
}

void TransformSystem::CalculateTransformDynamic(TransformComponent* transform)
{
    CalculateTransformWorld(transform);
}

void TransformSystem::CalculateTransformWorld(TransformComponent* transform)
{
    if (transform->World_Rotation != transform->Previous_WorldRotation)
    {
        WrapAngle(transform->World_Rotation);
        transform->World_Quaternion = VPMath::Quaternion::CreateFromYawPitchRoll(
            VPMath::XMConvertToRadians(transform->World_Rotation.y),
            VPMath::XMConvertToRadians(transform->World_Rotation.x),
            VPMath::XMConvertToRadians(transform->World_Rotation.z));
    }
    else if (transform->World_Quaternion != transform->Previous_WorldQuaternion)
    {
        transform->World_Rotation = transform->World_Quaternion.ToEuler() * 180 / VPMath::XM_PI;
    }

    transform->WorldTransform = VPMath::Matrix::CreateScale(transform->World_Scale) *
        VPMath::Matrix::CreateFromQuaternion(transform->World_Quaternion) *
        VPMath::Matrix::CreateTranslation(transform->World_Location);

    UpdatePreviousWorldTransform(transform);

    if (transform->HasComponent<Parent>())
    {
        TransformComponent* parentTransform = GetSceneManager()->GetComponent<TransformComponent>(transform->GetComponent<Parent>()->ParentID);
        if (parentTransform)
        {
            VPMath::Matrix inverseParentWorldTransform = parentTransform->WorldTransform.Invert();
            transform->LocalTransform = transform->WorldTransform * inverseParentWorldTransform;
        }
    }
    else
    {
        transform->LocalTransform = transform->WorldTransform;
    }

    transform->LocalTransform.Decompose(transform->Local_Scale, transform->Local_Quaternion, transform->Local_Location);
    transform->Local_Rotation = transform->Local_Quaternion.ToEuler() * 180 / VPMath::XM_PI;

    UpdatePreviousLocalTransform(transform);
}

void TransformSystem::CalculateTransformLocal(TransformComponent* transform)
{
    if (transform->Local_Rotation != transform->Previous_Rotation)
    {
        WrapAngle(transform->Local_Rotation);
        transform->Local_Quaternion = VPMath::Quaternion::CreateFromYawPitchRoll(
            VPMath::XMConvertToRadians(transform->Local_Rotation.y),
            VPMath::XMConvertToRadians(transform->Local_Rotation.x),
            VPMath::XMConvertToRadians(transform->Local_Rotation.z));
    }
    else if (transform->Local_Quaternion != transform->Previous_Quaternion)
    {
        transform->Local_Rotation = transform->Local_Quaternion.ToEuler() * 180 / VPMath::XM_PI;
    }

    transform->LocalTransform = VPMath::Matrix::CreateScale(transform->Local_Scale) *
        VPMath::Matrix::CreateFromQuaternion(transform->Local_Quaternion) *
        VPMath::Matrix::CreateTranslation(transform->Local_Location);

    UpdatePreviousLocalTransform(transform);

    if (transform->HasComponent<Parent>())
    {
        TransformComponent* parentTransform = GetSceneManager()->GetComponent<TransformComponent>(transform->GetComponent<Parent>()->ParentID);
        if (parentTransform)
        {
            transform->WorldTransform = transform->LocalTransform * parentTransform->WorldTransform;
        }
    }
    else
    {
        transform->WorldTransform = transform->LocalTransform;
    }

    UpdateWorldTransform(transform, transform->WorldTransform);
}

void TransformSystem::WrapAngle(VPMath::Vector3& rotation)
{
    auto wrap = [](float angle) -> float
        {
            while (angle <= -360.0f) angle += 720.0f;
            while (angle > 360.0f) angle -= 720.0f;
            return angle;
        };

    rotation.x = wrap(rotation.x);
    rotation.y = wrap(rotation.y);
    rotation.z = wrap(rotation.z);
}

void TransformSystem::UpdatePreviousWorldTransform(TransformComponent* transform)
{
    transform->Previous_WorldScale = transform->World_Scale;
    transform->Previous_WorldRotation = transform->World_Rotation;
    transform->Previous_WorldQuaternion = transform->World_Quaternion;
    transform->Previous_WorldLocation = transform->World_Location;
}

void TransformSystem::UpdatePreviousLocalTransform(TransformComponent* transform)
{
    transform->Previous_Scale = transform->Local_Scale;
    transform->Previous_Rotation = transform->Local_Rotation;
    transform->Previous_Quaternion = transform->Local_Quaternion;
    transform->Previous_Location = transform->Local_Location;
}
