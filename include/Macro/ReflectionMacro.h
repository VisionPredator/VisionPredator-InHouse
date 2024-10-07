#pragma once


#pragma region Meta Member 등록

///Class 기반
#define MEMBER(NAME)\
 .data<& NAME >(#NAME##_hs)\
		.prop(Reflection::Prop::Name,#NAME)
#define MEMBER_EXPAND( x ) x
#define MEMBER_GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,_31, _32, _33, _34, _35, _36, _37, _38, _39, _40, NAME, ...) NAME
#define MEMBER_PASTE(...) MEMBER_EXPAND(MEMBER_GET_MACRO(__VA_ARGS__, \
		MEMBER_PASTE40, \
		MEMBER_PASTE39, \
		MEMBER_PASTE38, \
		MEMBER_PASTE37, \
		MEMBER_PASTE36, \
		MEMBER_PASTE35, \
		MEMBER_PASTE34, \
		MEMBER_PASTE33, \
		MEMBER_PASTE32, \
		MEMBER_PASTE31, \
        MEMBER_PASTE30, \
        MEMBER_PASTE29, \
        MEMBER_PASTE28, \
        MEMBER_PASTE27, \
        MEMBER_PASTE26, \
        MEMBER_PASTE25, \
        MEMBER_PASTE24, \
        MEMBER_PASTE23, \
        MEMBER_PASTE22, \
        MEMBER_PASTE21, \
        MEMBER_PASTE20, \
        MEMBER_PASTE19, \
        MEMBER_PASTE18, \
        MEMBER_PASTE17, \
        MEMBER_PASTE16, \
        MEMBER_PASTE15, \
        MEMBER_PASTE14, \
        MEMBER_PASTE13, \
        MEMBER_PASTE12, \
        MEMBER_PASTE11, \
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

#define MEMBER_PASTE2(func, v1)\
 func(v1)
#define MEMBER_PASTE3(func, v1, v2) \
 MEMBER_PASTE2(func, v1) MEMBER_PASTE2(func, v2)
#define MEMBER_PASTE4(func, v1, v2, v3) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE3(func, v2, v3)
#define MEMBER_PASTE5(func, v1, v2, v3, v4) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE4(func, v2, v3, v4)
#define MEMBER_PASTE6(func, v1, v2, v3, v4, v5) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE5(func, v2, v3, v4, v5)
#define MEMBER_PASTE7(func, v1, v2, v3, v4, v5, v6) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE6(func, v2, v3, v4, v5, v6)
#define MEMBER_PASTE8(func, v1, v2, v3, v4, v5, v6, v7) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE7(func, v2, v3, v4, v5, v6, v7)
#define MEMBER_PASTE9(func, v1, v2, v3, v4, v5, v6, v7, v8) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE8(func, v2, v3, v4, v5, v6, v7, v8)
#define MEMBER_PASTE10(func, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE9(func, v2, v3, v4, v5, v6, v7, v8, v9)
#define MEMBER_PASTE11(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
 MEMBER_PASTE2(func, v1) MEMBER_PASTE10(func, v2, v3, v4, v5, v6, v7, v8, v9, v10)
#define MEMBER_PASTE12(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE11(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)
#define MEMBER_PASTE13(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE12(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12)
#define MEMBER_PASTE14(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE13(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13)
#define MEMBER_PASTE15(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE14(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14)
#define MEMBER_PASTE16(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE15(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15)
#define MEMBER_PASTE17(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE16(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16)
#define MEMBER_PASTE18(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE17(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17)
#define MEMBER_PASTE19(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE18(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18)
#define MEMBER_PASTE20(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE19(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19)
#define MEMBER_PASTE21(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE20(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20)
#define MEMBER_PASTE22(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE21(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21)
#define MEMBER_PASTE23(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE22(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22)
#define MEMBER_PASTE24(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE23(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23)
#define MEMBER_PASTE25(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE24(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24)
#define MEMBER_PASTE26(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE25(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25)
#define MEMBER_PASTE27(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE26(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26)
#define MEMBER_PASTE28(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE27(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27)
#define MEMBER_PASTE29(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE28(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28)
#define MEMBER_PASTE30(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE29(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29)
#define MEMBER_PASTE31(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE30(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30)
#define MEMBER_PASTE32(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE31(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31)
#define MEMBER_PASTE33(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE32(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32)
#define MEMBER_PASTE34(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE33(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33)
#define MEMBER_PASTE35(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE34(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34)
#define MEMBER_PASTE36(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE35(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35)
#define MEMBER_PASTE37(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE36(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36)
#define MEMBER_PASTE38(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE37(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37)
#define MEMBER_PASTE39(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE38(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38)
#define MEMBER_PASTE40(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39) \
MEMBER_PASTE2(func, v1) MEMBER_PASTE39(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39)
#define  MEMBERS(...)  MEMBER_EXPAND( MEMBER_PASTE( MEMBER, __VA_ARGS__))

#define META_ADD_COMP(CLASS,...)\
entt::meta<CLASS>()\
.base<Component>()\
.type(#CLASS##_hs)\
MEMBERS(__VA_ARGS__)\
.func<&CLASS::SerializeComponent>("SerializeComponent"_hs)\
.func<&CLASS::DeserializeComponent>("DeserializeComponent"_hs)\
.func<&CLASS::AddComponent>("AddComponent"_hs);




#define META_ADD_VALUE(CLASS)\
entt::meta<CLASS>()\
.type(#CLASS##_hs)\




#define META_ADD_STRUCT(CLASS,...)\
entt::meta<CLASS>()\
.type(#CLASS##_hs)\
MEMBERS(__VA_ARGS__)\







#define E_MEMBER(NAME)\
 .data<NAME >(#NAME##_hs)\
		.prop(Reflection::Prop::Name,#NAME)
#define E_MEMBER_EXPAND( x ) x
#define E_MEMBER_EXPAND(x) x
#define E_MEMBER_GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, NAME, ...) NAME

#define E_MEMBER_PASTE(...) E_MEMBER_EXPAND(E_MEMBER_GET_MACRO(__VA_ARGS__, \
		E_MEMBER_PASTE40, \
		E_MEMBER_PASTE39, \
		E_MEMBER_PASTE38, \
		E_MEMBER_PASTE37, \
		E_MEMBER_PASTE36, \
		E_MEMBER_PASTE35, \
		E_MEMBER_PASTE34, \
		E_MEMBER_PASTE33, \
		E_MEMBER_PASTE32, \
		E_MEMBER_PASTE31, \
        E_MEMBER_PASTE30, \
        E_MEMBER_PASTE29, \
        E_MEMBER_PASTE28, \
        E_MEMBER_PASTE27, \
        E_MEMBER_PASTE26, \
        E_MEMBER_PASTE25, \
        E_MEMBER_PASTE24, \
        E_MEMBER_PASTE23, \
        E_MEMBER_PASTE22, \
        E_MEMBER_PASTE21, \
        E_MEMBER_PASTE20, \
        E_MEMBER_PASTE19, \
        E_MEMBER_PASTE18, \
        E_MEMBER_PASTE17, \
        E_MEMBER_PASTE16, \
        E_MEMBER_PASTE15, \
        E_MEMBER_PASTE14, \
        E_MEMBER_PASTE13, \
        E_MEMBER_PASTE12, \
        E_MEMBER_PASTE11, \
        E_MEMBER_PASTE10, \
        E_MEMBER_PASTE9, \
        E_MEMBER_PASTE8, \
        E_MEMBER_PASTE7, \
        E_MEMBER_PASTE6, \
        E_MEMBER_PASTE5, \
        E_MEMBER_PASTE4, \
        E_MEMBER_PASTE3, \
        E_MEMBER_PASTE2, \
        E_MEMBER_PASTE1)(__VA_ARGS__))

#define E_MEMBER_PASTE2(func, v1) \
func(v1)
#define E_MEMBER_PASTE3(func, v1, v2) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE2(func, v2)
#define E_MEMBER_PASTE4(func, v1, v2, v3) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE3(func, v2, v3)
#define E_MEMBER_PASTE5(func, v1, v2, v3, v4) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE4(func, v2, v3, v4)
#define E_MEMBER_PASTE6(func, v1, v2, v3, v4, v5) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE5(func, v2, v3, v4, v5)
#define E_MEMBER_PASTE7(func, v1, v2, v3, v4, v5, v6) \
 E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE6(func, v2, v3, v4, v5, v6)
#define E_MEMBER_PASTE8(func, v1, v2, v3, v4, v5, v6, v7) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE7(func, v2, v3, v4, v5, v6, v7)
#define E_MEMBER_PASTE9(func, v1, v2, v3, v4, v5, v6, v7, v8) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE8(func, v2, v3, v4, v5, v6, v7, v8)
#define E_MEMBER_PASTE10(func, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE9(func, v2, v3, v4, v5, v6, v7, v8, v9)
#define E_MEMBER_PASTE11(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
 E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE10(func, v2, v3, v4, v5, v6, v7, v8, v9, v10)
#define E_MEMBER_PASTE12(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE11(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)
#define E_MEMBER_PASTE13(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE12(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12)
#define E_MEMBER_PASTE14(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE13(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13)
#define E_MEMBER_PASTE15(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE14(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14)
#define E_MEMBER_PASTE16(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE15(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15)
#define E_MEMBER_PASTE17(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE16(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16)
#define E_MEMBER_PASTE18(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE17(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17)
#define E_MEMBER_PASTE19(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE18(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18)
#define E_MEMBER_PASTE20(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE19(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19)
#define E_MEMBER_PASTE21(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE20(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20)
#define E_MEMBER_PASTE22(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE21(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21)
#define E_MEMBER_PASTE23(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22) \
 E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE22(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22)
#define E_MEMBER_PASTE24(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE23(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23)
#define E_MEMBER_PASTE25(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE24(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24)
#define E_MEMBER_PASTE26(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25) \
 E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE25(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25)
#define E_MEMBER_PASTE27(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26) \
 E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE26(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26)
#define E_MEMBER_PASTE28(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE27(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27)
#define E_MEMBER_PASTE29(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE28(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28)
#define E_MEMBER_PASTE30(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29) \
E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE29(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29)
#define E_MEMBER_PASTE31(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30) \
    E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE30(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30)
#define E_MEMBER_PASTE32(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31) \
    E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE31(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31)
#define E_MEMBER_PASTE33(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32) \
    E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE32(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32)
#define E_MEMBER_PASTE34(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33) \
    E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE33(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33)
#define E_MEMBER_PASTE35(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34) \
    E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE34(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34)
#define E_MEMBER_PASTE36(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35) \
    E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE35(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35)
#define E_MEMBER_PASTE37(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36) \
    E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE36(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36)
#define E_MEMBER_PASTE38(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37) \
    E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE37(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37)
#define E_MEMBER_PASTE39(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38) \
    E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE38(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38)
#define E_MEMBER_PASTE40(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39) \
    E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE39(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39)

#define  E_MEMBERS(...)  E_MEMBER_EXPAND( E_MEMBER_PASTE( E_MEMBER, __VA_ARGS__))
#define META_ADD_ENUMCLASS(CLASS,...)\
entt::meta<CLASS>()\
.prop(Reflection::Prop::Name,#CLASS)\
.conv<std::underlying_type_t<CLASS>>()\
E_MEMBERS(__VA_ARGS__)\







#pragma endregion







namespace Reflection
{
	namespace Prop
	{
		// 함수, 변수 이름
		constexpr static entt::hashed_string Name = entt::hashed_string("name");

		// 주석
		constexpr static entt::hashed_string Comment = entt::hashed_string("comment");

		// 상대경로로 저장 설정
		constexpr static entt::hashed_string RelativePath = entt::hashed_string("relative_path");

		// 읽기 전용
		constexpr static entt::hashed_string ReadOnly = entt::hashed_string("read_only");

		// 리소스 드래그 드랍 설정 
		// value를 ".mp3/.wav"로 설정하면 mp3, wav 파일을 드래그 드랍 받기 가능합니다 
		constexpr static entt::hashed_string DragDrop = entt::hashed_string("drag_drop");

		// Plain Old Data Structure 순수한 변수들의 집합
		constexpr static entt::hashed_string POD = entt::hashed_string("POD");

		// ??
		constexpr static entt::hashed_string Label = entt::hashed_string("label");
	}

	template<typename T>
	static entt::id_type GetTypeID() {
		static entt::id_type typeID = entt::resolve<T>().id();
		return typeID;
	}

	template<typename T>
	bool IsSameType(entt::id_type temp)
	{
		return temp == GetTypeID<T>();
	}
	// Static cache for enum members to avoid recomputation
	template<typename T>
	static std::map<int, entt::meta_data>  GetEnumMap()
	{
		static std::map<int, entt::meta_data> enumMap{};
		// eunmMember string table 생성

		if (enumMap.empty()) {
			auto metaType2 = entt::resolve(Reflection::GetTypeID<T>());
			for (auto [id, metaData] : metaType2.data()) {
				entt::meta_any any = metaData.get({});
				if (any.allow_cast<int>()) {
					int memberInt = any.cast<int>();
					enumMap[memberInt] = metaData;
				}
			}
			assert(!enumMap.empty());
		}

		return enumMap;
	}



	inline std::string GetName_Class(const entt::meta_type& type)
	{
		std::string_view name = type.info().name();

		// Find the position of the first space
		size_t space_pos = name.find(' ');

		// If a space is found, remove the prefix up to and including the space
		if (space_pos != std::string_view::npos) {
			name.remove_prefix(space_pos + 1);
		}

		return std::string(name);
	}


	inline std::string RemoveClassName(const std::string& fullName)
	{
		// "::" 이후의 부분을 찾기 위해 find를 사용합니다.
		size_t pos = fullName.find("::");

		// "::"을 찾지 못했을 경우 그대로 반환합니다.
		if (pos == std::string::npos) {
			return fullName;
		}

		// "::" 이후의 문자열을 반환합니다.
		return fullName.substr(pos + 2); // "::"의 길이가 2이므로 pos + 2로 이후의 문자열을 가져옵니다.
	}

	inline std::string GetName(const entt::meta_data& metaData)
	{
		auto prop = metaData.prop(Prop::Name);
		if (!prop)
			return {};
		std::string fullName =prop.value().cast<const char*>();
		return RemoveClassName(fullName);
	}

	inline std::string GetName(const entt::meta_type& metaType)
	{
		auto prop = metaType.prop(Prop::Name);
		if (!prop)
			return {};
		std::string fullName = prop.value().cast<const char*>();
		return RemoveClassName(fullName);

	}

	

}

