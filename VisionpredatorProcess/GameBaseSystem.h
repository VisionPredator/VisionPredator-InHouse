#pragma once
#include <System.h>
class GameBaseSystem :
    public System,public IStartable,public IUpdatable
{
public:
    GameBaseSystem(std::shared_ptr<SceneManager> sceneManager);


    // IStartable��(��) ���� ��ӵ�
    void Initialize() override;

    void Start(uint32_t gameObjectId) override;

    void Finish(uint32_t gameObjectId) override;

    void Finalize() override;


    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;

};

