#pragma once
///���´� �ڵ�
///�ڵ� ���÷��� �ý���
/// CPP�� ����ϰ� cpp �������� ���ؼ� ������ �ڵ�� �����.

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