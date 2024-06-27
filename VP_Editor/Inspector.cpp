#include "pch.h"
#include "Inspector.h"
#include "SceneManager.h"
#include "HierarchySystem.h"
#include "imgui_stdlib.h"
#include "Components.h"

Inspector::Inspector(SceneManager* sceneManager, HierarchySystem* hierarchySystem) :m_SceneManager{ sceneManager }, m_HierarchySystem{ hierarchySystem }
{
}

void Inspector::ImGuiRender()
{
	ImGui::Begin("Inspector");
	{
		const int entityID = m_HierarchySystem->m_SelectedEntityID;
		if (entityID != 0 && m_SceneManager->HasEntity(entityID))
			EntityImGui(entityID);
	}
	ImGui::End();
}

void Inspector::EntityImGui(uint32_t entityID)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.MouseDragThreshold = 10.0f;
	ImGui::PushID(static_cast<int>(entityID));
	auto& objectName = m_SceneManager->GetComponent<IDComponent>(entityID)->Name;
	ImGui::InputText("Name", &objectName);
	ImGui::PopID();
	ImGui::Separator();

	ImGui::Text("Serialize");	ImGui::SameLine();	
	if (ImGui::Button("Save"))
	{
		m_SceneManager->SerializePrefab(entityID);
	}
		ImGui::Text(" ");

	ImGui::Separator();

	if (ImGui::Button("  AddComponent  "))
		ImGui::OpenPopup("AddComponentPopup");

	ImGui::SetNextWindowSize({ 250.f, 330.f });
	if (ImGui::BeginPopup("AddComponentPopup"))
	{
		if (!m_searchComponent.empty())
		{
			if (ImGui::Button("x"))
				m_searchComponent.clear();
		}
		else
			ImGui::Button(">");

		ImGui::SameLine();

		ImGui::InputText("##AddComponent", &m_searchComponent);
		for (auto [id1, MetaType] : entt::resolve())
		{
			const entt::id_type MetaTypeID = MetaType.id();
			for (auto [id2, baseType] : MetaType.base())
			{
				if (MetaTypeID == Reflection::GetTypeID<IDComponent>()
					|| MetaTypeID == Reflection::GetTypeID<TransformComponent>()
					|| MetaTypeID == Reflection::GetTypeID<Parent>()
					|| MetaTypeID == Reflection::GetTypeID<Children>()
					|| Reflection::GetTypeID<Component>() != baseType.id())
					continue;
	
				auto ClassName = Reflection::GetName_Class(MetaType);
				// Convert a string to lowercase
				auto toLower = [](const std::string& str) -> std::string {
					std::string lowerStr = str;
					std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
					return lowerStr;
					};

				std::string lowerSearchComponent = toLower(m_searchComponent);
				auto lowerClassName = toLower(ClassName);

				// Check if ClassName contains m_searchComponent
				if (lowerClassName.find(lowerSearchComponent) == std::string::npos)
					continue;

				if (m_SceneManager->HasComponent(entityID, MetaTypeID))
					continue;

				ImGui::PushID(ClassName.c_str());

				if (ImGui::Button(ClassName.c_str(), ImVec2(235, 35)))
					m_SceneManager->GetEntity(entityID)->AddComponent(MetaTypeID);
				ImGui::PopID();
			}
		}

		ImGui::EndPopup();
	}

	ComponentImGui(m_SceneManager->GetComponent(entityID, Reflection::GetTypeID<TransformComponent>()));
	for (auto [id1, MetaType] : entt::resolve())
	{
		const entt::id_type metaTypeID = MetaType.id();
		for (auto [id2, baseType] : MetaType.base())
		{
			if (Reflection::GetTypeID<Component>() != baseType.id())
				continue;
			if (metaTypeID == Reflection::GetTypeID<IDComponent>() ||
				metaTypeID == Reflection::GetTypeID<TransformComponent>() ||
				metaTypeID == Reflection::GetTypeID<Parent>() ||
				metaTypeID == Reflection::GetTypeID<Children>())
				continue;
			if (m_SceneManager->HasComponent(entityID, MetaType.id()))
			{
				ComponentImGui(m_SceneManager->GetComponent(entityID, metaTypeID));
			}
		}
	}


	//auto Components = m_SceneManager->GetOwnedComponent(entityID);
	//for (Component* Comp : Components)
	//{
	//	entt::id_type CompID = Comp->GetHandle()->type().id();
	//	if (CompID == Reflection::GetTypeID<IDComponent>()
	//		|| CompID == Reflection::GetTypeID<TransformComponent>()
	//		|| CompID == Reflection::GetTypeID<Parent>()
	//		|| CompID == Reflection::GetTypeID<Children>())
	//		continue;
	//	ComponentImGui(Comp);
	//}


}

void Inspector::ComponentImGui(Component* component)
{
	//if (component->GetHandle()->type().id() == Reflection::GetTypeID<IDComponent>())
	//	return;
	entt::id_type compID = component->GetHandle()->type().id();
	entt::meta_handle compMetaHandle = component->GetHandle();
	std::string componentName = Reflection::GetName_Class(component->GetHandle()->type());
	ImGui::PushID(componentName.c_str());

	if (ImGui::TreeNode(componentName.c_str()))
	{
		for (auto [MemberID, memberMetaData] : compMetaHandle->type().data())
			MemberImGui(memberMetaData, component);
		ImGui::TreePop();
	}

	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("Component_Option");
		IsClicked = true;
		m_ClickedCompID = component->GetHandle()->type().id();

	}
	if (m_ClickedCompID != Reflection::GetTypeID<TransformComponent>())
	{
		if (IsClicked && ImGui::BeginPopupContextWindow("Component_Option"))
		{

			if (ImGui::MenuItem("Delete"))
			{
				if (component->GetEntity()->HasComponent(m_ClickedCompID))
				{
					component->GetEntity()->RemoveComponent(m_ClickedCompID);

				}
				//m_SceneManager->DeleteEntity(m_RClickedEntityID);
				//m_RClickedEntityID = 0;
				//m_IsEntityRClicked = false;
				IsClicked = false;
			}


			ImGui::EndPopup();
		}
	}

	ImGui::PopID();

}

void Inspector::MemberImGui(entt::meta_data memberMetaData, Component* component)
{


	auto metaType = memberMetaData.type();
	if (metaType.is_enum() )
		TypeImGui_EnumClass(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<VPMath::Vector3>())
		TypeImGui_Vector3(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<VPMath::Vector3>())
		TypeImGui_Vector3(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<VPMath::Vector4>())
		TypeImGui_Vector4(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<VPMath::Quaternion>())
		TypeImGui_Quaternion(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<VPMath::Color>())
		TypeImGui_Color(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<std::string>())
		TypeImGui_string(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<bool>())
		TypeImGui_bool(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<int>())
		TypeImGui_int(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<uint32_t>())
		TypeImGui_unsigned_int(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<float>())
		TypeImGui_float(memberMetaData, component);
}
void Inspector::TypeImGui_Vector2(entt::meta_data memberMetaData, Component* component)
{
	VPMath::Vector2 tempVector = memberMetaData.get(component->GetHandle()).cast<VPMath::Vector2>();
	auto memberName = Reflection::GetName(memberMetaData);
	float tempFloat[2]{ tempVector.x,tempVector.y };
	ImGui::PushID(memberName.c_str());
	if (ImGui::DragFloat2(memberName.c_str(), tempFloat, 1.f, -FLT_MAX, FLT_MAX))
	{
		tempVector.x = tempFloat[0];
		tempVector.y = tempFloat[1];
		memberMetaData.set(component->GetHandle(), tempVector);
	}
	ImGui::PopID();

}
void Inspector::TypeImGui_Vector3(entt::meta_data memberMetaData, Component* component)
{
	VPMath::Vector3 tempVector = memberMetaData.get(component->GetHandle()).cast<VPMath::Vector3>();
	auto memberName = Reflection::GetName(memberMetaData);
	float tempFloat[3]{ tempVector.x,tempVector.y,tempVector.z };
	ImGui::PushID(memberName.c_str());

	if (ImGui::DragFloat3(memberName.c_str(), tempFloat, .01f, -FLT_MAX, FLT_MAX))
	{
		tempVector.x = tempFloat[0];
		tempVector.y = tempFloat[1];
		tempVector.z = tempFloat[2];
		memberMetaData.set(component->GetHandle(), tempVector);
	}

	ImGui::PopID();

}
void Inspector::TypeImGui_Vector4(entt::meta_data memberMetaData, Component* component)
{
	VPMath::Vector4 tempVector = memberMetaData.get(component->GetHandle()).cast<VPMath::Vector4>();
	auto memberName = Reflection::GetName(memberMetaData);
	float tempFloat[4]{ tempVector.x,tempVector.y,tempVector.z,tempVector.w };
	ImGui::PushID(memberName.c_str());
	if (ImGui::DragFloat3(memberName.c_str(), tempFloat, 1.f, -FLT_MAX, FLT_MAX))
	{
		tempVector.x = tempFloat[0];
		tempVector.y = tempFloat[1];
		tempVector.z = tempFloat[2];
		tempVector.w = tempFloat[3];
		memberMetaData.set(component->GetHandle(), tempVector);
	}
	ImGui::PopID();
}
void Inspector::TypeImGui_Quaternion(entt::meta_data memberMetaData, Component* component)
{
	VPMath::Quaternion tempQuaternion = memberMetaData.get(component->GetHandle()).cast<VPMath::Quaternion>();
	auto memberName = Reflection::GetName(memberMetaData);
	VPMath::Vector3 euler = tempQuaternion.ToEuler();
	ImGui::PushID(memberName.c_str());
	float tempFloat[3] =
	{
		VPMath::XMConvertToDegrees(euler.x),
		VPMath::XMConvertToDegrees(euler.y),
		VPMath::XMConvertToDegrees(euler.z)
	};
	if (ImGui::DragFloat3(memberName.c_str(), tempFloat, 1.f, -FLT_MAX, FLT_MAX))
	{
		euler.x = DirectX::XMConvertToRadians(tempFloat[0]);
		euler.y = DirectX::XMConvertToRadians(tempFloat[1]);
		euler.z = DirectX::XMConvertToRadians(tempFloat[2]);
		tempQuaternion = VPMath::Quaternion::CreateFromYawPitchRoll(euler);
		memberMetaData.set(component->GetHandle(), tempQuaternion);
	}
	ImGui::PopID();
}
void Inspector::TypeImGui_Color(entt::meta_data memberMetaData, Component* component)
{
	using namespace VPMath;
	Color tempcolor = memberMetaData.get(component->GetHandle()).cast<Color>();
	std::string memberName = Reflection::GetName(memberMetaData);
	ImGui::PushID(memberName.c_str());
	float tempfloat[4] = { tempcolor.x, tempcolor.y ,tempcolor.z, tempcolor.w };
	if (ImGui::ColorEdit4(memberName.c_str(), tempfloat))
	{
		tempcolor.x = tempfloat[0];
		tempcolor.y = tempfloat[1];
		tempcolor.z = tempfloat[2];
		tempcolor.w = tempfloat[3];
		memberMetaData.set(component->GetHandle(), tempcolor);
	}
	ImGui::PopID();
}
void Inspector::TypeImGui_string(entt::meta_data memberMetaData, Component* component)
{
	std::string tempName = memberMetaData.get(component->GetHandle()).cast<std::string>();
	std::string memberName = Reflection::GetName(memberMetaData);
	ImGui::PushID(memberName.c_str());
	if (ImGui::InputText(memberName.c_str(), &tempName))
		memberMetaData.set(component->GetHandle(), tempName);
	ImGui::PopID();
}
void Inspector::TypeImGui_bool(entt::meta_data memberMetaData, Component* component)
{
	bool tempbool = memberMetaData.get(component->GetHandle()).cast<bool>();
	std::string memberName = Reflection::GetName(memberMetaData);
	ImGui::PushID(memberName.c_str());
	if (ImGui::Checkbox(memberName.c_str(), &tempbool))
		memberMetaData.set(component->GetHandle(), tempbool);
	ImGui::PopID();

}
void Inspector::TypeImGui_int(entt::meta_data memberMetaData, Component* component)
{
	int tempInt = memberMetaData.get(component->GetHandle()).cast<int>();
	std::string memberName = Reflection::GetName(memberMetaData);
	ImGui::PushID(memberName.c_str());
	if (ImGui::InputInt(memberName.c_str(), &tempInt))
		memberMetaData.set(component->GetHandle(), tempInt);
	ImGui::PopID();
}
void Inspector::TypeImGui_unsigned_int(entt::meta_data memberMetaData, Component* component)
{
	unsigned int tempUnsignedint = memberMetaData.get(component->GetHandle()).cast<uint32_t>();
	std::string memberName = Reflection::GetName(memberMetaData);
	ImGui::PushID(memberName.c_str());
	if (ImGui::DragScalar(memberName.c_str(), ImGuiDataType_U32, &tempUnsignedint))
		memberMetaData.set(component->GetHandle(), tempUnsignedint);
	ImGui::PopID();
}
void Inspector::TypeImGui_double(entt::meta_data memberMetaData, Component* component)
{
	double tempDouble = memberMetaData.get(component->GetHandle()).cast<double>();
	std::string memberName = Reflection::GetName(memberMetaData);
	ImGui::PushID(memberName.c_str());
	if (ImGui::DragScalar(memberName.c_str(),ImGuiDataType_Double, &tempDouble))
		memberMetaData.set(component->GetHandle(), tempDouble);
	ImGui::PopID();
}
void Inspector::TypeImGui_float(entt::meta_data memberMetaData, Component* component)
{
	float tempfloat = memberMetaData.get(component->GetHandle()).cast<float>();
	std::string memberName = Reflection::GetName(memberMetaData);
	ImGui::PushID(memberName.c_str());
	if (ImGui::DragScalar(memberName.c_str(), ImGuiDataType_Float, &tempfloat))
		memberMetaData.set(component->GetHandle(), tempfloat);
	ImGui::PopID();
}


void Inspector::TypeImGui_EnumClass(entt::meta_data memberMetaData, Component* component)
{
	// Static cache for enum members to avoid recomputation
	static std::map<int, entt::meta_data> enumCache;
	// eunmMember string table 생성

	if (enumCache.empty()) {
		for (auto [id, metaData] : memberMetaData.type().data()) {
			entt::meta_any any = metaData.get({});
			if (any.allow_cast<int>()) {
				int memberInt = any.cast<int>();
				enumCache[memberInt] = metaData;
			}
		}
		assert(!enumCache.empty());
	}

	// 현재 enum 값 int로 가져오기
	auto currentEnum = memberMetaData.get(component->GetHandle());
	int currentEnumInt = 0;

	if (currentEnum.allow_cast<int>())
		currentEnumInt = currentEnum.cast<int>();

	std::string memberName = Reflection::GetName(memberMetaData);
	auto iter = enumCache.find(currentEnumInt);
	assert(iter != enumCache.end());
	std::string currentEnumName = Reflection::GetName(iter->second);

	// Combo 창
	if (ImGui::BeginCombo(memberName.c_str(), currentEnumName.c_str()))
	{
		for (const auto& [val, metaData] : enumCache)
		{
			std::string memberName = Reflection::GetName(metaData);
			const bool bIsSelected = val == currentEnumInt;
			if (ImGui::Selectable(memberName.c_str(), bIsSelected))
			{
				memberMetaData.set(component->GetHandle(), val);
			}
		}
		ImGui::EndCombo();
	}
}
