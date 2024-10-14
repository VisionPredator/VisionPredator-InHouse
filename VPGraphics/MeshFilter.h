#pragma once
enum class GeoMetryFilter : unsigned int
{

	Axis = 0,
	Box = 1,
	Grid,
	Frustum,

	None
};

enum class PassState : unsigned int
{
	None = 0,					// 유지	
	Geometry = 1 << 4,			// 삭제? 나중에 사라질 예정
	ObjectMask = 1 << 5,		// 유지
	End = 99999
};

//지금 다 섞여있는데 data를 쓰는 기능에따라 나눠서 entity id로 접근해야할듯 덩치가 너무 큼
struct RenderData
{
public:
	RenderData() :EntityID(0), Name(L""), FBX(L""),ModelID(0)
		, local(VPMath::Matrix::Identity), world(VPMath::Matrix::Identity)
		, duration(0.f), preDuration(0.f), isPlay(false)
		, color(), useTexture(false), textureName(L"")
	{

	}

	RenderData(RenderData& other)
	{

	}

	RenderData& operator=(RenderData& other)
	{
		this->EntityID = other.EntityID;
		this->Filter = other.Filter;

		this->Name = std::move(other.Name);
		this->FBX = std::move(other.FBX);

		this->world = other.world;
		this->local = other.local;

		this->duration = other.duration;
		this->preDuration = other.preDuration;
		this->transitionDuration = other.transitionDuration;

		this->isPlay = other.isPlay;
	}

	//필수

	uint32_t EntityID;
	std::wstring Name;
	std::wstring FBX;
	PassState Pass; 
	int ModelID;

	GeoMetryFilter Filter;
	bool isSkinned = false;
	bool isVisible = true;

	VPMath::Matrix world; //게임 세상의 위치
	VPMath::Matrix local; //캐릭터 자체 로컬

	VPMath::Vector3 rotation;

	//애니메이션
	float duration;
	float preDuration;
	bool isPlay;
	int curAni;
	int preAni;
	float transitionDuration;

	//디버그박스용 - 나중에 지울거
	DirectX::XMFLOAT4 color;
	bool useTexture;
	std::wstring textureName;

	//라이트맵 데이터
	VPMath::Vector2 offset;
	VPMath::Vector2 tiling;
	int lightmapindex;
	float scale;

	// 아웃라인 관련 변수
	VPMath::Color MaskingColor = VPMath::Color{ 0, 0, 0, 0 };
};


// 비트 연산자 오버로딩
inline PassState operator|(PassState a, PassState b) {
	return static_cast<PassState>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

inline PassState& operator|=(PassState& a, PassState b) {
	a = a | b;
	return a;
}

inline PassState operator&(PassState a, PassState b) {
	return static_cast<PassState>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

inline PassState& operator&=(PassState& a, PassState b) {
	a = a & b;
	return a;
}

inline PassState operator~(PassState a) {
	return static_cast<PassState>(~static_cast<unsigned int>(a));
}

// 비트 연산자 오버로딩
inline GeoMetryFilter operator|(GeoMetryFilter a, GeoMetryFilter b) {
	return static_cast<GeoMetryFilter>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

inline GeoMetryFilter& operator|=(GeoMetryFilter& a, GeoMetryFilter b) {
	a = a | b;
	return a;
}

inline GeoMetryFilter operator&(GeoMetryFilter a, GeoMetryFilter b) {
	return static_cast<GeoMetryFilter>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

inline GeoMetryFilter& operator&=(GeoMetryFilter& a, GeoMetryFilter b) {
	a = a & b;
	return a;
}

inline GeoMetryFilter operator~(GeoMetryFilter a) {
	return static_cast<GeoMetryFilter>(~static_cast<unsigned int>(a));
}