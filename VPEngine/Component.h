#pragma once // 헤더 파일의 중복 포함을 방지

#include "Entity.h"

// 컴포넌트를 나타내는 기본 클래스 정의
struct Component
{
    Component() = default; // 기본 생성자
    virtual ~Component() { OwnedEntity = nullptr; } // 소멸자에서 OwnedEntity를 nullptr로 초기화

    // 복사 생성자
    Component(const Component& other) : OwnedEntity(other.OwnedEntity) {}

    // 엔티티의 메타 데이터를 반환하는 순수 가상 함수 (파생 클래스에서 구현 필요)
    virtual entt::meta_handle GetHandle() = 0;

    // 복사 할당 연산자
    Component& operator=(const Component& other) {
        if (this != &other) { // 자기 자신과의 할당 방지
            OwnedEntity = other.OwnedEntity;
        }
        return *this;
    }

    // 컴포넌트를 직렬화하는 가상 함수 (파생 클래스에서 필요에 따라 재정의)
    virtual void SerializeComponent(nlohmann::json& json) const {}

    // 컴포넌트를 반환하는 템플릿 함수
    template <typename T>
    T* GetComponent()
    {
        return OwnedEntity->GetComponent<T>(); // 소유한 엔티티에서 특정 타입의 컴포넌트 반환
    }

    // 특정 타입의 컴포넌트가 존재하는지 확인하는 템플릿 함수
    template <typename T>
    bool HasComponent()
    {
        return OwnedEntity->HasComponent<T>();
    }

    // 엔티티의 ID를 반환
    const uint32_t GetEntityID()
    {
        return OwnedEntity->GetEntityID();
    }

    // 소유 엔티티를 반환
    Entity* GetEntity()
    {
        return OwnedEntity;
    }

    // 소유 엔티티를 설정
    void SetEntity(Entity* entity)
    {
        OwnedEntity = entity;
    }

    // 새로운 순수 가상 복제 함수 추가
    // 복제된 컴포넌트를 반환하는 순수 가상 함수 (파생 클래스에서 구현 필요)
    virtual std::shared_ptr<Component> Clone() const = 0;

protected:
    // 부모 엔티티에 컴포넌트를 추가하는 템플릿 함수
    template <typename T>
    std::shared_ptr<T> AddComponentToEntity(Entity* parentEntity, bool Immediately = false, bool UseAddCompToScene = true) const {
        // Entity 클래스의 비공개 AddComponent 메서드 호출
        return parentEntity->AddComponent<T>(Immediately, UseAddCompToScene);
    }

    // 엔티티에 컴포넌트를 추가하는 가상 함수 (파생 클래스에서 구현 가능)
    virtual std::shared_ptr<Component> AddComponent(Entity* parentEntity) { return nullptr; }

    // JSON 데이터를 사용해 컴포넌트를 역직렬화하는 가상 함수
    // 필요 시 파생 클래스에서 재정의 가능
    virtual std::shared_ptr<Component> DeserializeComponent(const nlohmann::json& json, Entity* parentEntity, bool Immidiate = false, bool UseAddCompToScene = true) const { return nullptr; }

    Entity* OwnedEntity; // 이 컴포넌트를 소유한 엔티티

    friend class SceneManager; // SceneManager가 이 클래스의 private/protected 멤버에 접근 가능하도록 설정
};
