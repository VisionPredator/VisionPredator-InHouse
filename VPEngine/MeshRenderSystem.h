#pragma once
#include "System.h"
#include "EventSubscriber.h"
class MeshRenderSystem :
    public System, public EventSubscriber,public IFixedUpdatable
{
    MeshRenderSystem(SceneManager* entityManager);
    ~MeshRenderSystem() = default;
    void OnInitializeSystem(std::any );
    void OnFinalizeSystem(std::any);
    void OnInitializeEntity(std::any entityID);
    void OnFinalizeEntity(std::any entityID);

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

    void Render(float deltaTime);

};

