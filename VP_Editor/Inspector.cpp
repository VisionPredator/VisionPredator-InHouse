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
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.MouseDragThreshold = 10.0f;
	ImGui::PushID(static_cast<int>(entityID));
	auto& objectName = m_SceneManager.lock()->GetComponent<IDComponent>(entityID)->Name;
	ImGui::InputText("Name", &objectName);
	ImGui::PopID();
	ImGui::Separator();

	ImGui::Text("Serialize");	ImGui::SameLine();	
	if (ImGui::Button("Save"))
	{
		m_SceneManager.lock()->SerializePrefab(entityID);
	}

	ImGui::Separator();
	TransformComponentImGui(m_SceneManager.lock()->GetComponent(entityID, Reflection::GetTypeID<TransformComponent>()));
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

				if (m_SceneManager.lock()->HasComponent(entityID, MetaTypeID))
					continue;

				ImGui::PushID(ClassName.c_str());

				if (ImGui::Button(ClassName.c_str(), ImVec2(235, 35)))
					m_SceneManager.lock()->GetEntity(entityID)->AddComponent(MetaTypeID);
				ImGui::PopID();
			}
		}

		ImGui::EndPopup();
	}
	ImGui::Separator();

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
			if (m_SceneManager.lock()->HasComponent(entityID, MetaType.id()))
			{
				ComponentImGui(m_SceneManager.lock()->GetComponent(entityID, metaTypeID));
			}
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
		for (auto [MemberID, memberMetaData] : compMetaHandle->type().data())
			MemberImGui(memberMetaData, component);
	ImGui::PopID();

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
	ImGui::SetNextItemWidth(m_TypeBoxsize);
	if (metaType.is_enum())
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
	if (ImGui::DragScalar(memberName.c_str(), ImGuiDataType_Float, &tempfloat))
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
	ImGui::Text("Controller colliderInfo");


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

	memberMetaData.set(component->GetHandle(), tempControllerInfo);
	ImGui::PopID();
}
void Inspector::TypeImGui_CapsuleControllerInfo(entt::meta_data memberMetaData, Component* component)
{
	CapsuleControllerInfo tempCapsuleControllerInfo = memberMetaData.get(component->GetHandle()).cast<CapsuleControllerInfo>();
	std::string memberName = Reflection::GetName(memberMetaData);

	ImGui::PushID(memberName.c_str());
	ImGui::Text("Capsule Controller colliderInfo");

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
	ImGui::Text("Controller colliderInfo");

	memberMetaData.set(component->GetHandle(), tempCapsuleControllerInfo);
	ImGui::PopID();
}
