#include "pch.h"
#include "PhysichResourceManager.h"
#include "ConvexMeshResource.h"
#include "VPPhysicsStructs.h"

// PhysichResourceManager 클래스의 생성자
PhysichResourceManager::PhysichResourceManager()
{
}

// PhysichResourceManager 클래스 초기화 함수
// PxPhysics 객체를 받아 초기화하며, nullptr이 전달되면 false 반환
bool PhysichResourceManager::Initialize(physx::PxPhysics* physics)
{
	if (!physics)
		return false; // 물리 엔진 객체가 유효하지 않으면 초기화 실패
	m_PxPhysics = physics; // 유효한 PxPhysics 객체를 멤버 변수에 저장
	return true; // 초기화 성공
}

// PhysichResourceManager 클래스의 소멸자
PhysichResourceManager::~PhysichResourceManager()
{
	m_PxPhysics = nullptr;
}

// ConvexMeshResource를 키 값으로 가져오는 함수
std::shared_ptr<ConvexMeshResource> PhysichResourceManager::GetConvexMeshResource(const std::wstring& key)
{
	auto it = m_ConvexMeshMap.find(key); // 키를 검색
	if (it == m_ConvexMeshMap.end())
		return nullptr; // 키가 존재하지 않으면 nullptr 반환
    return it->second; // 키가 존재하면 해당 리소스를 반환
}

// ConvexMeshResource를 로드하고 저장하는 함수
void PhysichResourceManager::LoadConvexMeshResource(const VPPhysics::ConvexMeshResourceInfo& info)
{
    // ConvexMeshResource 객체 생성
    std::shared_ptr<ConvexMeshResource> convexmesh = std::make_shared<ConvexMeshResource>(m_PxPhysics, info);
    // 생성된 리소스를 맵에 저장 (FBXName을 키로 사용)
    m_ConvexMeshMap[info.FBXName] = convexmesh;
}

// 특정 ConvexMeshResource가 존재하는지 확인하는 함수
bool PhysichResourceManager::HasConvexMeshResource(const std::wstring& key)
{
    // 맵에 키가 존재하면 true, 아니면 false 반환
    return m_ConvexMeshMap.find(key) != m_ConvexMeshMap.end();
}
