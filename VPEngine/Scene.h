#pragma once
class Entity;
struct Component;
class Scene
{
public:
	Scene() {};
	~Scene() { Clear(); };
	void Clear();

	std::string SceneName{};
	std::unordered_map<uint32_t, Entity*> EntityMap;
	std::unordered_map<entt::id_type, std::vector< Component*>> m_ComponentPool;
};
