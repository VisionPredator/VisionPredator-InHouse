#include "pch.h"
#include "Entity.h"
#include "Components.h"
Entity::Entity(uint32_t entityID)
{
	m_EntityID = entityID;
}

Entity::~Entity()
{
	for (auto& comp:m_OwnedComp)
	{
		comp.second = nullptr;
		delete comp.second;
	}
	m_OwnedComp.clear();
}
void Entity::AddComponentToMap(Component* comp)
{
	/// ��Ÿ�� ����Ͽ� T�� Ÿ���߷��ؼ� ID���� �ش��ϴ� ������Ʈ�� �߰��Ҽ� ��������.
	if (HasComponent(comp->GetTypeID()))
	{
	VP_ASSERT(false, "������Ʈ�� �����մϴ�");
	return;
	}
	m_OwnedComp[comp->GetTypeID()] = comp;
}

Component* Entity::AddComponent(entt::id_type compID)
{
	if (HasComponent(compID))
	{
		VP_ASSERT(false, "�̹� �ִ� Component �Դϴ�.")
			return nullptr;
	}
	auto metaType = entt::resolve(compID);

	if (metaType)
	{
		// ��Ÿ Ÿ�����κ��� �ν��Ͻ��� �����մϴ�.
		auto instance = metaType.construct();
		// Ư�� �Լ��� ã�� ȣ���մϴ�.
		auto myFunctionMeta = metaType.func("AddComponent"_hs);
		if (myFunctionMeta)
		{
			entt::meta_any result = myFunctionMeta.invoke(instance, this);
			if (auto componentPtr = result.try_cast<Component*>())
			{
				return *componentPtr;
			}
			else
			{
				VP_ASSERT(false, " �Լ� ���� ����!");

			}
		}
		else
			VP_ASSERT(false, "FunctionMeta �Լ� ����!");
	}
	else
	{
		VP_ASSERT(false, "resolve �Լ� ����!");
	}


	return nullptr;
}
