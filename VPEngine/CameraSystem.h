#pragma once
#include "System.h"
class CameraSystem :
    public System, public IUpdatable
{






    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;

};

