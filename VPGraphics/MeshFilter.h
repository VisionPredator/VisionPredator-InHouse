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
	Foward = 1 << 1,
	Debug = 1 << 2,
	Deferred = 1 << 3,

	End = 99999
};


struct RenderData
{
public:
	RenderData() :EntityID(0), Name(L""), FBX(L""), Pass(PassState::Foward)
		,local(DirectX::SimpleMath::Matrix::Identity), world(DirectX::SimpleMath::Matrix::Identity)
		,duration(0.f), curAnimation(L""), isPlay(false)
	{

	}

	uint32_t EntityID;
	std::wstring Name;
	std::wstring FBX;
	PassState Pass;
	MeshFilter Filter;

	DirectX::SimpleMath::Matrix world; //���� ������ ��ġ
	DirectX::SimpleMath::Matrix local; //ĳ���� ��ü ����


	float duration;
	std::wstring curAnimation;
	bool isPlay;
};


// ��Ʈ ������ �����ε�
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