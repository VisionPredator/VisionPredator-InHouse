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
	// �ֻ���� Entity �� ����ϱ�
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

    // ���� ���¸� �����ϱ� ���� �÷��׸� �ʱ�ȭ; ó������ ��� 0 (���ŵ��� ����)���� ����
    std::vector<bool> indicesToRemove(newupdatevector.size(), false);

	size_t i = 0;
	try 
    {
		while (i < newupdatevector.size())
        {
			// �̹� ���ŵ� ��ƼƼ �ǳʶ�
			if (indicesToRemove[i])
			{
				i++;
				continue;
			}

			TransformComponent* current = newupdatevector.at(i); // ������ ������ ���� .at() ���
            if (!current->GetEntity())
            {
                indicesToRemove[i] = true;          // ���� ������� ǥ��
                i++;
                continue; // �� ������Ʈ�� ���� ������ �˻� ����
            }

			uint32_t currentEntityID = current->GetEntityID();
			bool HaveParentEntity = false;
			// **���ο� üũ**: ���� ������Ʈ�� �θ� ���ٸ�, �ٷ� updateList�� �߰�
			if (!current->HasComponent<Parent>())
			{
				updateList.push_back(current);    // �ٷ� updateList�� �߰�
				indicesToRemove[i] = true;          // ���� ������� ǥ��
				i++;
				continue; // �� ������Ʈ�� ���� ������ �˻� ����
            }

            // �θ�-�ڽ� ���� Ȯ��
            for (size_t j = 0; j < newupdatevector.size(); j++)
            {
                if (i == j || indicesToRemove[j] ) // �ڽ� �Ǵ� �̹� ���ŵ� ��ƼƼ �ǳʶ�
                    continue;

                TransformComponent* other = newupdatevector.at(j); // ������ ������ ���� .at() ���
                uint32_t otherEntityID = other->GetEntityID();
               //auto cur=  GetSceneManager()->GetComponent<IDComponent>(currentEntityID);
               //auto next=  GetSceneManager()->GetComponent<IDComponent>(otherEntityID);
               //if (cur->Name == "WeaponCabinet" || next->Name == "WeaponCabinet")
               //{
               //    int a = 5;
               //    a = 6;
               //}
                // current�� other�� �θ��� ���, other�� ���� ������� ǥ��
                if (GetSceneManager()->CheckParent(currentEntityID, otherEntityID)) 
                {
                    HaveParentEntity = true;
                    indicesToRemove[j] = true; // �ڽ��� ���� ������� ǥ��
                    break;
                }
                // other�� current�� �θ��� ���, current�� ���� ������� ǥ��
                else if (GetSceneManager()->CheckParent(otherEntityID, currentEntityID)) 
                {
                    HaveParentEntity = true;
                    indicesToRemove[i] = true; // current�� ���� ������� ǥ��
                    break;
                }
            }

            // �θ�-�ڽ� ���谡 �߰ߵ��� ������, current�� updateList�� �߰�
            if (!HaveParentEntity && current->GetEntity()) 
            {
                updateList.push_back(current);
                indicesToRemove[i] = true; // ���� ������� ǥ��
            }

            i++;
        }

        // indicesToRemove ���� false�� ��ƼƼ���� updateList�� �߰��Ͽ� �ߺ� ����
        for (size_t k = 0; k < newupdatevector.size(); k++)
        {
            if (!indicesToRemove[k])
            {
                updateList.push_back(newupdatevector[k]);
            }
        }

        // updateList�� �ִ� ������Ʈ���� ��ȯ�� ������Ʈ
        for (TransformComponent* comp : updateList)
            CalculateTransform_Parent(comp);

        // �����ִ� ��Ұ� ������ newupdatevector�� ���
        newupdatevector.clear();
    }
    catch (const std::out_of_range& e) 
    {
        // .at()���� ���� ��ҿ� ������ �� ������ �ʰ��� ��� ó��
        std::cerr << "����: newupdatevector ������ �ʰ��� ����: " << e.what() << std::endl;
        return;  // ���� �߻� �� ���� ����
    }
    catch (const std::exception& e) 
    {
        // ��Ÿ �Ϲ����� ���� ó��
        std::cerr << "����: newUpdate���� ���� �߻�: " << e.what() << std::endl;
        return;  // ���� �߻� �� ���� ����
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
    //VP_ASSERT(temp, "Depose ����");
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
       //VP_ASSERT(temp, "Depose ����");
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

	////VP_ASSERT(temp, "Depose ����");
    
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

    //VP_ASSERT(temp, "Depose ����");

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

