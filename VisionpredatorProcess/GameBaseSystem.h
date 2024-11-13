#pragma once
#include <System.h>
class GameBaseSystem :
    public System,public IStartable,public IUpdatable
{
public:
    GameBaseSystem(std::shared_ptr<SceneManager> sceneManager);


    // IStartable을(를) 통해 상속됨
    void Initialize() override;

    void Start(uint32_t gameObjectId) override;

    void Finish(uint32_t gameObjectId) override;

    void Finalize() override;


    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;

};

