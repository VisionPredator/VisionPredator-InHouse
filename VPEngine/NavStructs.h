#pragma once
struct BuildSettings
{
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_VER2(BuildSettings, UseNavMesh, MaxCrowdNumber, MaxAgentRadius, WalkableSlopeAngle, WalkableClimb, WalkableHeight, DivisionSizeXZ, DivisionSizeY)

    bool UseNavMesh = false;
    // ��ã�� ��ü���� �ִ� ��ü��
    int MaxCrowdNumber{ 64 };
    // ��ã�� ��ü���� �ִ� �浹�ݰ�
    float MaxAgentRadius{ 0.6f };
    // ������ �ִ� ���
    float WalkableSlopeAngle{ 30.f };
    // ���� �� �ִ� ����
    float WalkableClimb{ 0.2f };
    // õ���� �ּ� ����
    float WalkableHeight{ 0.3f };
    // x��,z�� ���� ������ ����, ������ �������� ������ �� ������������, �׺���̼� ����� ��ã�� �ý����� ���ϰ� �ð� �ȴ�.
    float DivisionSizeXZ{ 0.3f };
    // y�� ���� ������ ����, ������ �������� ������ �� ������������, �׺���̼� ����� ��ã�� �ý����� ���ϰ� �ð� �ȴ�.
    float DivisionSizeY{ 0.2f };
    // ���� ������ xz������ 250*330, y������ 200�� ���� ���ҵǴ� ������ ���İ��� �׺���̼� ���嵵 �Ǹ鼭 ��ã�⵵ �������� �ϰ� �Ǵ� ������.
    // xz������ 743* 989�� ���� ������ �ǵ� ū ���ϴ� ����.
};

