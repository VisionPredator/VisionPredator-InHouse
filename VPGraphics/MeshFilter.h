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
	None = 0,					// ����	
	Geometry = 1 << 4,			// ����? ���߿� ����� ����
	ObjectMask = 1 << 5,		// ����
	End = 99999
};

//���� �� �����ִµ� data�� ���� ��ɿ����� ������ entity id�� �����ؾ��ҵ� ��ġ�� �ʹ� ŭ
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

	//�ʼ�

	uint32_t EntityID;
	std::wstring Name;
	std::wstring FBX;
	PassState Pass; 
	int ModelID;

	GeoMetryFilter Filter;
	bool isSkinned = false;
	bool isVisible = true;

	VPMath::Matrix world; //���� ������ ��ġ
	VPMath::Matrix local; //ĳ���� ��ü ����

	VPMath::Vector3 rotation;

	//�ִϸ��̼�
	float duration;
	float preDuration;
	bool isPlay;
	int curAni;
	int preAni;
	float transitionDuration;

	//����׹ڽ��� - ���߿� �����
	DirectX::XMFLOAT4 color;
	bool useTexture;
	std::wstring textureName;

	//����Ʈ�� ������
	VPMath::Vector2 offset;
	VPMath::Vector2 tiling;
	int lightmapindex;
	float scale;

	// �ƿ����� ���� ����
	VPMath::Color MaskingColor = VPMath::Color{ 0, 0, 0, 0 };
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

// ��Ʈ ������ �����ε�
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