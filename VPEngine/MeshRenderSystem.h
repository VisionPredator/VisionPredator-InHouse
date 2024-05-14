#pragma once
#include "System.h"
#include "EventSubscriber.h"
class MeshRenderSystem :
    public System,public IFixedUpdatable,public EventSubscriber
{
public:
    MeshRenderSystem(EntityManager* entityManager);
    ~MeshRenderSystem() = default;
    void OnInitializeSystem(std::any);
    void OnFinalizeSystem(std::any);

    ///어치피 Update 이후에 Render가 그러질꺼면
    ///계속 계산하는거
    ///
    ///FixedUpdate 
    /// 너한테 정보를 보내주고
    ///어치피 FixedUpdate 이후에 Render가 그러질꺼면
    /// 
    /// 
    /// 프레임이 만약 1500 




    // IFixedUpdatable을(를) 통해 상속됨
    void FixedUpdate(float deltaTime) override;

};

