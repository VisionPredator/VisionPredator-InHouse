#pragma once
#include "System.h"
namespace VisPred
{

class CameraSystem :
    public System, public IUpdatable
{






    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;

};
}

