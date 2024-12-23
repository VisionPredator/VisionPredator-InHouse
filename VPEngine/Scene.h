#pragma once
#include "../PhysxEngine/VPPhysicsStructs.h"
#include "NavMeshData.h"
#include "NavStructs.h"
//class NavMeshData;
class Entity;
struct Component;
class Scene
{
public:
	Scene() {};
	~Scene() { Clear(); };
	void Clear();

	std::string SceneName{};
	std::unordered_map<uint32_t, std::shared_ptr<Entity>> EntityMap;
	std::unordered_map<entt::id_type, std::vector<std::weak_ptr<Component>>> m_ComponentPool;
	VPPhysics::PhysicsInfo ScenePhysicInfo;
	BuildSettings NavBuildSetting;
	std::shared_ptr<NavMeshData> SceneNavData;
};
