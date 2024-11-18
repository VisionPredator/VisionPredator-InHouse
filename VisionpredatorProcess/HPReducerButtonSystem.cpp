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
    // std::any_cast�� ����� data�� std::pair�� ��ȯ�Ͽ� interected�� interector�� ����
    auto [interected, interector] = std::any_cast<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>>(data);

    // interected�� HPReducerComponent�� ���ų�, interector�� PlayerComponent�� ������ �Լ� ����
    if (!interected->HasComponent<HPReducerComponent>() || !interector->HasComponent<PlayerComponent>())
        return;

    // HPReducerComponent�� PlayerComponent�� ������
    auto hpdown = interected->GetComponent<HPReducerComponent>();
    auto player = interector->GetComponent<PlayerComponent>();

    int& hp = player->HP;       // ���� �÷��̾��� HP�� ������ ������
    int& maxhp = player->MaxHP; // ���� �÷��̾��� �ִ� HP�� ������ ������

    // ���� hpdown->MaxHP�� player�� maxhp�� ���ٸ� �Լ� ����
    if (hpdown->MaxHP == maxhp)
        return;

    // ���ο� �ִ� HP�� ����ϸ�, hpdown->MaxHP���� ���̴� ���� ������ (CustomMax ���)
    int newMaxHP = VPMath::CustomMax(maxhp - hpdown->DownHP, hpdown->MaxHP);

    // ���� HP�� ���� �����ϸ� ����
    if (maxhp > 0) {
        float hpRatio = static_cast<float>(hp) / maxhp; // ���� HP ���� ���
        hp = static_cast<int>(std::ceil(hpRatio * newMaxHP)); // ���� �����ϸ� ���ο� �ִ� HP�� �°� HP ����, �Ҽ��� �ø� ó��
    }

    // ���ο� �ִ� HP ����
    maxhp = newMaxHP;
}
