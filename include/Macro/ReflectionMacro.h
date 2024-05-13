#pragma once
///기태님 코드
///자동 리플렉션 시스템
/// CPP에 사용하고 cpp 컴파일을 위해서 쓰레기 코드라도 남긴다.

#define CAT_IMPL(x, y) x##y //Concatenate Implementation 
#define CAT(x, y) CAT_IMPL(x, y)
static void AutoRegisterMetaTypeFunction();

#define FQ_REGISTRATION_FRIEND friend void ::AutoRegisterMetaTypeFunction();

#define META_REGISTRATION                                                           \
static void AutoRegisterMetaTypeFunction();                                         \
namespace                                                                           \
{                                                                                   \
    struct fq__auto__register__                                                     \
    {                                                                               \
        fq__auto__register__()                                                      \
        {                                                                           \
            AutoRegisterMetaTypeFunction();                                         \
        }                                                                           \
    };                                                                              \
}                                                                                   \
static const fq__auto__register__ CAT(auto_register__, __LINE__);                   \
static void AutoRegisterMetaTypeFunction()


namespace VisPred::Reflection
{
    template<typename T>
    static entt::id_type GetTypeID() {
    static entt::id_type typeID = entt::resolve<T>().id();
    return typeID;
}
}