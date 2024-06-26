#pragma once
#include "Component.h"

struct IDComponent : public Component
{
    friend void to_json(nlohmann::json& nlohmann_json_j, const IDComponent& nlohmann_json_t)
    {
        nlohmann_json_j["Name"] = nlohmann_json_t.Name;
    } friend void from_json(const nlohmann::json& nlohmann_json_j, IDComponent& nlohmann_json_t) {
        nlohmann_json_j.at("Name").get_to(nlohmann_json_t.Name);
    }Component* AddComponent(Entity* parentEntity) override {
        IDComponent* component = parentEntity->AddComponent<IDComponent>(); return component;
    }void SerializeComponent(nlohmann::json& json)const override {
        to_json(json, *this);
    }Component* DeserializeComponent(const nlohmann::json json, Entity* parentEntity) const override {
        auto component = parentEntity->AddComponent<IDComponent>();
        *component = json; component->OwnedEntity = parentEntity;
        return component;
    } entt::meta_handle GetHandle() override {
        return *this;
    }
		IDComponent();
	std::string Name = "Entity";
};
