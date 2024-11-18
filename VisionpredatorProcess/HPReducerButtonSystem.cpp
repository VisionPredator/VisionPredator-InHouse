#include "pch.h"
#include "HPReducerButtonSystem.h"
#include "EventManager.h"
#include "HPReducerComponent.h"
#include "PlayerComponent.h"
// Define a custom max function

HPReducerButtonSystem::HPReducerButtonSystem(std::shared_ptr<SceneManager> sceneManager):System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnInterected", CreateSubscriber(&HPReducerButtonSystem::OnInterected));
}


void HPReducerButtonSystem::OnInterected(std::any data) 
{
    // std::any_cast를 사용해 data를 std::pair로 변환하여 interected와 interector에 저장
    auto [interected, interector] = std::any_cast<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>>(data);

    // interected에 HPReducerComponent가 없거나, interector에 PlayerComponent가 없으면 함수 종료
    if (!interected->HasComponent<HPReducerComponent>() || !interector->HasComponent<PlayerComponent>())
        return;

    // HPReducerComponent와 PlayerComponent를 가져옴
    auto hpdown = interected->GetComponent<HPReducerComponent>();
    auto player = interector->GetComponent<PlayerComponent>();

    int& hp = player->HP;       // 현재 플레이어의 HP를 참조로 가져옴
    int& maxhp = player->MaxHP; // 현재 플레이어의 최대 HP를 참조로 가져옴

    // 만약 hpdown->MaxHP와 player의 maxhp가 같다면 함수 종료
    if (hpdown->MaxHP == maxhp)
        return;

    // 새로운 최대 HP를 계산하며, hpdown->MaxHP까지 깎이는 것을 제한함 (CustomMax 사용)
    int newMaxHP = VPMath::CustomMax(maxhp - hpdown->DownHP, hpdown->MaxHP);

    // 현재 HP를 비율 유지하며 조정
    if (maxhp > 0) {
        float hpRatio = static_cast<float>(hp) / maxhp; // 현재 HP 비율 계산
        hp = static_cast<int>(std::ceil(hpRatio * newMaxHP)); // 비율 유지하며 새로운 최대 HP에 맞게 HP 조정, 소수점 올림 처리
    }

    // 새로운 최대 HP 설정
    maxhp = newMaxHP;
}
