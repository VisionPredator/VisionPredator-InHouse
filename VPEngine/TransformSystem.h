#pragma once
#include "System.h"
#include "EventSubscriber.h"
#include <any>

struct TransformComponent;

class TransformSystem : public System, public IUpdatable,public ILateUpdatable, public EventSubscriber/*,public IStartable*/,public IRenderable, public IStartable,public ICompAddable
{
public:
    TransformSystem(std::shared_ptr<SceneManager> entityManager);
    ~TransformSystem() override = default;
    void OnAddedComponent(std::any);
    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;
    void OnUpdate(std::any);
    void UpdateAllEntitys();
    void newUpdate();
    void OnSetParentAndChild(std::any data);
    void OnUpdateTransfomData(std::any data);
    void OnRelaseParentAndChild(std::any data);
	static void AddUpdateData(TransformComponent* newComponent)
	{
		// Check if the component is already in the vector
		auto it = std::find(newupdatevector.begin(), newupdatevector.end(), newComponent);

		// If the component is not found, add it to the vector
		if (it == newupdatevector.end())
		{
			newupdatevector.push_back(newComponent);
		}
	}
    static void RemoveUpdateData(TransformComponent* componentToRemove)
    {
        // newupdatevector에서 componentToRemove를 제거
        auto it = std::find(newupdatevector.begin(), newupdatevector.end(), componentToRemove);
        if (it != newupdatevector.end())
        {
            newupdatevector.erase(it);
        }
    }
private:
    void UpdateDirVector(TransformComponent* transform);
    static std::vector<TransformComponent*> newupdatevector;
    void CalculateTransform_Child(TransformComponent* transform, bool IsParentWorldChanged);
    void CalculateTransform_Parent(TransformComponent* transform);
    bool IsLocalTransformChanged(TransformComponent* transform);
    bool IsWorldTransformChanged(TransformComponent* transform);
    void UpdateWorldTransform(TransformComponent* transform, const VPMath::Matrix& worldTransform);
    void CalculateTransformDynamic(TransformComponent* transform);
    void CalculateTransformWorld(TransformComponent* transform);
    void CalculateTransformLocal(TransformComponent* transform);
    void WrapAngle(VPMath::Vector3& rotation);
    void UpdatePreviousWorldTransform(TransformComponent* transform);
    void UpdatePreviousLocalTransform(TransformComponent* transform);


    //// IStartable을(를) 통해 상속됨
    //void Initialize() override;

    //void Start(uint32_t gameObjectId) override;

    //void Finish(uint32_t gameObjectId) override;

    //void Finalize() override;


    // IRenderable을(를) 통해 상속됨
    void BeginRenderUpdate(float deltaTime) override;
    // IRenderable을(를) 통해 상속됨
    void RenderUpdate(float deltaTime) override;

    void LateRenderUpdate(float deltaTime) override;

    void EditorRenderUpdate(float deltaTime) override;

    // IStartable을(를) 통해 상속됨
    void Initialize() override;

    void Start(uint32_t gameObjectId) override;

    void Finish(uint32_t gameObjectId) override;

    void Finalize() override;








    // ILateUpdatable을(를) 통해 상속됨
    void LateUpdate(float deltaTime) override;


    // ICompAddable을(를) 통해 상속됨
    void ComponentAdded(Component* comp) override;

    void ComponentReleased(Component* comp) override;

};
