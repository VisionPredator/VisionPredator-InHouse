#pragma once
enum class MeshFilter
{

	Axis = 0,
	Box = 1,
	Grid,
	Static,
	Skinning,
	Circle,

	None
};

enum class PassState : unsigned int
{
	None = 0,
	Forward = 1 << 1,
	Debug = 1 << 2,
	Deferred = 1 << 3,
	GeoMetry = 1 << 4,

	End = 99999
};

//지금 다 섞여있는데 data를 쓰는 기능에따라 나눠서 entity id로 접근해야할듯 덩치가 너무 큼
struct RenderData
{
public:
	RenderData() :EntityID(0), Name(L""), FBX(L""), Pass(PassState::Deferred)
		, local(DirectX::SimpleMath::Matrix::Identity), world(DirectX::SimpleMath::Matrix::Identity)
		, duration(0.f), preDuration(0.f), curAnimation(L""), preAnimation(L""), isPlay(false), isChange(false)
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

		this->isChange = other.isChange;
		this->isPlay = other.isPlay;

		this->curAnimation = std::move(other.curAnimation);
		this->preAnimation = std::move(other.preAnimation);
	}

	uint32_t EntityID;
	std::wstring Name;
	std::wstring FBX;
	PassState Pass;
	MeshFilter Filter;

	DirectX::SimpleMath::Matrix world; //게임 세상의 위치
	DirectX::SimpleMath::Matrix local; //캐릭터 자체 로컬

	float duration;
	float preDuration;
	std::wstring curAnimation;
	std::wstring preAnimation;
	bool isPlay;
	bool isChange;

	DirectX::XMFLOAT4 color;
	bool useTexture;
	std::wstring textureName;
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