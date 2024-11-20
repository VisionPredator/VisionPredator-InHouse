#pragma once
#include <System.h>
struct TextBounceComponent;
struct ImageBounceComponent;
class UIBounceSystem :
    public System,public IUpdatable,public IStartable
{
public: 
    UIBounceSystem(std::shared_ptr<SceneManager> scenemanager);

    // IFixedUpdatable��(��) ���� ��ӵ�
    void SettingBounce(ImageBounceComponent& bouncecomp);
    void SettingBounce(TextBounceComponent& bouncecomp);
    // IStartable��(��) ���� ��ӵ�
    void Initialize() override;
    void Start(uint32_t gameObjectId) override;
    void Finish(uint32_t gameObjectId) override;
    void Finalize() override;
    float Lerp(float start, float end, float percent);
    double BouncePercent(double progress, double bounceTime, double bounceTimePercent);
    bool BounceReturn(double progress, double totalBounceTime, double bounceTimePercent);


    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;

};

