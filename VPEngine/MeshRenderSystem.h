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

    ///��ġ�� Update ���Ŀ� Render�� �׷�������
    ///��� ����ϴ°�
    ///
    ///FixedUpdate 
    /// ������ ������ �����ְ�
    ///��ġ�� FixedUpdate ���Ŀ� Render�� �׷�������
    /// 
    /// 
    /// �������� ���� 1500 




    // IFixedUpdatable��(��) ���� ��ӵ�
    void FixedUpdate(float deltaTime) override;

    void Render(float deltaTime);

};

