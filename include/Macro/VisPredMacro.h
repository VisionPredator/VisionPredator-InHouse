#pragma once
#include <random>
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
    while (false)

#endif

// Macro to simplify type ID definition
#define ERRORCOUT(TEXT) std::cout<<"ERROR: "<< TEXT<<std::endl;




/// \def VP_JSONBODY(CLASSNAME,...)
/// \brief 이 매크로는 컴포넌트 클래스에 대해 시리얼라이즈, 디시리얼라이즈, 복제 및 기타 유틸리티 함수를 자동으로 생성합니다.
/// \param CLASSNAME : 클래스 이름
/// \param ... : 클래스의 멤버 변수를 순서대로 입력 (NLOHMANN_DEFINE_TYPE_INTRUSIVE_VER2와 연결)
/// 
/// 이 매크로는 다음 기능을 제공합니다:
/// 1. `AddComponent`: 지정된 엔티티에 컴포넌트를 추가하는 함수
/// 2. `SerializeComponent`: 컴포넌트를 JSON으로 변환하는 함수
/// 3. `DeserializeComponent`: JSON에서 컴포넌트를 역직렬화하고 엔티티에 추가하는 함수
/// 4. `GetHandle`: 메타 데이터를 반환하는 함수
/// 5. `Clone`: 현재 컴포넌트를 복제하는 함수
#define VP_JSONBODY(CLASSNAME,...)\
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_VER2(CLASSNAME, __VA_ARGS__)\
    std::shared_ptr<Component> AddComponent(Entity*  parentEntity) override\
    {\
        auto component = AddComponentToEntity<CLASSNAME>(parentEntity);\
        return component;\
    }\
    void SerializeComponent(nlohmann::json& json) const override\
    {\
        to_json(json, *this);\
    }\
    std::shared_ptr<Component> DeserializeComponent(const nlohmann::json& json, Entity*  parentEntity,bool Immidiate=false,bool UseAddCompToScene=true) const override\
    {\
        auto component = AddComponentToEntity<CLASSNAME>(parentEntity,Immidiate,UseAddCompToScene);\
        *component = json.get<CLASSNAME>();\
        component->SetEntity(parentEntity);\
        return component;\
    }\
    entt::meta_handle GetHandle() override\
    {\
        return *this;\
    }\
    std::shared_ptr<Component> Clone() const override \
    { \
        auto clonedComponent = std::make_shared<CLASSNAME>(*this); \
        clonedComponent->SetEntity(nullptr); /* Clone 후에 새로운 엔티티로 설정 */ \
        return clonedComponent; \
    }
constexpr float DegToRad(float degrees) 
{
    return degrees * (VPMath::XM_PI / 180.0f);
}


