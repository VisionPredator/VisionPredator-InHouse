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
    while (false)

#endif

// Macro to simplify type ID definition
#define ERRORCOUT(TEXT) std::cout<<"ERROR: "<< TEXT<<std::endl;



/// \def VP_JSONBODY(CLASSNAME,...)
/// \brief �ش� ������Ʈ�� �ø�������� �Լ��� ��ø�������� �Լ��� �����Ѵ�.
/// \param CLASSNAME : Class
/// \param �߰� �Ű����� : �������ִ� ������� ���� ���缭 �������ּ���.
#define VP_JSONBODY(CLASSNAME,...)\
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_VER2(CLASSNAME, __VA_ARGS__)\
    std::shared_ptr<Component> AddComponent(Entity*  parentEntity) override\
    {\
        auto component = parentEntity->AddComponent<CLASSNAME>();\
        return component;\
    }\
    void SerializeComponent(nlohmann::json& json) const override\
    {\
        to_json(json, *this);\
    }\
    std::shared_ptr<Component> DeserializeComponent(const nlohmann::json& json, Entity*  parentEntity,bool Immidiate=false,bool UseAddCompToScene=true) const override\
    {\
        auto component = parentEntity->AddComponent<CLASSNAME>(Immidiate,UseAddCompToScene);\
        *component = json.get<CLASSNAME>();\
        component->SetEntity(parentEntity);\
        return component;\
    }\
    entt::meta_handle GetHandle() override\
    {\
        return *this;\
    }

//