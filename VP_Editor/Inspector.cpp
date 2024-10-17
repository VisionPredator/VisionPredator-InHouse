#include "pch.h"
#include "Inspector.h"
#include "SceneManager.h"
#include "HierarchySystem.h"
#include "imgui_stdlib.h"
#include "Components.h"
#include "../PhysxEngine/VPPhysicsStructs.h"
Inspector::Inspector(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<HierarchySystem> hierarchySystem) :m_SceneManager{ sceneManager }, m_HierarchySystem{ hierarchySystem }
{
}

void Inspector::ImGuiRender()
{
	ImGui::Begin("Inspector");
	{
		const int entityID = m_HierarchySystem.lock()->m_SelectedEntityID;
		if (entityID != 0 && m_SceneManager.lock()->HasEntity(entityID))
			EntityImGui(entityID);
	}
	ImGui::End();
}

void Inspector::EntityImGui(uint32_t entityID)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDragThreshold = 10.0f;
	ImGui::PushID(static_cast<int>(entityID));

	auto sceneManager = m_SceneManager.lock();
	if (!sceneManager) return;

	auto& objectName = sceneManager->GetComponent<IDComponent>(entityID)->Name;
	ImGui::InputText("Name", &objectName);
	ImGui::PopID();
	ImGui::Separator();

	ImGui::Text("Serialize"); ImGui::SameLine();
	if (ImGui::Button("Save"))
	{
		sceneManager->SerializePrefab(entityID);
	}
	ImGui::Separator();

	TransformComponentImGui(sceneManager->GetComponent(entityID, Reflection::GetTypeID<TransformComponent>()));
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
		ImGui::SetNextItemWidth(150); // Set the width to 150 pixels
		ImGui::InputText("##AddComponent", &m_searchComponent);
		for (auto [id1, MetaType] : entt::resolve())
		{
			const entt::id_type MetaTypeID = MetaType.id();
			for (auto [id2, baseType] : MetaType.base())
			{
				if (ShouldSkipComponent(MetaTypeID) || Reflection::GetTypeID<Component>() != baseType.id() ||
					sceneManager->HasComponent(entityID, MetaTypeID))
					continue;

				auto ClassName = Reflection::GetName_Class(MetaType);
				 ClassName = RemoveComponentSuffix(ClassName);

				auto toLower = [](const std::string& str) -> std::string {
					std::string lowerStr = str;
					std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
					return lowerStr;
					};

				std::string lowerSearchComponent = toLower(m_searchComponent);
				std::string lowerClassName = toLower(ClassName);

				if (lowerClassName.find(lowerSearchComponent) == std::string::npos)
					continue;

				ImGui::PushID(ClassName.c_str());
				ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f)); // Align text to the left
				ImGui::AlignTextToFramePadding();

				if (ImGui::Button(ClassName.c_str(), ImVec2(200, 35)))  // Display without suffix
					sceneManager->GetEntity(entityID)->AddComponent(MetaTypeID);
				ImGui::PopStyleVar();
				ImGui::PopID();
			}
		}
		ImGui::EndPopup();
	}
	ImGui::Separator();

	for (auto [id1, MetaType] : entt::resolve())
	{
		const entt::id_type metaTypeID = MetaType.id();
		if (ShouldSkipComponent(metaTypeID)) continue;
		if (sceneManager->HasComponent(entityID, metaTypeID))
		{
			ComponentImGui(sceneManager->GetComponent(entityID, metaTypeID));
		}
	}
}
void Inspector::TransformComponentImGui(Component* component)
{
	entt::id_type compID = component->GetHandle()->type().id();
	entt::meta_handle compMetaHandle = component->GetHandle();
	std::string componentName = Reflection::GetName_Class(component->GetHandle()->type());
	ImGui::PushID(componentName.c_str());

	ImGui::Text("TransformComponent");
	TransformComponent* comp = static_cast<TransformComponent*> (component);
	float temp_Local_Location[3] = { comp->Local_Location.x,comp->Local_Location.y,comp->Local_Location.z };
	float temp_Local_Rotation[3] = { comp->Local_Rotation.x,comp->Local_Rotation.y,comp->Local_Rotation.z };
	float temp_Local_Quaternion[3] = {};
	float temp_Local_Scale[3] = { comp->Local_Scale.x,comp->Local_Scale.y,comp->Local_Scale.z };
	float temp_World_Location[3] = { comp->World_Location.x,	comp->World_Location.y,		comp->World_Location.z };
	float temp_World_Rotation[3] = { comp->World_Rotation.x,	comp->World_Rotation.y,		comp->World_Rotation.z };
	float temp_World_Quaternion[3] = { comp->World_Quaternion.x,	comp->World_Quaternion.y,	comp->World_Quaternion.z };
	float temp_World_Scale[3] = { comp->World_Scale.x,		comp->World_Scale.y,		comp->World_Scale.z };
	float temp_FrontVector[3] = { comp->FrontVector.x,		comp->FrontVector.y,		comp->FrontVector.z };
	float temp_RightVector[3] = { comp->RightVector.x,		comp->RightVector.y,		comp->RightVector.z };


	{
		ImGui::PushID("Local_Location");
		ImGui::SetNextItemWidth(m_TypeBoxsize);
		if (ImGui::DragFloat3("Local_Location", temp_Local_Location, .01f, -FLT_MAX, FLT_MAX))
		{
			VPMath::Vector3 tempVector{};
			tempVector.x = temp_Local_Location[0];
			tempVector.y = temp_Local_Location[1];
			tempVector.z = temp_Local_Location[2];
			comp->SetLocalLocation(tempVector);
		}
		ImGui::PopID();
	}

	{
		ImGui::PushID("Local_Rotation");
		ImGui::SetNextItemWidth(m_TypeBoxsize);
		if (ImGui::DragFloat3("Local_Rotation", temp_Local_Rotation, .01f, -FLT_MAX, FLT_MAX))
		{
			VPMath::Vector3 tempVector{};
			tempVector.x = temp_Local_Rotation[0];
			tempVector.y = temp_Local_Rotation[1];
			tempVector.z = temp_Local_Rotation[2];
			comp->SetLocalRotation(tempVector);
		}
		ImGui::PopID();
	}

	{
		VPMath::Vector3 euler = comp->Local_Quaternion.ToEuler();
		VPMath::Quaternion tempVector{};
		temp_Local_Quaternion[0] = VPMath::XMConvertToDegrees(euler.x);
		temp_Local_Quaternion[1] = VPMath::XMConvertToDegrees(euler.y);
		temp_Local_Quaternion[2] = VPMath::XMConvertToDegrees(euler.z);
		ImGui::PushID("Local_Quaternion");
		ImGui::SetNextItemWidth(m_TypeBoxsize);
		if (ImGui::DragFloat3("Local_Quaternion", temp_Local_Quaternion, 1.f, -FLT_MAX, FLT_MAX))
		{
			euler.x = DirectX::XMConvertToRadians(temp_Local_Quaternion[0]);
			euler.y = DirectX::XMConvertToRadians(temp_Local_Quaternion[1]);
			euler.z = DirectX::XMConvertToRadians(temp_Local_Quaternion[2]);
			tempVector = VPMath::Quaternion::CreateFromYawPitchRoll(euler);
			comp->SetLocalQuaternion(tempVector);
		}
		ImGui::PopID();
	}

	{
		ImGui::PushID("Local_Scale");
		ImGui::SetNextItemWidth(m_TypeBoxsize);
		if (ImGui::DragFloat3("Local_Scale", temp_Local_Scale, .01f, -FLT_MAX, FLT_MAX))
		{
			VPMath::Vector3 tempVector{};
			tempVector.x = temp_Local_Scale[0];
			tempVector.y = temp_Local_Scale[1];
			tempVector.z = temp_Local_Scale[2];
			comp->SetLocalScale(tempVector);

		}
		ImGui::PopID();
	}

	{
		ImGui::PushID("World_Location");
		ImGui::SetNextItemWidth(m_TypeBoxsize);
		if (ImGui::DragFloat3("World_Location", temp_World_Location, .01f, -FLT_MAX, FLT_MAX))
		{
			VPMath::Vector3 tempVector{};
			tempVector.x = temp_World_Location[0];
			tempVector.y = temp_World_Location[1];
			tempVector.z = temp_World_Location[2];
			comp->SetWorldLocation(tempVector);
		}
		ImGui::PopID();
	}

	{
		ImGui::PushID("World_Rotation");
		ImGui::SetNextItemWidth(m_TypeBoxsize);
		if (ImGui::DragFloat3("World_Rotation", temp_World_Rotation, .01f, -FLT_MAX, FLT_MAX))
		{
			VPMath::Vector3 tempVector{};
			tempVector.x = temp_World_Rotation[0];
			tempVector.y = temp_World_Rotation[1];
			tempVector.z = temp_World_Rotation[2];
			comp->SetWorldRotation(tempVector);
		}
		ImGui::PopID();
	}

	{
		VPMath::Vector3 euler = comp->World_Quaternion.ToEuler();
		VPMath::Quaternion tempVector{};
		temp_World_Quaternion[0] = VPMath::XMConvertToDegrees(euler.x);
		temp_World_Quaternion[1] = VPMath::XMConvertToDegrees(euler.y);
		temp_World_Quaternion[2] = VPMath::XMConvertToDegrees(euler.z);
		ImGui::PushID("World_Quaternion");
		ImGui::SetNextItemWidth(m_TypeBoxsize);
		if (ImGui::DragFloat3("World_Quaternion", temp_Local_Quaternion, 1.f, -FLT_MAX, FLT_MAX))
		{
			euler.x = DirectX::XMConvertToRadians(temp_World_Quaternion[0]);
			euler.y = DirectX::XMConvertToRadians(temp_World_Quaternion[1]);
			euler.z = DirectX::XMConvertToRadians(temp_World_Quaternion[2]);
			tempVector = VPMath::Quaternion::CreateFromYawPitchRoll(euler);
			comp->SetWorldQuaternion(tempVector);
		}
		ImGui::PopID();
	}




	{
		ImGui::PushID("World_Scale");
		ImGui::SetNextItemWidth(m_TypeBoxsize);
		if (ImGui::DragFloat3("World_Scale", temp_World_Scale, .01f, -FLT_MAX, FLT_MAX))
		{
			VPMath::Vector3 tempVector{};
			tempVector.x = temp_World_Scale[0];
			tempVector.y = temp_World_Scale[1];
			tempVector.z = temp_World_Scale[2];
			comp->SetWorldScale(tempVector);
		}
		ImGui::PopID();
	}

	{
		ImGui::PushID("Front_Vector");
		ImGui::SetNextItemWidth(m_TypeBoxsize);

		ImGui::BeginDisabled();
		ImGui::DragFloat3("Front_Vector", temp_FrontVector, .01f, -FLT_MAX, FLT_MAX);
		ImGui::EndDisabled();

		ImGui::PopID();
	}
	{
		ImGui::PushID("RightVector");
		ImGui::SetNextItemWidth(m_TypeBoxsize);

		ImGui::BeginDisabled();
		ImGui::DragFloat3("Right_Vector", temp_RightVector, .01f, -FLT_MAX, FLT_MAX);
		ImGui::EndDisabled();

		ImGui::PopID();
	}
	ImGui::PopID();



}

void Inspector::ComponentImGui(Component* component)
{
	entt::id_type compID = component->GetHandle()->type().id();
	if (ShouldSkipComponent(compID)) return;

	std::string componentName = Reflection::GetName_Class(component->GetHandle()->type());
	ImGui::PushID(componentName.c_str());

	if (ImGui::TreeNode(componentName.c_str()))
	{
		for (auto [MemberID, memberMetaData] : component->GetHandle()->type().data())
			MemberImGui(memberMetaData, component);
		ImGui::TreePop();
	}

	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("Component_Option");
		IsClicked = true;
		m_ClickedCompID = compID;
	}

	if (IsClicked && ImGui::BeginPopupContextWindow("Component_Option"))
	{
		if (ImGui::MenuItem("Delete") && component->GetEntity()->HasComponent(m_ClickedCompID))
		{
			component->GetEntity()->RemoveComponent(m_ClickedCompID);
			IsClicked = false;
		}
		ImGui::EndPopup();
	}
	ImGui::PopID();

}

std::string Inspector::RemoveComponentSuffix(const std::string& className)
{
	const std::string suffix = "Component";
	if (className.size() >= suffix.size() &&
		className.compare(className.size() - suffix.size(), suffix.size(), suffix) == 0)
	{
		return className.substr(0, className.size() - suffix.size());
	}
	return className;
}

void Inspector::MemberImGui(entt::meta_data memberMetaData, Component* component)
{


	auto metaType = memberMetaData.type();
	ImGui::SetNextItemWidth(m_TypeBoxsize);
	if (metaType.is_enum())
		TypeImGui_EnumClass(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<VPMath::Vector2>())
		TypeImGui_Vector2(memberMetaData, component);
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
	else if (metaType.id() == Reflection::GetTypeID<std::wstring>())
		TypeImGui_wstring(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<bool>())
		TypeImGui_bool(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<int>())
		TypeImGui_int(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<uint32_t>())
		TypeImGui_unsigned_int(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<float>())
		TypeImGui_float(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<std::vector<std::string>>())
		TypeImGui_vector_string(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<std::vector<std::wstring>>())
		TypeImGui_vector_wstring(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<std::vector<std::pair<std::wstring, float>>>()) 
		TypeImGui_vector_pair_wstring_float(memberMetaData, component);
		else if (metaType.id() == Reflection::GetTypeID<std::vector<std::tuple<std::wstring, float,float>>>()) 
		TypeImGui_vector_tuple_wstring_float_float(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<VPPhysics::ColliderInfo>())
		TypeImGui_ColliderInfo(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<VPPhysics::BoxColliderInfo>())
		TypeImGui_BoxColliderInfo(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<VPPhysics::CapsuleColliderInfo>())
		TypeImGui_CapsuleColliderInfo(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<VPPhysics::SphereColliderInfo>())
		TypeImGui_SphereColliderInfo(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<VPPhysics::ControllerInfo>())
		TypeImGui_ControllerInfo(memberMetaData, component);
	else if (metaType.id() == Reflection::GetTypeID<VPPhysics::CapsuleControllerInfo>())
		TypeImGui_CapsuleControllerInfo(memberMetaData, component);
}
void Inspector::TypeImGui_Vector2(entt::meta_data memberMetaData, Component* component)
{
	VPMath::Vector2 tempVector = memberMetaData.get(component->GetHandle()).cast<VPMath::Vector2>();
	auto memberName = Reflection::GetName(memberMetaData);
	float tempFloat[2]{ tempVector.x,tempVector.y };
	ImGui::PushID(memberName.c_str());
	if (ImGui::DragFloat2(memberName.c_str(), tempFloat, 0.001f, -FLT_MAX, FLT_MAX))
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
	if (ImGui::DragFloat4(memberName.c_str(), tempFloat, 1.f, -FLT_MAX, FLT_MAX))
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
void Inspector::TypeImGui_wstring(entt::meta_data memberMetaData, Component* component)
{
	std::wstring tempWName = memberMetaData.get(component->GetHandle()).cast<std::wstring>();
	std::string memberName = Reflection::GetName(memberMetaData);

	// Convert wstring to string
	std::string tempName(tempWName.begin(), tempWName.end());

	ImGui::PushID(memberName.c_str());

	// InputText handling for std::wstring
	if (ImGui::InputText(memberName.c_str(), &tempName))
	{
		// Convert string back to wstring
		std::wstring newWName(tempName.begin(), tempName.end());
		memberMetaData.set(component->GetHandle(), newWName);
	}

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
	if (INPUTKEY(KEYBOARDKEY::LSHIFT))
	{
		if (ImGui::DragScalar(memberName.c_str(), ImGuiDataType_Float, &tempfloat, 0.01f))
			memberMetaData.set(component->GetHandle(), tempfloat);
	}
	else if (ImGui::DragScalar(memberName.c_str(), ImGuiDataType_Float, &tempfloat, 0.001f))
		memberMetaData.set(component->GetHandle(), tempfloat);

	ImGui::PopID();
}


void Inspector::TypeImGui_EnumClass(entt::meta_data memberMetaData, Component* component)
{
	// Static cache for enum members to avoid recomputation
	std::map<int, entt::meta_data> enumMap;
	// eunmMember string table 생성

	if (enumMap.empty()) {
		for (auto [id, metaData] : memberMetaData.type().data()) {
			entt::meta_any any = metaData.get({});
			if (any.allow_cast<int>()) {
				int memberInt = any.cast<int>();
				enumMap[memberInt] = metaData;
			}
		}
		assert(!enumMap.empty());
	}

	// 현재 enum 값 int로 가져오기
	auto currentEnum = memberMetaData.get(component->GetHandle());
	int currentEnumInt = 0;

	if (currentEnum.allow_cast<int>())
		currentEnumInt = currentEnum.cast<int>();

	std::string memberName = Reflection::GetName(memberMetaData);
	auto iter = enumMap.find(currentEnumInt);
	assert(iter != enumMap.end());
	std::string currentEnumName = Reflection::GetName(iter->second);

	// Combo 창
	if (ImGui::BeginCombo(memberName.c_str(), currentEnumName.c_str()))
	{
		for (const auto& [val, metaData] : enumMap)
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

void Inspector::TypeImGui_vector_string(entt::meta_data memberMetaData, Component* component)
{
	// Retrieve the vector<std::string> from component's handle
	auto stringVector = memberMetaData.get(component->GetHandle()).cast<std::vector<std::string>>();

	std::string memberName = Reflection::GetName(memberMetaData);
	ImGui::PushID(memberName.c_str());
	ImGui::Text(memberName.c_str());
	// Display each string in the vector
	for (size_t i = 0; i < stringVector.size(); ++i) 
	{
		ImGui::SetNextItemWidth(m_TypeBoxsize);

		ImGui::InputText(("Element " + std::to_string(i)).c_str(), &stringVector[i]);
	}

	ImGui::SetNextItemWidth(m_TypeBoxsize/2);

	// Option to add a new element
	if (ImGui::Button("  Add  ")) 
	{
		stringVector.push_back(""); // Add an empty string by default
	}

	// Option to remove the last element
	if (!stringVector.empty() ) 
	{
		ImGui::SameLine();
		ImGui::SetNextItemWidth(m_TypeBoxsize / 2);

		if (ImGui::Button("Remove"))
		{
		stringVector.pop_back();
		}
	}

	// Apply changes if vector was modified
	memberMetaData.set(component->GetHandle(), std::move(stringVector));

	ImGui::PopID();
}


void Inspector::TypeImGui_vector_wstring(entt::meta_data memberMetaData, Component* component)
{
	// Retrieve the vector<std::wstring> from component's handle
	auto wstringVector = memberMetaData.get(component->GetHandle()).cast<std::vector<std::wstring>>();
	std::string memberName = Reflection::GetName(memberMetaData);
	ImGui::PushID(memberName.c_str());
	ImGui::Text(memberName.c_str());
	// Display each string in the vector
	for (size_t i = 0; i < wstringVector.size(); ++i) 
	{
		std::wstring tempWMember = wstringVector[i];
		std::string tempSMember(tempWMember.begin(), tempWMember.end());

		ImGui::SetNextItemWidth(m_TypeBoxsize);
		// ImGui::InputText expects a wchar_t array for input
		if (ImGui::InputText(("Element " + std::to_string(i)).c_str(), &tempSMember)) 
		{
			// Convert string back to wstring
			std::wstring newWName(tempSMember.begin(), tempSMember.end());
			wstringVector[i] = newWName; // Convert back to std::wstring
		}
	}
	ImGui::SetNextItemWidth(m_TypeBoxsize / 2);
	// Option to add a new element
	if (ImGui::Button("  Add  "))
	{
		wstringVector.push_back(L""); // Add an empty wstring by default
	}

	// Option to remove the last element
	if (!wstringVector.empty() ) 
	{
		ImGui::SameLine();
		ImGui::SetNextItemWidth(m_TypeBoxsize / 2);
		if (ImGui::Button("Remove"))
		{
		wstringVector.pop_back();
		}
	}

	// Apply changes if vector was modified
	memberMetaData.set(component->GetHandle(), std::move(wstringVector));

	ImGui::PopID();
}
void Inspector::TypeImGui_vector_pair_wstring_float(entt::meta_data memberMetaData, Component* component) {
	// component의 핸들에서 vector<std::pair<std::wstring, float>>를 가져옴
	auto pairVector = memberMetaData.get(component->GetHandle()).cast<std::vector<std::pair<std::wstring, float>>>();
	std::string memberName = Reflection::GetName(memberMetaData);

	ImGui::PushID(memberName.c_str());

	// Collapsing Header를 사용하여 UI를 토글할 수 있도록 함 (기본으로 열림 상태)
	if (ImGui::CollapsingHeader((memberName + " Vector Pairs").c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		// 새로운 요소 추가 옵션 (기본적으로 빈 wstring과 0.0 float를 가진 pair 추가)
		ImGui::SetNextItemWidth(m_TypeBoxsize / 2);
		if (ImGui::Button("  Add  ")) {
			pairVector.push_back(std::make_pair(L"", 0.0f));
		}

		// 마지막 요소 제거 옵션
		if (!pairVector.empty()) {
			ImGui::SameLine(); // "Add" 버튼과 같은 줄에 표시
			ImGui::SetNextItemWidth(m_TypeBoxsize / 2);
			if (ImGui::Button("Remove")) {
				pairVector.pop_back();
			}
		}

		// 벡터의 각 pair 요소를 인덱스와 함께 표시
		for (size_t i = 0; i < pairVector.size(); ++i) {
			std::pair<std::wstring, float>& tempPair = pairVector[i];

			// std::wstring을 ImGui에서 사용하기 위해 std::string으로 변환
			std::string tempSFirst(tempPair.first.begin(), tempPair.first.end());
			float& tempSecond = tempPair.second;

			// 인덱스 생성 (예: "01", "02" 등)
			std::string index = (i < 9 ? "0" : "") + std::to_string(i + 1); // 10보다 작은 수에 0을 추가함

			// 인덱스, 문자열 입력, float 입력을 같은 줄에 표시
			ImGui::Text((index + ":").c_str());
			ImGui::SameLine(); // 같은 줄에 유지
			ImGui::SetNextItemWidth(m_TypeBoxsize);
			if (ImGui::InputText(("##String Element " + index).c_str(), &tempSFirst)) {
				// std::string을 다시 std::wstring으로 변환
				std::wstring newWString(tempSFirst.begin(), tempSFirst.end());
				tempPair.first = newWString; // 변환된 wstring으로 값 설정
			}

			ImGui::SameLine(); // float 입력을 같은 줄에 유지
			ImGui::SetNextItemWidth(m_TypeBoxsize / 2);
			ImGui::DragFloat(("##Float Element " + index).c_str(), &tempSecond, 0.1f, -100.0f, 100.0f); // float 값을 조정할 수 있는 슬라이더
		}
	}

	// 벡터가 수정된 경우 새로운 값을 적용
	memberMetaData.set(component->GetHandle(), std::move(pairVector));

	ImGui::PopID();
}
void Inspector::TypeImGui_vector_tuple_wstring_float_float(entt::meta_data memberMetaData, Component* component) {
	// component의 핸들에서 vector<std::tuple<std::wstring, float, float>>를 가져옴
	auto tupleVector = memberMetaData.get(component->GetHandle()).cast<std::vector<std::tuple<std::wstring, float, float>>>();
	std::string memberName = Reflection::GetName(memberMetaData);

	ImGui::PushID(memberName.c_str());

	// Collapsing Header를 사용하여 UI를 토글할 수 있도록 함 (기본으로 열림 상태)
	if (ImGui::CollapsingHeader((memberName + " Vector Tuples").c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		// 새로운 요소 추가 옵션 (기본적으로 빈 wstring과 0.0 float를 가진 tuple 추가)
		ImGui::SetNextItemWidth(m_TypeBoxsize / 2);
		if (ImGui::Button("  Add  ")) {
			tupleVector.push_back(std::make_tuple(L"", 0.00000000f, 0.000000f));
		}

		// 마지막 요소 제거 옵션
		if (!tupleVector.empty()) {
			ImGui::SameLine(); // "Add" 버튼과 같은 줄에 표시
			ImGui::SetNextItemWidth(m_TypeBoxsize / 2);
			if (ImGui::Button("Remove")) {
				tupleVector.pop_back();
			}
		}

		// 벡터의 각 tuple 요소를 인덱스와 함께 표시
		for (size_t i = 0; i < tupleVector.size(); ++i) {
			std::tuple<std::wstring, float, float>& tempTuple = tupleVector[i];

			// std::wstring을 ImGui에서 사용하기 위해 std::string으로 변환
			std::string tempSFirst(std::get<0>(tempTuple).begin(), std::get<0>(tempTuple).end());
			float& tempSecond = std::get<1>(tempTuple);  // 첫 번째 float 값
			float& tempThird = std::get<2>(tempTuple);   // 두 번째 float 값

			// 인덱스 생성 (예: "00", "01" 등)
			std::string index = (i < 10 ? "0" : "") + std::to_string(i); 

			// 인덱스, 문자열 입력, 두 개의 float 입력을 같은 줄에 표시
			ImGui::Text((index + ":").c_str());
			ImGui::SameLine(); // 같은 줄에 유지
			ImGui::SetNextItemWidth(m_TypeBoxsize);
			if (ImGui::InputText(("##String Element " + index).c_str(), &tempSFirst)) {
				// std::string을 다시 std::wstring으로 변환
				std::wstring newWString(tempSFirst.begin(), tempSFirst.end());
				std::get<0>(tempTuple) = newWString; // 변환된 wstring으로 값 설정
			}

			ImGui::SameLine(); // 첫 번째 float 값을 같은 줄에 유지
			ImGui::SetNextItemWidth(m_TypeBoxsize / 3);
			ImGui::DragFloat(("##Float Element 1 " + index).c_str(), &tempSecond, 0.1f, -100.0f, 100.0f); // 첫 번째 float 값을 조정할 수 있는 슬라이더

			ImGui::SameLine(); // 두 번째 float 값을 같은 줄에 유지
			ImGui::SetNextItemWidth(m_TypeBoxsize / 3);
			ImGui::DragFloat(("##Float Element 2 " + index).c_str(), &tempThird, 0.1f, -100.0f, 100.0f); // 두 번째 float 값을 조정할 수 있는 슬라이더
		}
	}

	// 벡터가 수정된 경우 새로운 값을 적용
	memberMetaData.set(component->GetHandle(), std::move(tupleVector));

	ImGui::PopID();
}





using namespace VPPhysics;



void Inspector::TypeImGui_ColliderInfo(entt::meta_data memberMetaData, Component* component)
{
	ColliderInfo tempColliderInfo = memberMetaData.get(component->GetHandle()).cast<ColliderInfo>();
	std::string memberName = Reflection::GetName(memberMetaData);

	auto enumMap = Reflection::GetEnumMap<EPhysicsLayer>();


	// 현재 enum 값 int로 가져오기
	int currentEnumInt = (int)tempColliderInfo.PhysicsLayer;

	std::string valuename = "EPhysicsLayer";
	auto iter = enumMap.find(currentEnumInt);
	assert(iter != enumMap.end());
	std::string currentEnumName = Reflection::GetName(iter->second);

	ImGui::PushID(memberName.c_str());
	// Combo 창
	ImGui::Text("ColliderInfo");
	ImGui::SetNextItemWidth(m_TypeBoxsize);
	if (ImGui::BeginCombo(valuename.c_str(), currentEnumName.c_str()))
	{
		for (const auto& [val, metaData] : enumMap)
		{
			std::string memberName = Reflection::GetName(metaData);
			const bool bIsSelected = val == currentEnumInt;
			if (ImGui::Selectable(memberName.c_str(), bIsSelected))
			{
				tempColliderInfo.PhysicsLayer = (EPhysicsLayer)val;
			}
		}
		ImGui::EndCombo();
	}
	

	ImGui::Checkbox("Use Gravity?", &tempColliderInfo.UseGravity);
	ImGui::Text("Linear / Angle Lock");
	ImGui::Checkbox("X##LinearLock", &tempColliderInfo.LinearLock[0]);
	ImGui::SameLine();
	ImGui::Checkbox("Y##LinearLock", &tempColliderInfo.LinearLock[1]);
	ImGui::SameLine();
	ImGui::Checkbox("Z##LinearLock", &tempColliderInfo.LinearLock[2]);

	ImGui::Checkbox("X##AngleLock", &tempColliderInfo.AngleLock[0]);
	ImGui::SameLine();
	ImGui::Checkbox("Y##AngleLock", &tempColliderInfo.AngleLock[1]);
	ImGui::SameLine();
	ImGui::Checkbox("Z##AngleLock", &tempColliderInfo.AngleLock[2]);

	float tempfriction[2]{ tempColliderInfo.StaticFriction ,tempColliderInfo.DynamicFriction };
	ImGui::SetNextItemWidth(m_TypeBoxsize);
	ImGui::DragFloat2("Static/Dynamic Friction", tempfriction, 0.1f);
	ImGui::SetNextItemWidth(m_TypeBoxsize);
	ImGui::DragFloat("Restitution", &tempColliderInfo.Restitution, 0.1f);
	ImGui::SetNextItemWidth(m_TypeBoxsize);
	ImGui::DragFloat("Density", &tempColliderInfo.Density, 0.1f);
	tempColliderInfo.StaticFriction = tempfriction[0];
	tempColliderInfo.DynamicFriction = tempfriction[1];
		memberMetaData.set(component->GetHandle(), tempColliderInfo);
	ImGui::PopID();
}
void Inspector::TypeImGui_BoxColliderInfo(entt::meta_data memberMetaData, Component* component)
{
	RigidBodyComponent* tempComp = static_cast<RigidBodyComponent*>(component);
	if (tempComp->ColliderShape != EColliderShape::BOX)
		return;
	BoxColliderInfo tempBoxColliderInfo = memberMetaData.get(component->GetHandle()).cast<BoxColliderInfo>();
	std::string memberName = Reflection::GetName(memberMetaData);

	ImGui::PushID(memberName.c_str());
	ImGui::Text("Box colliderInfo");
	ImGui::SetNextItemWidth(m_TypeBoxsize);
	ImGui::DragFloat3("Extent", &tempBoxColliderInfo.Extent.x, 0.1f,0.1f);
	if (tempBoxColliderInfo.Extent.x <= 0)
		tempBoxColliderInfo.Extent.x = 0.1f;
	if (tempBoxColliderInfo.Extent.y <= 0)
		tempBoxColliderInfo.Extent.y = 0.1f;
	if (tempBoxColliderInfo.Extent.z <= 0)
		tempBoxColliderInfo.Extent.z = 0.1f;

	memberMetaData.set(component->GetHandle(), tempBoxColliderInfo);
	ImGui::PopID();
}

void Inspector::TypeImGui_CapsuleColliderInfo(entt::meta_data memberMetaData, Component* component)
{
	RigidBodyComponent* tempComp = static_cast<RigidBodyComponent*>(component);
	if (tempComp->ColliderShape != EColliderShape::CAPSULE)
		return;
	CapsuleColliderInfo tempCapsuleColliderInfo = memberMetaData.get(component->GetHandle()).cast<CapsuleColliderInfo>();
	std::string memberName = Reflection::GetName(memberMetaData);

	ImGui::PushID(memberName.c_str());
	ImGui::Text("Capsule colliderInfo");

	ImGui::SetNextItemWidth(m_TypeBoxsize);
	ImGui::DragFloat("Radius", &tempCapsuleColliderInfo.Radius, 0.1f, 0.1f);
	ImGui::SetNextItemWidth(m_TypeBoxsize);
	ImGui::DragFloat("Half Height", &tempCapsuleColliderInfo.HalfHeight, 0.1f, 0.1f);
	if (tempCapsuleColliderInfo.Radius <= 0.f)
		tempCapsuleColliderInfo.Radius = 0.1f;
	if (tempCapsuleColliderInfo.HalfHeight <= 0.f)
		tempCapsuleColliderInfo.HalfHeight = 0.1f;

	memberMetaData.set(component->GetHandle(), tempCapsuleColliderInfo);

	ImGui::PopID();
}

void Inspector::TypeImGui_SphereColliderInfo(entt::meta_data memberMetaData, Component* component)
{
	RigidBodyComponent* tempComp = static_cast<RigidBodyComponent*>(component);
	if (tempComp->ColliderShape != EColliderShape::SPHERE)
		return;
	SphereColliderInfo tempSphereColliderInfo = memberMetaData.get(component->GetHandle()).cast<SphereColliderInfo>();
	std::string memberName = Reflection::GetName(memberMetaData);

	ImGui::PushID(memberName.c_str());
	ImGui::Text("Sphere colliderInfo");
	ImGui::SetNextItemWidth(m_TypeBoxsize);
	ImGui::DragFloat("Radius", &tempSphereColliderInfo.Radius,0.1f,0.1f);
	if (tempSphereColliderInfo.Radius <= 0.f)
		tempSphereColliderInfo.Radius = 0.1f;
	memberMetaData.set(component->GetHandle(), tempSphereColliderInfo);
	ImGui::PopID();
}

void Inspector::TypeImGui_ControllerInfo(entt::meta_data memberMetaData, Component* component)
{
	ControllerInfo tempControllerInfo = memberMetaData.get(component->GetHandle()).cast<ControllerInfo>();
	std::string memberName = Reflection::GetName(memberMetaData);

	ImGui::PushID(memberName.c_str());

	// Use Collapsing Header to toggle visibility
	if (ImGui::CollapsingHeader("Controller colliderInfo", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// Handling LayerNumber
		auto enumMap = Reflection::GetEnumMap<EPhysicsLayer>();

		int currentEnumInt = (int)tempControllerInfo.LayerNumber;
		std::string valuename = "EPhysicsLayer";
		auto iter = enumMap.find(currentEnumInt);
		assert(iter != enumMap.end());
		std::string currentEnumName = Reflection::GetName(iter->second);

		ImGui::SetNextItemWidth(m_TypeBoxsize);
		if (ImGui::BeginCombo(valuename.c_str(), currentEnumName.c_str()))
		{
			for (const auto& [val, metaData] : enumMap)
			{
				std::string memberName = Reflection::GetName(metaData);
				const bool bIsSelected = val == currentEnumInt;
				if (ImGui::Selectable(memberName.c_str(), bIsSelected))
				{
					tempControllerInfo.LayerNumber = (EPhysicsLayer)val;
				}
			}
			ImGui::EndCombo();
		}

		// Handling Pivot
		auto pivotEnumMap = Reflection::GetEnumMap<ControllerPivot>();

		int currentPivotEnumInt = (int)tempControllerInfo.Pivot;
		std::string pivotValueName = "ControllerPivot";
		auto pivotIter = pivotEnumMap.find(currentPivotEnumInt);
		assert(pivotIter != pivotEnumMap.end());
		std::string currentPivotEnumName = Reflection::GetName(pivotIter->second);

		ImGui::SetNextItemWidth(m_TypeBoxsize);
		if (ImGui::BeginCombo(pivotValueName.c_str(), currentPivotEnumName.c_str()))
		{
			for (const auto& [val, metaData] : pivotEnumMap)
			{
				std::string memberName = Reflection::GetName(metaData);
				const bool bIsSelected = val == currentPivotEnumInt;
				if (ImGui::Selectable(memberName.c_str(), bIsSelected))
				{
					tempControllerInfo.Pivot = (ControllerPivot)val;
				}
			}
			ImGui::EndCombo();
		}

		ImGui::SetNextItemWidth(m_TypeBoxsize);
		float LocalOffset[3] = { tempControllerInfo.LocalOffset.x, tempControllerInfo.LocalOffset.y, tempControllerInfo.LocalOffset.z };
		ImGui::DragFloat3("LocalOffset", LocalOffset, 0.1f, 0.1f);
		tempControllerInfo.LocalOffset = { LocalOffset[0], LocalOffset[1], LocalOffset[2] };
	}

	// Update the memberMetaData with the new values
	memberMetaData.set(component->GetHandle(), tempControllerInfo);

	ImGui::PopID();
}

void Inspector::TypeImGui_CapsuleControllerInfo(entt::meta_data memberMetaData, Component* component)
{
	CapsuleControllerInfo tempCapsuleControllerInfo = memberMetaData.get(component->GetHandle()).cast<CapsuleControllerInfo>();
	std::string memberName = Reflection::GetName(memberMetaData);

	ImGui::PushID(memberName.c_str());

	// Use Collapsing Header to toggle visibility
	if (ImGui::CollapsingHeader("Capsule Controller Info", ImGuiTreeNodeFlags_DefaultOpen))
	{


		ImGui::SetNextItemWidth(m_TypeBoxsize);
		ImGui::DragFloat("Height", &tempCapsuleControllerInfo.height, 0.1f, 0.1f);
		if (tempCapsuleControllerInfo.height <= 0.f)
			tempCapsuleControllerInfo.height = 0.1f;

		ImGui::SetNextItemWidth(m_TypeBoxsize);
		ImGui::DragFloat("Radius", &tempCapsuleControllerInfo.radius, 0.01f, 0.01f);
		if (tempCapsuleControllerInfo.radius <= 0.f)
			tempCapsuleControllerInfo.radius = 0.01f;

		ImGui::SetNextItemWidth(m_TypeBoxsize);
		ImGui::DragFloat("Step Offset", &tempCapsuleControllerInfo.stepOffset, 0.01f);

		ImGui::SetNextItemWidth(m_TypeBoxsize);
		ImGui::DragFloat("Slope Limit", &tempCapsuleControllerInfo.slopeLimit, 0.01f);

		ImGui::SetNextItemWidth(m_TypeBoxsize);
		ImGui::DragFloat("Contact Offset", &tempCapsuleControllerInfo.contactOffset, 0.001f);
	}

	// Update the memberMetaData with the new values
	memberMetaData.set(component->GetHandle(), tempCapsuleControllerInfo);

	ImGui::PopID();
}


bool Inspector::ShouldSkipComponent(entt::id_type metaTypeID)
{
	return metaTypeID == Reflection::GetTypeID<IDComponent>() ||
		metaTypeID == Reflection::GetTypeID<TransformComponent>() ||
		metaTypeID == Reflection::GetTypeID<Parent>() ||
		metaTypeID == Reflection::GetTypeID<Children>();
}