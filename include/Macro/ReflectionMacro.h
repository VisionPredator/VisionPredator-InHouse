#pragma once
///기태님 코드
///자동 리플렉션 시스템
/// CPP에 사용하고 cpp 컴파일을 위해서 쓰레기 코드라도 남긴다.

#define CAT_IMPL(x, y) x##y //Concatenate Implementation 
#define CAT(x, y) CAT_IMPL(x, y)
static void AutoRegisterMetaTypeFunction();

#define FQ_REGISTRATION_FRIEND friend void ::AutoRegisterMetaTypeFunction();

#pragma region Meta Member 등록

#define MEMBER(NAME)\
 .data<& NAME >(#NAME##_hs)

#define MEMBER_EXPAND( x ) x
#define MEMBER_GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME,...) NAME
#define MEMBER_PASTE(...) MEMBER_EXPAND(MEMBER_GET_MACRO(__VA_ARGS__, \
        MEMBER_PASTE10, \
        MEMBER_PASTE9, \
        MEMBER_PASTE8, \
        MEMBER_PASTE7, \
        MEMBER_PASTE6, \
        MEMBER_PASTE5, \
        MEMBER_PASTE4, \
        MEMBER_PASTE3, \
        MEMBER_PASTE2, \
        MEMBER_PASTE1)(__VA_ARGS__))
#define MEMBER_PASTE2(func, v1) func(v1)
#define MEMBER_PASTE3(func, v1, v2) MEMBER_PASTE2(func, v1) MEMBER_PASTE2(func, v2)
#define MEMBER_PASTE4(func, v1, v2, v3) MEMBER_PASTE2(func, v1) MEMBER_PASTE3(func, v2, v3)
#define MEMBER_PASTE5(func, v1, v2, v3, v4) MEMBER_PASTE2(func, v1) MEMBER_PASTE4(func, v2, v3, v4)
#define MEMBER_PASTE6(func, v1, v2, v3, v4, v5) MEMBER_PASTE2(func, v1) MEMBER_PASTE5(func, v2, v3, v4, v5)
#define MEMBER_PASTE7(func, v1, v2, v3, v4, v5, v6) MEMBER_PASTE2(func, v1) MEMBER_PASTE6(func, v2, v3, v4, v5, v6)
#define MEMBER_PASTE8(func, v1, v2, v3, v4, v5, v6, v7) MEMBER_PASTE2(func, v1) MEMBER_PASTE7(func, v2, v3, v4, v5, v6, v7)
#define MEMBER_PASTE9(func, v1, v2, v3, v4, v5, v6, v7, v8) MEMBER_PASTE2(func, v1) MEMBER_PASTE8(func, v2, v3, v4, v5, v6, v7, v8)
#define MEMBER_PASTE10(func, v1, v2, v3, v4, v5, v6, v7, v8, v9) MEMBER_PASTE2(func, v1) MEMBER_PASTE9(func, v2, v3, v4, v5, v6, v7, v8, v9)

#define  MEMBERS(...)  MEMBER_EXPAND( MEMBER_PASTE( MEMBER, __VA_ARGS__))

#define META_ADD_MEMBER(CLASS,...)\
entt::meta<CLASS>()\
.type(#CLASS##_hs)\
MEMBERS(__VA_ARGS__)\
.func<&CLASS::SerializeComponent>("SerializeComponent"_hs)\
.func<&CLASS::DeserializeComponent>("DeserializeComponent"_hs)\
.func<&CLASS::AddComponent>("AddComponent"_hs);



#pragma endregion


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





namespace Reflection
{
    template<typename T>
    static entt::id_type GetTypeID() {
    static entt::id_type typeID = entt::resolve<T>().id();
    return typeID;
}
}