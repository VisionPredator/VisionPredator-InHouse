#pragma once
#include <System.h>
#include "VisPredComponents.h"
#include "EventSubscriber.h"
class QuestSystem :
    public System, public IUpdatable, public IFixedUpdatable,public IStartable,public EventSubscriber
{
public: 
    QuestSystem(std::shared_ptr<SceneManager> scenemanager);
    std::weak_ptr<Entity> m_MainQuestEntity;

    // IFixedUpdatable을(를) 통해 상속됨
    void FixedUpdate(float deltaTime) override;


    // IStartable을(를) 통해 상속됨
    void Initialize() override;

    void Start(uint32_t gameObjectId) override;

    void Finish(uint32_t gameObjectId) override;
    void QuestCheck_VPMOVE(QuestComponent& questcomp) ;
    void QuestCheck_VPJUMP(QuestComponent& questcomp) ;
    void QuestCheck_VPDASH(QuestComponent& questcomp) ;
    void QuestCheck_VPCHANGE(QuestComponent& questcomp) ;
    void QuestCheck_PLAYERSHOOT(QuestComponent& questcomp) ;
    void QuestCheck_PLAYERRUN(QuestComponent& questcomp) ;
    void QuestCheck_PLAYERJUMP(QuestComponent& questcomp) ;
    void QuestCheck_PLAYERCROUCH(QuestComponent& questcomp) ;
    void QuestCheck_PLAYERSLIDE(QuestComponent& questcomp) ;
    void QuestCheck_PLAYERATTACK(QuestComponent& questcomp) ;
    void QuestCheck_PLAYERPICKUP(QuestComponent& questcomp) ;
    void QuestCheck_PLAYERTHROW(QuestComponent& questcomp) ;
    void QuestCheck_PLAYERINTERECT(QuestComponent& questcomp) ;
    void OnInterected(std::any interective_interector);
    void OnTutorialClear(std::any none);

    void Finalize() override;

    
    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;

};

