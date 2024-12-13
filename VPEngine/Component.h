#pragma once // ��� ������ �ߺ� ������ ����

#include "Entity.h"

// ������Ʈ�� ��Ÿ���� �⺻ Ŭ���� ����
struct Component
{
    Component() = default; // �⺻ ������
    virtual ~Component() { OwnedEntity = nullptr; } // �Ҹ��ڿ��� OwnedEntity�� nullptr�� �ʱ�ȭ

    // ���� ������
    Component(const Component& other) : OwnedEntity(other.OwnedEntity) {}

    // ��ƼƼ�� ��Ÿ �����͸� ��ȯ�ϴ� ���� ���� �Լ� (�Ļ� Ŭ�������� ���� �ʿ�)
    virtual entt::meta_handle GetHandle() = 0;

    // ���� �Ҵ� ������
    Component& operator=(const Component& other) {
        if (this != &other) { // �ڱ� �ڽŰ��� �Ҵ� ����
            OwnedEntity = other.OwnedEntity;
        }
        return *this;
    }

    // ������Ʈ�� ����ȭ�ϴ� ���� �Լ� (�Ļ� Ŭ�������� �ʿ信 ���� ������)
    virtual void SerializeComponent(nlohmann::json& json) const {}

    // ������Ʈ�� ��ȯ�ϴ� ���ø� �Լ�
    template <typename T>
    T* GetComponent()
    {
        return OwnedEntity->GetComponent<T>(); // ������ ��ƼƼ���� Ư�� Ÿ���� ������Ʈ ��ȯ
    }

    // Ư�� Ÿ���� ������Ʈ�� �����ϴ��� Ȯ���ϴ� ���ø� �Լ�
    template <typename T>
    bool HasComponent()
    {
        return OwnedEntity->HasComponent<T>();
    }

    // ��ƼƼ�� ID�� ��ȯ
    const uint32_t GetEntityID()
    {
        return OwnedEntity->GetEntityID();
    }

    // ���� ��ƼƼ�� ��ȯ
    Entity* GetEntity()
    {
        return OwnedEntity;
    }

    // ���� ��ƼƼ�� ����
    void SetEntity(Entity* entity)
    {
        OwnedEntity = entity;
    }

    // ���ο� ���� ���� ���� �Լ� �߰�
    // ������ ������Ʈ�� ��ȯ�ϴ� ���� ���� �Լ� (�Ļ� Ŭ�������� ���� �ʿ�)
    virtual std::shared_ptr<Component> Clone() const = 0;

protected:
    // �θ� ��ƼƼ�� ������Ʈ�� �߰��ϴ� ���ø� �Լ�
    template <typename T>
    std::shared_ptr<T> AddComponentToEntity(Entity* parentEntity, bool Immediately = false, bool UseAddCompToScene = true) const {
        // Entity Ŭ������ ����� AddComponent �޼��� ȣ��
        return parentEntity->AddComponent<T>(Immediately, UseAddCompToScene);
    }

    // ��ƼƼ�� ������Ʈ�� �߰��ϴ� ���� �Լ� (�Ļ� Ŭ�������� ���� ����)
    virtual std::shared_ptr<Component> AddComponent(Entity* parentEntity) { return nullptr; }

    // JSON �����͸� ����� ������Ʈ�� ������ȭ�ϴ� ���� �Լ�
    // �ʿ� �� �Ļ� Ŭ�������� ������ ����
    virtual std::shared_ptr<Component> DeserializeComponent(const nlohmann::json& json, Entity* parentEntity, bool Immidiate = false, bool UseAddCompToScene = true) const { return nullptr; }

    Entity* OwnedEntity; // �� ������Ʈ�� ������ ��ƼƼ

    friend class SceneManager; // SceneManager�� �� Ŭ������ private/protected ����� ���� �����ϵ��� ����
};
