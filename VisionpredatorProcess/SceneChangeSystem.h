#pragma once
#include <System.h>
class SceneChangeSystem :
    public System,public IUpdatable
{
public :
    SceneChangeSystem(std::shared_ptr<SceneManager> scenemanager);

    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;
};

