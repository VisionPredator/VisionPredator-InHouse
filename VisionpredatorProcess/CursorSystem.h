#pragma once
#include <System.h>
class CursorSystem :
    public System,public IStartable,public IFixedUpdatable
{
public: 
    CursorSystem(std::shared_ptr<SceneManager> scenemanager);

    // IStartable을(를) 통해 상속됨
    void Initialize() override;
    void Start(uint32_t gameObjectId) override;
    void Finish(uint32_t gameObjectId) override;
    void Finalize() override;
   
    void SetImagePos(ImageComponent* Cursor);

    // IFixedUpdatable을(를) 통해 상속됨
    void FixedUpdate(float deltaTime) override;
};

