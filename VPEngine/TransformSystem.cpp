#include "pch.h"
#include "TransformSystem.h"
#include "Components.h"
#include <queue>
#include <iostream>


TransformSystem::TransformSystem(SceneManager* sceneManager)
	:System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnSetParentAndChild", CreateSubscriber(&TransformSystem::OnSetParentAndChild));
	EventManager::GetInstance().Subscribe("OnRelaseParentAndChild", CreateSubscriber(&TransformSystem::OnRelaseParentAndChild));
	EventManager::GetInstance().Subscribe("OnUpdateTransfomData", CreateSubscriber(&TransformSystem::OnUpdateTransfomData));

}
void TransformSystem::Update(float deltaTime)	///transform update ���� �����ϴ� ����
{
	///������Ʈ ������ �ſ� �߿��ϰ� ������.
	///���� �� Parent�� ������Ʈ �Ǳ����� �ٸ��͵��� ������Ʈ �ȴٸ�?? ������ ���������!!
	auto a = Reflection::GetTypeID<Parent>();
	auto b = Reflection::GetTypeID<Children>();
	std::list<uint32_t> updatelist;
	///�ֻ���� Entity �� ����ϱ�. �ٵ� ���� ����ϴ°� ������ ����غ���.
	for (TransformComponent& comp : CompIter<TransformComponent>(m_SceneManager))
	{
		if (comp.HasComponent<Parent>())
			continue;
		updatelist.push_back(comp.GetEntityID());

	}
	while (!updatelist.empty())
	{
		uint32_t transformid = updatelist.front();

		TransformComponent* transformcomp = m_SceneManager->GetComponent<TransformComponent>(transformid);

		CalulateTransform(transformcomp);
		updatelist.pop_front();
		if (!transformcomp->HasComponent<Children>())
			continue;
		auto* children = transformcomp->GetComponent<Children>();
		updatelist.insert(updatelist.end(), children->ChildrenID.begin(), children->ChildrenID.end());
	}


}
/// <summary>
/// �̺�
/// </summary>
/// <param name="Parent_Child"></param>
void TransformSystem::OnSetParentAndChild(std::any Parent_Child)
{
	auto [ParentID, ChildID] = std::any_cast<std::pair<uint32_t, uint32_t>>(Parent_Child);

	if (!m_SceneManager->HasEntity(ChildID) || !m_SceneManager->HasEntity(ParentID))
		return;
	TransformComponent* childTransform = m_SceneManager->GetComponent<TransformComponent>(ChildID);

	TransformComponent* parentTransform = m_SceneManager->GetComponent<TransformComponent>(ParentID);

	VPMath::Matrix newLocalTransform = childTransform->WorldTransform * parentTransform->WorldTransform.Invert();
	newLocalTransform.Decompose(childTransform->Local_Scale, childTransform->Local_Quaternion, childTransform->Local_Location);
	Update(0);

}

void TransformSystem::OnUpdateTransfomData(std::any data)
{
	Update(0);
}

void TransformSystem::OnRelaseParentAndChild(std::any Child)
{
	auto ChildID = std::any_cast<uint32_t>(Child);

	TransformComponent* childTransform = m_SceneManager->GetComponent<TransformComponent>(ChildID);
	childTransform->WorldTransform.Decompose(childTransform->Local_Scale, childTransform->Local_Quaternion, childTransform->Local_Location);

	Update(0);
}



void TransformSystem::CalulateTransform(TransformComponent* transform)
{
	/// �ڽ����� Dynamic�� ������� �� ������ ����.
	///������ �ٲ���°�? + Parent ��ġ ���
	if (transform->Previous_Location != transform->Local_Location ||
		transform->Previous_Rotation != transform->Local_Rotation ||
		transform->Previous_Quaternion != transform->Local_Quaternion ||
		transform->Previous_Scale != transform->Local_Scale)
		CalulateTransform_Local(transform);
	///��ü�� ���� ��ǥ�� �ٲ���°�?  + Parent ��ġ ���
	else if (transform->World_Location != transform->Previous_WorldLocation ||
		transform->World_Rotation != transform->Previous_WorldRotation ||
		transform->World_Quaternion != transform->Previous_WorldQuaternion ||
		transform->World_Scale != transform->Previous_WorldScale)
		CalulateTransform_World(transform);
	///�Ѵ� �� �ٲ��, Parent�� ��ġ�� �ٲ���°�?
	else if (transform->HasComponent<Parent>())
	{
		TransformComponent parentComponent = *m_SceneManager->GetComponent<TransformComponent>(transform->GetComponent<Parent>()->ParentID);
		VPMath::Matrix WorldTransform = transform->LocalTransform * parentComponent.WorldTransform;
		if (transform->WorldTransform != WorldTransform)
		{
			// Update world transform matrix
			transform->WorldTransform = WorldTransform;
			transform->WorldTransform.Decompose(transform->World_Scale, transform->World_Quaternion, transform->World_Location);
			transform->World_Rotation = transform->World_Quaternion.ToEuler() * 180 / VPMath::XM_PI;

			transform->Previous_WorldScale = transform->World_Scale;
			transform->Previous_WorldRotation = transform->World_Rotation;
			transform->Previous_WorldQuaternion = transform->World_Quaternion;
			transform->Previous_WorldLocation = transform->World_Location;
			transform->FrontVector = -transform->WorldTransform.Forward();
			transform->RightVector = transform->WorldTransform.Right();
			transform->UpVector = transform->WorldTransform.Up();
		}
	}


}

void TransformSystem::CalulateTransform_Dynamic(TransformComponent* transform)
{
	CalulateTransform_World(transform);
}

void TransformSystem::CalulateTransform_World(TransformComponent* transform)
{

	///ȸ���� �ٲ���°�?
	if (transform->World_Rotation != transform->Previous_WorldRotation)
	{
		auto wrapAngle = [](float angle) -> float {
			while (angle <= -360.0f) angle += 720.0f;
			while (angle > 360.0f) angle -= 720.0f;
			return angle;
			};

		transform->World_Rotation.x = wrapAngle(transform->World_Rotation.x);
		transform->World_Rotation.y = wrapAngle(transform->World_Rotation.y);
		transform->World_Rotation.z = wrapAngle(transform->World_Rotation.z);
		transform->World_Quaternion = VPMath::Quaternion::CreateFromYawPitchRoll(
			VPMath::XMConvertToRadians(transform->World_Rotation.y),
			VPMath::XMConvertToRadians(transform->World_Rotation.x),
			VPMath::XMConvertToRadians(transform->World_Rotation.z));
	}
	///Quaternion�� �ٲ���°�?
	else if (transform->World_Quaternion != transform->Previous_WorldQuaternion)
	{
		transform->World_Rotation = (transform->World_Quaternion.ToEuler() * 180 / VPMath::XM_PI);
	}

	// Update world transform matrix
	transform->WorldTransform = VPMath::Matrix::CreateScale(transform->World_Scale) * VPMath::Matrix::CreateFromQuaternion(transform->World_Quaternion) * VPMath::Matrix::CreateTranslation(transform->World_Location);
	transform->Previous_WorldScale = transform->World_Scale;
	transform->Previous_WorldRotation = transform->World_Rotation;
	transform->Previous_WorldQuaternion = transform->World_Quaternion;
	transform->Previous_WorldLocation = transform->World_Location;
	transform->FrontVector = -transform->WorldTransform.Forward();
	transform->RightVector = transform->WorldTransform.Right();
	transform->UpVector = transform->WorldTransform.Up();
	// Check if there is a parent and recalculate local transform
	if (transform->HasComponent<Parent>())
	{
		const TransformComponent parentTransform = *m_SceneManager->GetComponent<TransformComponent>(transform->GetComponent<Parent>()->ParentID);
		VPMath::Matrix inverseParentWorldTransform = parentTransform.WorldTransform.Invert();
		transform->LocalTransform = transform->WorldTransform * inverseParentWorldTransform;
	}
	else 
		transform->LocalTransform = transform->WorldTransform;

	transform->LocalTransform.Decompose(transform->Local_Scale, transform->Local_Quaternion, transform->Local_Location);

	transform->Local_Rotation = transform->Local_Quaternion.ToEuler() * 180 / VPMath::XM_PI;

	transform->Previous_Scale = transform->Local_Scale;
	transform->Previous_Rotation = transform->Local_Rotation;
	transform->Previous_Quaternion = transform->Local_Quaternion;
	transform->Previous_Location = transform->Local_Location;
}

void TransformSystem::CalulateTransform_Local(TransformComponent* transform)
{

	///ȸ���� �ٲ���°�?
	if (transform->Local_Rotation != transform->Previous_Rotation)
	{
		auto wrapAngle = [](float angle) -> float {
			while (angle <= -360.0f) angle += 720.0f;
			while (angle > 360.0f) angle -= 720.0f;
			return angle;
			};

		transform->Local_Rotation.x = wrapAngle(transform->Local_Rotation.x);
		transform->Local_Rotation.y = wrapAngle(transform->Local_Rotation.y);
		transform->Local_Rotation.z = wrapAngle(transform->Local_Rotation.z);
		transform->Local_Quaternion = VPMath::Quaternion::CreateFromYawPitchRoll(
			VPMath::XMConvertToRadians(transform->Local_Rotation.y),
			VPMath::XMConvertToRadians(transform->Local_Rotation.x),
			VPMath::XMConvertToRadians(transform->Local_Rotation.z));
	}
	///Quaternion�� �ٲ���°�?
	else if (transform->Local_Quaternion != transform->Previous_Quaternion)
	{
		transform->Local_Rotation = (transform->Local_Quaternion.ToEuler() * 180 / VPMath::XM_PI);
	}

	transform->LocalTransform = VPMath::Matrix::CreateScale(transform->Local_Scale) *
		VPMath::Matrix::CreateFromQuaternion(transform->Local_Quaternion) *
		VPMath::Matrix::CreateTranslation(transform->Local_Location);

	transform->Previous_Scale = transform->Local_Scale;
	transform->Previous_Rotation = transform->Local_Rotation;
	transform->Previous_Quaternion = transform->Local_Quaternion;
	transform->Previous_Location = transform->Local_Location;



	///Parent�� �ִ°�?
	if (transform->HasComponent<Parent>())
	{
		const uint32_t parentID = transform->GetComponent<Parent>()->ParentID;
		const TransformComponent parentTransform = *m_SceneManager->GetComponent<TransformComponent>(parentID);
		transform->WorldTransform = transform->LocalTransform * parentTransform.WorldTransform;
	}
	else
		transform->WorldTransform = transform->LocalTransform;


	// Update world transform matrix
	transform->WorldTransform.Decompose(transform->World_Scale, transform->World_Quaternion, transform->World_Location);
	transform->World_Rotation = transform->World_Quaternion.ToEuler() * 180 / VPMath::XM_PI;

	transform->Previous_WorldScale = transform->World_Scale;
	transform->Previous_WorldRotation = transform->World_Rotation;
	transform->Previous_WorldQuaternion = transform->World_Quaternion;
	transform->Previous_WorldLocation = transform->World_Location;
	transform->FrontVector = -transform->WorldTransform.Forward();
	transform->RightVector = transform->WorldTransform.Right();
	transform->UpVector = transform->WorldTransform.Up();

}

