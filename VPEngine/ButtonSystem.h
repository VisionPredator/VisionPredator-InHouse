#pragma once
#include "System.h"
#include "EventSubscriber.h"
class ButtonSystem :
    public System, public EventSubscriber, public IUpdatable, public IRenderable
{
public:
    ButtonSystem(std::shared_ptr<SceneManager> sceneManager);
    ~ButtonSystem() = default;

    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;


    // IRenderable을(를) 통해 상속됨
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

    //자신을 포함한 종속ui들 다 on off
    void OnOffUISelf(std::any entity);


    std::set<int> activeLayer;

    int curUILayer = 9999;
    int minUILayer = -1;
};

