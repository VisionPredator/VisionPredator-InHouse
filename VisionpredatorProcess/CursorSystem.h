#pragma once
#include <System.h>
class CursorSystem :
    public System,public IStartable,public IFixedUpdatable
{
public: 
    CursorSystem(std::shared_ptr<SceneManager> scenemanager);

    // IStartable��(��) ���� ��ӵ�
    void Initialize() override;
    void Start(uint32_t gameObjectId) override;
    void Finish(uint32_t gameObjectId) override;
    void Finalize() override;
   
    void SetImagePos(ImageComponent* Cursor);

    // IFixedUpdatable��(��) ���� ��ӵ�
    void FixedUpdate(float deltaTime) override;
};

