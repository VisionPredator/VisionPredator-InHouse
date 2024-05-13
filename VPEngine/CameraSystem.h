#pragma once
#include "System.h"
namespace VisPred
{

class CameraSystem :
    public System, public IUpdatable
{






    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;

};
}

