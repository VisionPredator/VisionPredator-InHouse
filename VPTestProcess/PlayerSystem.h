#pragma once
#include <System.h>
class PlayerSystem :
    public System,public IUpdatable
{
    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;
};

