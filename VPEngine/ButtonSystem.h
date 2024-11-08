#pragma once
#include "System.h"
#include "EventSubscriber.h"
class ButtonSystem :
    public System, public EventSubscriber, public IUpdatable, public IRenderable
{
public:
    ButtonSystem(std::shared_ptr<SceneManager> sceneManager);
    ~ButtonSystem() = default;

    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;


    // IRenderable��(��) ���� ��ӵ�
    void BeginRenderUpdate(float deltaTime) override;

    void RenderUpdate(float deltaTime) override;

    void LateRenderUpdate(float deltaTime) override;

    void EditorRenderUpdate(float deltaTime) override;
private:


    void OnSceneChange(std::any scenename);
    void OnSpawnPrefab(std::any prefabname);
    void OnDestrorPrefab(std::any uipreb);
    void OnOffUI(std::any entity);
    void OnDragLR(std::any entity);

    //�ڽ��� ������ ����ui�� �� on off
    void OnOffUISelf(std::any entity);


    std::set<int> activeLayer;

    int curUILayer = 9999;
    int minUILayer = -1;
};

