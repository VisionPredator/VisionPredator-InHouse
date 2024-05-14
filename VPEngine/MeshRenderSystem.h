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

};

