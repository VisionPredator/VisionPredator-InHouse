#pragma once

struct BuildSettings
{
    bool UseNavMesh = true;
    // 길찾기 주체들의 최대 개체수
    int maxCrowdNumber{ 1024 };
    // 길찾기 주체들의 최대 충돌반경
    float maxAgentRadius{ 0.6f };
    // 오를수 있는 경사
    float walkableSlopeAngle{ 30.f };
    // 오를 수 있는 단차
    float walkableClimb{ 0.2f };
    // 천장의 최소 높이
    float walkableHeight{ 0.3f };
    // x축,z축 공간 분할의 단위, 단위가 작을수록 판정이 더 세밀해지지만, 네비게이션 빌드와 길찾기 시스템의 부하가 늘게 된다.
    float divisionSizeXZ{ 0.3f };
    // y축 공간 분할의 단위, 단위가 작을수록 판정이 더 세밀해지지만, 네비게이션 빌드와 길찾기 시스템의 부하가 늘게 된다.
    float divisionSizeY{ 0.2f };
    // 공간 분할은 xz축으로 250*330, y축으로 200개 정도 분할되는 정도면 순식간에 네비게이션 빌드도 되면서 길찾기도 무리없이 하게 되는 정도다.
    // xz축으로 743* 989개 정도 분할이 되도 큰 부하는 없다.
};
