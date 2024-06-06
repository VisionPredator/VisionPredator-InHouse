#include "pch.h"
#include "TransformSystem.h"
#include "Components.h"
#include <queue>
#include <iostream>


	TransformSystem::TransformSystem(SceneManager* sceneManager)
		:System(sceneManager)
	{

	}
	void TransformSystem::Update(float deltaTime)	///transform update ���� �����ϴ� ����
	{
		///������Ʈ ������ �ſ� �߿��ϰ� ������.
		///���� �� Parent�� ������Ʈ �Ǳ����� �ٸ��͵��� ������Ʈ �ȴٸ�?? ������ ���������!!
			
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
	void TransformSystem::CalulateTransform(TransformComponent* transform)
	{
		if (transform->Previous_Location != transform->Local_Location ||
			transform->Previous_Quaternion != transform->Local_Quaternion ||
			transform->Previous_Scale != transform->Local_Scale)
		{
			transform->Previous_Location = transform->Local_Location;
			transform->Previous_Quaternion = transform->Local_Quaternion;
			transform->Previous_Scale = transform->Local_Scale;
			transform->LocalTransform = VPMath::Matrix::CreateScale(transform->Local_Scale) *
			VPMath::Matrix::CreateFromQuaternion(transform->Local_Quaternion) *
			VPMath::Matrix::CreateTranslation(transform->Local_Location);
		}



		VPMath::Matrix tempWorldTrasnform;	//trasnform�� �ٲ������ Ȯ���ϴ� �뵵!
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
