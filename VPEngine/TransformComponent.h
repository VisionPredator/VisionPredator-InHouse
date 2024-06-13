#pragma once
#include "Component.h"
using namespace entt::literals;


/// <summary>
/// ���߿� prefab�� �������谡�ִٸ�, ��������?
/// </summary>


struct Parent : public Component
{
	VP_JSONBODY(Parent, ParentID)
		Parent();

	uint32_t ParentID = 0;


};

struct Children : public Component
{
	VP_JSONBODY(Children, ChildrenID)
		Children();

	std::list<uint32_t> ChildrenID;

};

struct TransformComponent :public Component
{
public:
	friend void to_json(nlohmann::json& nlohmann_json_j, const TransformComponent& nlohmann_json_t) {
		nlohmann_json_j["Local_Location"] = nlohmann_json_t.Local_Location; nlohmann_json_j["Local_Rotation"] = nlohmann_json_t.Local_Rotation; nlohmann_json_j["Local_Scale"] = nlohmann_json_t.Local_Scale;
	} friend void from_json(const nlohmann::json& nlohmann_json_j, TransformComponent& nlohmann_json_t) {
		nlohmann_json_j.at("Local_Location").get_to(nlohmann_json_t.Local_Location); nlohmann_json_j.at("Local_Rotation").get_to(nlohmann_json_t.Local_Rotation); nlohmann_json_j.at("Local_Scale").get_to(nlohmann_json_t.Local_Scale);
	}Component* AddComponent(Entity* parentEntity) override 
	{
		TransformComponent* component = parentEntity->AddComponent<TransformComponent>(); return component;
	}
	void SerializeComponent(nlohmann::json& json)const override 
	{
		to_json(json, *this);
	}Component* DeserializeComponent(const nlohmann::json json, Entity* parentEntity)
		const override 
	{
		auto component = parentEntity->AddComponent<TransformComponent>(); *component = json; component->OwnedEntity = parentEntity; return component;
	} 
	entt::meta_handle GetHandle() override
	{
		return *this;
	}

		TransformComponent();


	VPMath::Vector3 Local_Location = {};
	VPMath::Vector3 Local_Rotation = {};
	VPMath::Quaternion Local_Quaternion = {};
	VPMath::Vector3 Local_Scale = { 1.f,1.f,1.f };

	VPMath::Vector3 Previous_Location = {};
	VPMath::Vector3 Previous_Rotation = {};
	VPMath::Quaternion Previous_Quaternion = {};
	VPMath::Vector3 Previous_Scale = { 1.f,1.f,1.f };

	VPMath::Vector3 World_Location = {};
	VPMath::Quaternion World_Quaternion = {};
	VPMath::Vector3 World_Scale = { 1.f,1.f,1.f };
	VPMath::Vector3 FrontVector = {};
	VPMath::Vector3 UpVector = {};
	VPMath::Vector3 RightVector = {};
	VPMath::Matrix LocalTransform = {}; 
	VPMath::Matrix WorldTransform = {};
};
