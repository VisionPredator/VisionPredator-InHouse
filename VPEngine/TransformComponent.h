#pragma once
#include "Component.h"
using namespace entt::literals;


/// <summary>
/// 나중에 prefab이 계층관계가있다면, 어찌하지?
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
		nlohmann_json_j["Local_Location"] = nlohmann_json_t.Local_Location; nlohmann_json_j["Local_Quaternion"] = nlohmann_json_t.Local_Quaternion; nlohmann_json_j["Local_Scale"] = nlohmann_json_t.Local_Scale;
	} friend void from_json(const nlohmann::json& nlohmann_json_j, TransformComponent& nlohmann_json_t) {
		nlohmann_json_j.at("Local_Location").get_to(nlohmann_json_t.Local_Location); nlohmann_json_j.at("Local_Quaternion").get_to(nlohmann_json_t.Local_Quaternion); nlohmann_json_j.at("Local_Scale").get_to(nlohmann_json_t.Local_Scale);
	} std::shared_ptr<Component> AddComponent(Entity* parentEntity) override {
		auto component = parentEntity->AddComponent<TransformComponent>(); return component;
	} void SerializeComponent(nlohmann::json& json) const override 
	{
		to_json(json, *this);
	} std::shared_ptr<Component> DeserializeComponent(const nlohmann::json& json, Entity* parentEntity) const override
	{
		auto component = parentEntity->AddComponent<TransformComponent>();
		*component = json.get<TransformComponent>();
		component->SetEntity(parentEntity); return component;
	} entt::meta_handle GetHandle() override {
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

	VPMath::Vector3 Previous_WorldLocation = {};
	VPMath::Vector3 Previous_WorldRotation = {};
	VPMath::Quaternion Previous_WorldQuaternion = {};
	VPMath::Vector3 Previous_WorldScale = { 1.f,1.f,1.f };
	VPMath::Matrix Previous_WorldTransform = {};

	VPMath::Vector3 World_Location = {};
	VPMath::Vector3 World_Rotation = {};
	VPMath::Quaternion World_Quaternion = {};
	VPMath::Vector3 World_Scale = { 1.f,1.f,1.f };
	VPMath::Vector3 FrontVector = {};
	VPMath::Vector3 UpVector = {};
	VPMath::Vector3 RightVector = {};
	VPMath::Matrix LocalTransform = {}; 
	VPMath::Matrix WorldTransform = {};
};
