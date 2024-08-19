#pragma once


#pragma region Meta Member 등록

///Class 기반
#define MEMBER(NAME)\
 .data<& NAME >(#NAME##_hs)\
		.prop(Reflection::Prop::Name,#NAME)
#define MEMBER_EXPAND( x ) x
#define MEMBER_GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10,_11,_12,_13,_14,_15,_16,_17, NAME,...) NAME
#define MEMBER_PASTE(...) MEMBER_EXPAND(MEMBER_GET_MACRO(__VA_ARGS__, \
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
#define MEMBER_PASTE2(func, v1) func(v1)
#define MEMBER_PASTE3(func, v1, v2) MEMBER_PASTE2(func, v1) MEMBER_PASTE2(func, v2)
#define MEMBER_PASTE4(func, v1, v2, v3) MEMBER_PASTE2(func, v1) MEMBER_PASTE3(func, v2, v3)
#define MEMBER_PASTE5(func, v1, v2, v3, v4) MEMBER_PASTE2(func, v1) MEMBER_PASTE4(func, v2, v3, v4)
#define MEMBER_PASTE6(func, v1, v2, v3, v4, v5) MEMBER_PASTE2(func, v1) MEMBER_PASTE5(func, v2, v3, v4, v5)
#define MEMBER_PASTE7(func, v1, v2, v3, v4, v5, v6) MEMBER_PASTE2(func, v1) MEMBER_PASTE6(func, v2, v3, v4, v5, v6)
#define MEMBER_PASTE8(func, v1, v2, v3, v4, v5, v6, v7) MEMBER_PASTE2(func, v1) MEMBER_PASTE7(func, v2, v3, v4, v5, v6, v7)
#define MEMBER_PASTE9(func, v1, v2, v3, v4, v5, v6, v7, v8) MEMBER_PASTE2(func, v1) MEMBER_PASTE8(func, v2, v3, v4, v5, v6, v7, v8)
#define MEMBER_PASTE10(func, v1, v2, v3, v4, v5, v6, v7, v8, v9) MEMBER_PASTE2(func, v1) MEMBER_PASTE9(func, v2, v3, v4, v5, v6, v7, v8, v9)
#define MEMBER_PASTE11(func, v1, v2, v3, v4, v5, v6, v7, v8, v9,v10) MEMBER_PASTE2(func, v1) MEMBER_PASTE10(func, v2, v3, v4, v5, v6, v7, v8, v9,v10)
#define MEMBER_PASTE12(func, v1, v2, v3, v4, v5, v6, v7, v8, v9,v10,v11) MEMBER_PASTE2(func, v1) MEMBER_PASTE11(func, v2, v3, v4, v5, v6, v7, v8, v9,v10,v11)
#define MEMBER_PASTE13(func, v1, v2, v3, v4, v5, v6, v7, v8, v9,v10,v11,v12) MEMBER_PASTE2(func, v1) MEMBER_PASTE12(func, v2, v3, v4, v5, v6, v7, v8, v9,v10,v11,v12)
#define MEMBER_PASTE14(func, v1, v2, v3, v4, v5, v6, v7, v8, v9,v10,v11,v12,v13) MEMBER_PASTE2(func, v1) MEMBER_PASTE12(func, v2, v3, v4, v5, v6, v7, v8, v9,v10,v11,v12,v13)
#define MEMBER_PASTE15(func, v1, v2, v3, v4, v5, v6, v7, v8, v9,v10,v11,v12,v13,v14) MEMBER_PASTE2(func, v1) MEMBER_PASTE12(func, v2, v3, v4, v5, v6, v7, v8, v9,v10,v11,v12,v13,v14)
#define MEMBER_PASTE16(func, v1, v2, v3, v4, v5, v6, v7, v8, v9,v10,v11,v12,v13,v14,v15) MEMBER_PASTE2(func, v1) MEMBER_PASTE12(func, v2, v3, v4, v5, v6, v7, v8, v9,v10,v11,v12,v13,v14,v15)
#define MEMBER_PASTE17(func, v1, v2, v3, v4, v5, v6, v7, v8, v9,v10,v11,v12,v13,v14,v15,v16) MEMBER_PASTE2(func, v1) MEMBER_PASTE12(func, v2, v3, v4, v5, v6, v7, v8, v9,v10,v11,v12,v13,v14,v15,v16)
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
#define E_MEMBER_GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME,...) NAME
#define E_MEMBER_PASTE(...) E_MEMBER_EXPAND(E_MEMBER_GET_MACRO(__VA_ARGS__, \
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
#define E_MEMBER_PASTE2(func, v1) func(v1)
#define E_MEMBER_PASTE3(func, v1, v2) E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE2(func, v2)
#define E_MEMBER_PASTE4(func, v1, v2, v3) E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE3(func, v2, v3)
#define E_MEMBER_PASTE5(func, v1, v2, v3, v4) E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE4(func, v2, v3, v4)
#define E_MEMBER_PASTE6(func, v1, v2, v3, v4, v5) E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE5(func, v2, v3, v4, v5)
#define E_MEMBER_PASTE7(func, v1, v2, v3, v4, v5, v6) E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE6(func, v2, v3, v4, v5, v6)
#define E_MEMBER_PASTE8(func, v1, v2, v3, v4, v5, v6, v7) E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE7(func, v2, v3, v4, v5, v6, v7)
#define E_MEMBER_PASTE9(func, v1, v2, v3, v4, v5, v6, v7, v8) E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE8(func, v2, v3, v4, v5, v6, v7, v8)
#define E_MEMBER_PASTE10(func, v1, v2, v3, v4, v5, v6, v7, v8, v9) E_MEMBER_PASTE2(func, v1) E_MEMBER_PASTE9(func, v2, v3, v4, v5, v6, v7, v8, v9)
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

