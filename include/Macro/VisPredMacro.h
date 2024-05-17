#pragma once
//#include <Windows.h> // MessageBox 함수를 사용하기 위해 필요
//#include <iostream>
//#include <sstream> // std::stringstream을 사용하기 위해 필요



// NDEBUG가 정의되어 있으면, VISPRED_ASSERT 매크로는 아무 동작도 하지 않습니다.
#ifdef NDEBUG
#define VP_ASSERT(condition, message) ((void)0)
#else
/// \def VISPRED_ASSERT(condition, message)
/// \brief 조건이 거짓일 때, 메시지와 함께 MessageBox를 띄우고, 
/// \brief 선택에 따라 프로그램 종료 또는 중단합니다.
/// \param condition : 검사할 조건
/// \param message : 사용자에게 보여줄 메시지
#define VP_ASSERT(condition, message) \
    do \
    { \
        if (!(condition)) \
        { \
            std::stringstream ss; \
            std::string filePath(__FILE__); \
            auto pos = filePath.find_last_of("/\\"); \
            auto folderPos = filePath.rfind('\\', pos ? pos - 1 : 0); \
            ss << "원인 : " << (message) << "\n" \
               << "\n상위폴더 : " << filePath.substr(folderPos + 1, pos - folderPos - 1) \
               << "\n파일명 : " << filePath.substr(pos + 1) \
               << "\n줄 : " << __LINE__; \
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
/// \brief 해당 컴포넌트에 시리얼라이즈 함수와 디시리얼라이즈 함수를 구현한다.
/// \param CLASSNAME : Class
/// \param 추가 매개변수 : 가지고있는 멤버변수 순서 맞춰서 기입해주세요.
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



