#include "pch.h"
#include "TrunSystem.h"
#include "TrunComponent.h"
TrunSystem::TrunSystem(std::shared_ptr<SceneManager> SceneManager) :System(SceneManager)
{
}

void TrunSystem::FixedUpdate(float deltaTime) {
    COMPLOOP(TrunComponent, truncomp) {
        // 트런이 완료되었으면 해당 엔티티 삭제
        if (truncomp.Finished) {
            GetSceneManager()->DestroyEntity(truncomp.GetEntityID());
            continue;
        }

        // ProgressTime이 MoveTime을 초과할 경우 완료 플래그 설정 및 ProgressTime 고정
        if (truncomp.ProgressTime >= truncomp.MoveTime) {
            truncomp.Finished = true;
            truncomp.ProgressTime = truncomp.MoveTime;
        }

        // Slerp 진행: 두 단계로 나누어 중간 지점까지 또는 중간 지점에서 끝까지 회전
        float interpolationFactor{};
        VPMath::Quaternion quat{};

        if (truncomp.Angle >= 180.0f) {
            // 두 단계로 나누어 Slerp
            float halfMoveTime = truncomp.MoveTime / 2.0f;
            if (truncomp.ProgressTime < halfMoveTime) 
            {
                // Stage 1: StartPoint에서 HalfPoint까지 Slerp
                interpolationFactor = truncomp.ProgressTime / halfMoveTime;
                quat = VPMath::Quaternion::Slerp(truncomp.StartPoint, truncomp.HalfPoint, interpolationFactor);
            }
            else 
            {
                // Stage 2: HalfPoint에서 EndPoint까지 Slerp
                interpolationFactor = (truncomp.ProgressTime - halfMoveTime) / halfMoveTime;
                quat = VPMath::Quaternion::Slerp(truncomp.HalfPoint, truncomp.EndPoint, interpolationFactor);
            }
        }
        else {
            // 단일 Slerp: StartPoint에서 EndPoint까지
            interpolationFactor = truncomp.ProgressTime / truncomp.MoveTime;
            quat = VPMath::Quaternion::Slerp(truncomp.StartPoint, truncomp.EndPoint, interpolationFactor);
        }

        // TransformComponent에 계산된 쿼터니언을 로컬 회전으로 설정
        truncomp.GetComponent<TransformComponent>()->SetLocalQuaternion(quat);

        // deltaTime을 통해 ProgressTime 업데이트
        truncomp.ProgressTime += deltaTime;
    }
}

void TrunSystem::Initialize()
{
	COMPLOOP(TrunComponent, truncomp)
	{
		Start(truncomp.GetEntityID());
	}

}
void TrunSystem::Start(uint32_t gameObjectId)
{
    if (!GetSceneManager()->HasComponent<TrunComponent>(gameObjectId))
        return;

    auto truncomp = GetSceneManager()->GetComponent<TrunComponent>(gameObjectId);
    auto transcomp = GetSceneManager()->GetComponent<TransformComponent>(gameObjectId);

    // 방향 결정 (Is_Left가 true일 때 방향을 설정: 왼쪽이면 양수, 오른쪽이면 음수)
    float direction = truncomp->Is_Left ? 1.0f : -1.0f;

    // 회전 축 설정 (로컬 공간에서 Y축 기준으로 회전하거나, 설정에 따라 X축으로 회전)
    VPMath::Vector3 rotationAxis = truncomp->Is_X ? VPMath::Vector3(0.0f, 1.0f, 0.0f) : VPMath::Vector3(1.0f, 0.0f, 0.0f);
    rotationAxis.Normalize();  // 회전 축 정규화

    // 로컬 회전 쿼터니언 가져오기
    VPMath::Quaternion localRotation = transcomp->Local_Quaternion;

    // 시작 및 종료 각도를 설정 (기준 각도 ± Angle/2)
    float halfAngle = DegToRad(truncomp->Angle / 2);
    float startAngle = direction * -halfAngle;
    float endAngle = direction * halfAngle;

    // 시작 및 종료 쿼터니언 생성
    VPMath::Quaternion rotationStart = VPMath::Quaternion::CreateFromAxisAngle(rotationAxis, startAngle);
    VPMath::Quaternion rotationEnd = VPMath::Quaternion::CreateFromAxisAngle(rotationAxis, endAngle);

    // StartPoint와 EndPoint를 현재 로컬 쿼터니언에 곱하여 설정
    truncomp->StartPoint = rotationStart * localRotation;
    truncomp->HalfPoint = localRotation;
    truncomp->EndPoint = rotationEnd * localRotation;

    // 트랜스폼에 초기 회전 설정
    transcomp->SetWorldQuaternion(truncomp->StartPoint);
}

void TrunSystem::Finish(uint32_t gameObjectId)
{
}

void TrunSystem::Finalize()
{
}
