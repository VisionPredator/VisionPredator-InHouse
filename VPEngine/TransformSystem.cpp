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
    EventManager::GetInstance().Subscribe("OnUpdateTransfomData", CreateSubscriber(&TransformSystem::OnUpdateTransfomData), EventType::SCENE);
    EventManager::GetInstance().Subscribe("OnUpdate", CreateSubscriber(&TransformSystem::OnUpdate));    
}
void TransformSystem::OnAddedComponent(std::any data)
{

}
std::vector<TransformComponent*> TransformSystem::newupdatevector;

void TransformSystem::Update(float deltaTime)
{
    newUpdate();
}

void TransformSystem::OnUpdate(std::any)
{
	newUpdate();

}

void TransformSystem::UpdateAllEntitys()
{
	// 최상단의 Entity 만 기억하기
	std::vector<TransformComponent*> updateList;

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
			CalculateTransform_Parent(transformComp);
		}

}
void TransformSystem::newUpdate() 
{
    if (newupdatevector.empty())
        return;

    std::list<TransformComponent*> updateList;

    // 제거 상태를 추적하기 위한 플래그를 초기화; 처음에는 모두 0 (제거되지 않음)으로 설정
    std::vector<bool> indicesToRemove(newupdatevector.size(), false);

	size_t i = 0;
	try 
    {
		while (i < newupdatevector.size())
        {
			// 이미 제거된 엔티티 건너뜀
			if (indicesToRemove[i])
			{
				i++;
				continue;
			}

			TransformComponent* current = newupdatevector.at(i); // 안전한 접근을 위해 .at() 사용
            if (!current->GetEntity())
            {
                indicesToRemove[i] = true;          // 제거 대상으로 표시
                i++;
                continue; // 이 컴포넌트에 대한 나머지 검사 생략
            }

			uint32_t currentEntityID = current->GetEntityID();
			bool HaveParentEntity = false;
			// **새로운 체크**: 현재 컴포넌트에 부모가 없다면, 바로 updateList에 추가
			if (!current->HasComponent<Parent>())
			{
				updateList.push_back(current);    // 바로 updateList에 추가
				indicesToRemove[i] = true;          // 제거 대상으로 표시
				i++;
				continue; // 이 컴포넌트에 대한 나머지 검사 생략
            }

            // 부모-자식 관계 확인
            for (size_t j = 0; j < newupdatevector.size(); j++)
            {
                if (i == j || indicesToRemove[j] ) // 자신 또는 이미 제거된 엔티티 건너뜀
                    continue;

                TransformComponent* other = newupdatevector.at(j); // 안전한 접근을 위해 .at() 사용
                uint32_t otherEntityID = other->GetEntityID();
               //auto cur=  GetSceneManager()->GetComponent<IDComponent>(currentEntityID);
               //auto next=  GetSceneManager()->GetComponent<IDComponent>(otherEntityID);
               //if (cur->Name == "WeaponCabinet" || next->Name == "WeaponCabinet")
               //{
               //    int a = 5;
               //    a = 6;
               //}
                // current가 other의 부모인 경우, other를 제거 대상으로 표시
                if (GetSceneManager()->CheckParent(currentEntityID, otherEntityID)) 
                {
                    HaveParentEntity = true;
                    indicesToRemove[j] = true; // 자식을 제거 대상으로 표시
                    break;
                }
                // other가 current의 부모인 경우, current를 제거 대상으로 표시
                else if (GetSceneManager()->CheckParent(otherEntityID, currentEntityID)) 
                {
                    HaveParentEntity = true;
                    indicesToRemove[i] = true; // current를 제거 대상으로 표시
                    break;
                }
            }

            // 부모-자식 관계가 발견되지 않으면, current를 updateList에 추가
            if (!HaveParentEntity && current->GetEntity()) 
            {
                updateList.push_back(current);
                indicesToRemove[i] = true; // 제거 대상으로 표시
            }

            i++;
        }

        // indicesToRemove 값이 false인 엔티티들을 updateList에 추가하여 중복 방지
        for (size_t k = 0; k < newupdatevector.size(); k++)
        {
            if (!indicesToRemove[k])
            {
                updateList.push_back(newupdatevector[k]);
            }
        }

        // updateList에 있는 컴포넌트들의 변환을 업데이트
        for (TransformComponent* comp : updateList)
            CalculateTransform_Parent(comp);

        // 남아있는 요소가 있으면 newupdatevector를 비움
        newupdatevector.clear();
    }
    catch (const std::out_of_range& e) 
    {
        // .at()으로 벡터 요소에 접근할 때 범위를 초과한 경우 처리
        std::cerr << "오류: newupdatevector 범위를 초과한 접근: " << e.what() << std::endl;
        return;  // 오류 발생 시 조기 종료
    }
    catch (const std::exception& e) 
    {
        // 기타 일반적인 예외 처리
        std::cerr << "오류: newUpdate에서 예외 발생: " << e.what() << std::endl;
        return;  // 오류 발생 시 조기 종료
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
	VPMath::Matrix newLocalTransform = childTransform->WorldTransform * parentTransform->WorldTransform.Invert();
	auto temp =newLocalTransform.NewDecompose(childTransform->Local_Scale, childTransform->Local_Quaternion, childTransform->Local_Location);
    //VP_ASSERT(temp, "Depose 실패");
    auto a = childTransform->GetComponent<IDComponent>()->Name;
    auto b = parentTransform->GetComponent<IDComponent>()->Name;

    AddUpdateData(parentTransform);
    AddUpdateData(childTransform);
}

void TransformSystem::OnUpdateTransfomData(std::any data)
{
    UpdateAllEntitys();
}

void TransformSystem::OnRelaseParentAndChild(std::any child)
{
    auto childID = std::any_cast<uint32_t>(child);

    TransformComponent* childTransform = GetSceneManager()->GetComponent<TransformComponent>(childID);
    if (childTransform)
    {
       bool temp =  childTransform->WorldTransform.NewDecompose(childTransform->Local_Scale, childTransform->Local_Quaternion, childTransform->Local_Location);
       //VP_ASSERT(temp, "Depose 실패");
    }
    AddUpdateData(childTransform);
}



void TransformSystem::UpdateDirVector(TransformComponent* transform)
{
    transform->FrontVector = transform->WorldTransform.Forward_L();
    transform->RightVector = transform->WorldTransform.Right();
    transform->UpVector = transform->WorldTransform.Up();

    transform->FrontVector.Normalize();
    transform->RightVector.Normalize();
    transform->UpVector.Normalize();
}

void TransformSystem::CalculateTransform_Child(TransformComponent* transform, bool IsParentWorldChanged)
{
    if (IsLocalTransformChanged(transform))
    {
        CalculateTransformLocal(transform);
        UpdateDirVector(transform);

        IsParentWorldChanged = true;
    }
    else if (IsWorldTransformChanged(transform))
    {
        CalculateTransformWorld(transform);
        UpdateDirVector(transform);

        IsParentWorldChanged = true;
    }
	else if (IsParentWorldChanged)
	{
		TransformComponent* parentComponent = GetSceneManager()->GetComponent<TransformComponent>(transform->GetComponent<Parent>()->ParentID);
		VPMath::Matrix worldTransform = transform->LocalTransform * parentComponent->WorldTransform;
		if (transform->WorldTransform != worldTransform)
		{
			UpdateWorldTransform(transform, worldTransform);
			UpdateDirVector(transform);

			IsParentWorldChanged = true;
		}
	}

	if (transform->HasComponent<Children>())
	{
		auto children = transform->GetComponent<Children>();
		for (auto childID : children->ChildrenID)
		{
			auto entity = GetSceneManager()->GetEntity(childID);

			if (!entity)
				return;
			TransformComponent* childTransform = entity->GetComponent<TransformComponent>();
			if (childTransform)
				CalculateTransform_Child(childTransform, IsParentWorldChanged);
		}
	}
}

void TransformSystem::CalculateTransform_Parent(TransformComponent* transform)
{
    bool IsParentWorldChanged = false;
    if (IsLocalTransformChanged(transform))
    {
        CalculateTransformLocal(transform);
        UpdateDirVector(transform);

        IsParentWorldChanged = true;
    }
    else if (IsWorldTransformChanged(transform))
    {
        CalculateTransformWorld(transform);
        UpdateDirVector(transform);

        IsParentWorldChanged = true;
    }

    if (transform->HasComponent<Children>())
    {
        auto children = transform->GetComponent<Children>();
        for (auto childID : children->ChildrenID)
        {
            TransformComponent* childTransform = GetSceneManager()->GetComponent<TransformComponent>(childID);
            if (childTransform)
            {
                CalculateTransform_Child(childTransform, IsParentWorldChanged);

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
	auto temp = transform->WorldTransform.NewDecompose(transform->World_Scale, transform->World_Quaternion, transform->World_Location);

	////VP_ASSERT(temp, "Depose 실패");
    
    transform->World_Rotation = transform->World_Quaternion.ToEuler() * 180 / VPMath::XM_PI;

    transform->Previous_WorldScale = transform->World_Scale;
    transform->Previous_WorldRotation = transform->World_Rotation;
    transform->Previous_WorldQuaternion = transform->World_Quaternion;
    transform->Previous_WorldLocation = transform->World_Location;

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

   auto  temp =transform->LocalTransform.NewDecompose(transform->Local_Scale, transform->Local_Quaternion, transform->Local_Location);

    //VP_ASSERT(temp, "Depose 실패");

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
		if (GetSceneManager()->HasComponent<TransformComponent>(transform->GetComponent<Parent>()->ParentID))
		{
			TransformComponent* parentTransform = GetSceneManager()->GetComponent<TransformComponent>(transform->GetComponent<Parent>()->ParentID);
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


void TransformSystem::BeginRenderUpdate(float deltaTime)
{

}

void TransformSystem::RenderUpdate(float deltaTime)
{
    for (TransformComponent& comp : COMPITER(TransformComponent))
    {
        debug::OBBInfo obbinfo{};
        obbinfo.OBB.Center = comp.World_Location;
        obbinfo.OBB.Extents = { 0.2f,0.2f ,0.2f };
        obbinfo.xAxisAngle = comp.World_Rotation.x;
        obbinfo.yAxisAngle = comp.World_Rotation.y;
        obbinfo.zAxisAngle = comp.World_Rotation.z;
        //m_Graphics->DrawOBB(obbinfo);
    }
}

void TransformSystem::LateRenderUpdate(float deltaTime)
{
}

void TransformSystem::EditorRenderUpdate(float deltaTime)
{
    RenderUpdate(deltaTime);
}




void TransformSystem::Initialize()
{
}

void TransformSystem::Start(uint32_t gameObjectId)
{
}

void TransformSystem::Finish(uint32_t gameObjectId)
{
}

void TransformSystem::Finalize()
{
    newupdatevector.clear();
}

void TransformSystem::LateUpdate(float deltaTime)
{
    newUpdate();
}

void TransformSystem::ComponentAdded(Component* comp)
{
    if (comp->GetHandle()->type().id() == Reflection::GetTypeID<TransformComponent>())
    {
        TransformComponent* aniComp = static_cast<TransformComponent*>(comp);
        AddUpdateData(aniComp);
        return;
    }
}

void TransformSystem::ComponentReleased(Component* comp)
{
}

