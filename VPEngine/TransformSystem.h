#pragma once
#include "System.h"
#include "EventSubscriber.h"
#include <any>

struct TransformComponent;

class TransformSystem : public System, public IUpdatable, public EventSubscriber
{
public:
    TransformSystem(std::shared_ptr<SceneManager> entityManager);
    ~TransformSystem() override = default;

    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;
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

private:
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

};
