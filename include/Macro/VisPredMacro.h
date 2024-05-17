#pragma once
//#include <Windows.h> // MessageBox �Լ��� ����ϱ� ���� �ʿ�
//#include <iostream>
//#include <sstream> // std::stringstream�� ����ϱ� ���� �ʿ�



// NDEBUG�� ���ǵǾ� ������, VISPRED_ASSERT ��ũ�δ� �ƹ� ���۵� ���� �ʽ��ϴ�.
#ifdef NDEBUG
#define VP_ASSERT(condition, message) ((void)0)
#else
/// \def VISPRED_ASSERT(condition, message)
/// \brief ������ ������ ��, �޽����� �Բ� MessageBox�� ����, 
/// \brief ���ÿ� ���� ���α׷� ���� �Ǵ� �ߴ��մϴ�.
/// \param condition : �˻��� ����
/// \param message : ����ڿ��� ������ �޽���
#define VP_ASSERT(condition, message) \
    do \
    { \
        if (!(condition)) \
        { \
            std::stringstream ss; \
            std::string filePath(__FILE__); \
            auto pos = filePath.find_last_of("/\\"); \
            auto folderPos = filePath.rfind('\\', pos ? pos - 1 : 0); \
            ss << "���� : " << (message) << "\n" \
               << "\n�������� : " << filePath.substr(folderPos + 1, pos - folderPos - 1) \
               << "\n���ϸ� : " << filePath.substr(pos + 1) \
               << "\n�� : " << __LINE__; \
            int result = MessageBoxA(NULL, ss.str().c_str(), "VP_ASSERT Failed", MB_ICONERROR | MB_ABORTRETRYIGNORE); \
            if (result == IDABORT)\
                std::exit(EXIT_FAILURE); \
            else if (result == IDRETRY) \
                __debugbreak(); \
        } \
    } \
    while (false);

#endif

// Macro to simplify type ID definition
#define DEFINE_COMPONENT_TYPE(TYPE) \
public: \
    static const entt::id_type typeID; \
    entt::id_type GetTypeID() const override { return TYPE::typeID; }

#define IMPLEMENT_COMPONENT_TYPE(TYPE) \
    const entt::id_type TYPE::typeID =entt::resolve<TYPE>().id();

/// \def VP_JSONBODY(CLASSNAME,...)
/// \brief �ش� ������Ʈ�� �ø�������� �Լ��� ��ø�������� �Լ��� �����Ѵ�.
/// \param CLASSNAME : Class
/// \param �߰� �Ű����� : �������ִ� ������� ���� ���缭 �������ּ���.
#define VP_JSONBODY(CLASSNAME,...)\
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CLASSNAME, __VA_ARGS__)\
DEFINE_COMPONENT_TYPE(CLASSNAME)\
void* AddComponent( SceneManager* sceneManager,uint32_t entityID) override\
{\
CLASSNAME* component= sceneManager->AddComponent<CLASSNAME>(entityID);\
component->OwnedEntity = sceneManager->GetEntity(entityID);\
return component;\
}\
void SerializeComponent(nlohmann::json& json)const override\
 { to_json(json, *this); }\
void* DeserializeComponent(const nlohmann::json json, SceneManager* sceneManager,uint32_t entityID) const override\
{\
	auto component = sceneManager->AddComponent<CLASSNAME>(entityID);\
	*component = json;\
	component->OwnedEntity = sceneManager->GetEntity(entityID);\
    return component;\
}\



