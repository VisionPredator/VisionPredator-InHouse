#include "pch.h"
#include "TrunSystem.h"
#include "TrunComponent.h"
TrunSystem::TrunSystem(std::shared_ptr<SceneManager> SceneManager) :System(SceneManager)
{
}

void TrunSystem::FixedUpdate(float deltaTime) {
    COMPLOOP(TrunComponent, truncomp) {
        // Ʈ���� �Ϸ�Ǿ����� �ش� ��ƼƼ ����
        if (truncomp.Finished) {
            GetSceneManager()->DestroyEntity(truncomp.GetEntityID());
            continue;
        }

        // ProgressTime�� MoveTime�� �ʰ��� ��� �Ϸ� �÷��� ���� �� ProgressTime ����
        if (truncomp.ProgressTime >= truncomp.MoveTime) {
            truncomp.Finished = true;
            truncomp.ProgressTime = truncomp.MoveTime;
        }

        // Slerp ����: �� �ܰ�� ������ �߰� �������� �Ǵ� �߰� �������� ������ ȸ��
        float interpolationFactor{};
        VPMath::Quaternion quat{};

        if (truncomp.Angle >= 180.0f) {
            // �� �ܰ�� ������ Slerp
            float halfMoveTime = truncomp.MoveTime / 2.0f;
            if (truncomp.ProgressTime < halfMoveTime) 
            {
                // Stage 1: StartPoint���� HalfPoint���� Slerp
                interpolationFactor = truncomp.ProgressTime / halfMoveTime;
                quat = VPMath::Quaternion::Slerp(truncomp.StartPoint, truncomp.HalfPoint, interpolationFactor);
            }
            else 
            {
                // Stage 2: HalfPoint���� EndPoint���� Slerp
                interpolationFactor = (truncomp.ProgressTime - halfMoveTime) / halfMoveTime;
                quat = VPMath::Quaternion::Slerp(truncomp.HalfPoint, truncomp.EndPoint, interpolationFactor);
            }
        }
        else {
            // ���� Slerp: StartPoint���� EndPoint����
            interpolationFactor = truncomp.ProgressTime / truncomp.MoveTime;
            quat = VPMath::Quaternion::Slerp(truncomp.StartPoint, truncomp.EndPoint, interpolationFactor);
        }

        // TransformComponent�� ���� ���ʹϾ��� ���� ȸ������ ����
        truncomp.GetComponent<TransformComponent>()->SetLocalQuaternion(quat);

        // deltaTime�� ���� ProgressTime ������Ʈ
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

    // ���� ���� (Is_Left�� true�� �� ������ ����: �����̸� ���, �������̸� ����)
    float direction = truncomp->Is_Left ? 1.0f : -1.0f;

    // ȸ�� �� ���� (���� �������� Y�� �������� ȸ���ϰų�, ������ ���� X������ ȸ��)
    VPMath::Vector3 rotationAxis = truncomp->Is_X ? VPMath::Vector3(0.0f, 1.0f, 0.0f) : VPMath::Vector3(1.0f, 0.0f, 0.0f);
    rotationAxis.Normalize();  // ȸ�� �� ����ȭ

    // ���� ȸ�� ���ʹϾ� ��������
    VPMath::Quaternion localRotation = transcomp->Local_Quaternion;

    // ���� �� ���� ������ ���� (���� ���� �� Angle/2)
    float halfAngle = DegToRad(truncomp->Angle / 2);
    float startAngle = direction * -halfAngle;
    float endAngle = direction * halfAngle;

    // ���� �� ���� ���ʹϾ� ����
    VPMath::Quaternion rotationStart = VPMath::Quaternion::CreateFromAxisAngle(rotationAxis, startAngle);
    VPMath::Quaternion rotationEnd = VPMath::Quaternion::CreateFromAxisAngle(rotationAxis, endAngle);

    // StartPoint�� EndPoint�� ���� ���� ���ʹϾ� ���Ͽ� ����
    truncomp->StartPoint = rotationStart * localRotation;
    truncomp->HalfPoint = localRotation;
    truncomp->EndPoint = rotationEnd * localRotation;

    // Ʈ�������� �ʱ� ȸ�� ����
    transcomp->SetWorldQuaternion(truncomp->StartPoint);
}

void TrunSystem::Finish(uint32_t gameObjectId)
{
}

void TrunSystem::Finalize()
{
}
