#pragma once
#include <System.h>
class SceneChangeSystem :
    public System,public IUpdatable
{
public :
    SceneChangeSystem(std::shared_ptr<SceneManager> scenemanager);

    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;
};

