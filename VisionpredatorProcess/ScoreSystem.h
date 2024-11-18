#pragma once
#include <System.h>
#include "EventSubscriber.h"
class ScoreSystem :
	public System, public EventSubscriber,public IStartable
{
public: ScoreSystem(std::shared_ptr<SceneManager> scenemanager);

	  
	  void OnEnemyKilled(std::any null = {});
	  void OnHideScore(std::any null = {});
	  void OnScoreToMiddle(std::any null = {});
	  void OnEndingBestScore(std::any null = {});
	  std::weak_ptr <Entity> m_Score;
	  std::weak_ptr <Entity> m_BestScore;

	  // IStartable을(를) 통해 상속됨
	  void Initialize() override;
	  void Start(uint32_t gameObjectId) override;
	  void Finish(uint32_t gameObjectId) override;
	  void Finalize() override;
};

