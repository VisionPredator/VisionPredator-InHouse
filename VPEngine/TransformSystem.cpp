#include "pch.h"
#include "TransformSystem.h"
#include "Components.h"
#include <queue>
#include <iostream>


	TransformSystem::TransformSystem(SceneManager* sceneManager)
		:System(sceneManager)
	{

	}
	void TransformSystem::Update(float deltaTime)	///transform update 값을 수정하는 느낌
	{
		///업데이트 순서가 매우 중요하게 느껴짐.
		///예를 들어서 Parent가 업데이트 되기전에 다른것들이 업데이트 된다면?? 문제가 생길수있음!!
			
		std::list<uint32_t> updatelist;
		///최상단의 Entity 만 기억하기. 근데 뭘로 기억하는게 좋을까 고민해보기.
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
	/// 이벤
	/// </summary>
	/// <param name="Parent_Child"></param>
	void SetParentAndChild(std::any Parent_Child)
	{





	}

	void RelaseParentAndChild(std::any Parent_Child)
	{





	}

	void TransformSystem::CalulateTransform(TransformComponent* transform)
	{
		if (transform->Previous_Location != transform->Local_Location ||
			transform->Previous_Rotation != transform->Local_Rotation ||
			transform->Previous_Quaternion != transform->Local_Quaternion ||
			transform->Previous_Scale != transform->Local_Scale)
		{
			// Lambda function to wrap the angle
			auto wrapAngle = [](float angle) -> float {
				while (angle <= -360.0f) angle += 720.0f;
				while (angle > 360.0f) angle -= 720.0f;
				return angle;
				};
			transform->Local_Rotation.x = wrapAngle(transform->Local_Rotation.x);
			transform->Local_Rotation.y = wrapAngle(transform->Local_Rotation.y);
			transform->Local_Rotation.z = wrapAngle(transform->Local_Rotation.z);

			transform->Previous_Location = transform->Local_Location;
			transform->Previous_Rotation = transform->Local_Rotation;
			transform->Previous_Scale = transform->Local_Scale;
			
			// Update the quaternion from the wrapped euler angles
			transform->Local_Quaternion =VPMath::Quaternion::CreateFromYawPitchRoll(
				VPMath::XMConvertToRadians(transform->Local_Rotation.y),
				VPMath::XMConvertToRadians(transform->Local_Rotation.x),
				VPMath::XMConvertToRadians(transform->Local_Rotation.z)
			);

			transform->LocalTransform = VPMath::Matrix::CreateScale(transform->Local_Scale) *
			VPMath::Matrix::CreateFromQuaternion(transform->Local_Quaternion) *
			VPMath::Matrix::CreateTranslation(transform->Local_Location);
		}



		VPMath::Matrix tempWorldTrasnform;	//trasnform이 바뀌었는지 확인하는 용도!
		if (transform->HasComponent<Parent>())
		{
			const TransformComponent parentTransform =*m_SceneManager->GetComponent<TransformComponent>(transform->GetComponent<Parent>()->ParentID);
			tempWorldTrasnform = transform->LocalTransform * parentTransform.WorldTransform;
		}
		else
			tempWorldTrasnform = transform->LocalTransform;

		if (transform->WorldTransform != tempWorldTrasnform)
		{
			transform->WorldTransform = tempWorldTrasnform;
			transform->WorldTransform.Decompose(transform->World_Scale, transform->World_Quaternion, transform->World_Location);
			transform->FrontVector = transform->WorldTransform.Forward();
			transform->FrontVector.Normalize();
			transform->RightVector = transform->WorldTransform.Right();
			transform->RightVector.Normalize();

			transform->UpVector = transform->WorldTransform.Up();
			transform->UpVector.Normalize();

		}
	}
	void TransformSystem::FixedUpdate(float deltaTime)
	{

	}
