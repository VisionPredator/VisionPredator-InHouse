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

    void OnSetParentAndChild(std::any data);
    void OnUpdateTransfomData(std::any data);
    void OnRelaseParentAndChild(std::any data);

private:
    void CalculateTransform_new(TransformComponent* transform, bool IsParentWorldChanged);
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
