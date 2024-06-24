#pragma once
enum class MeshFilter
{

	Axis = 0,
	Box = 1,
	Grid,
	TextureBox,
	Static,
	Skinning,
	Circle,

	None
};

enum class PassState : unsigned int
{
	None = 0,
	Static = 1 << 0,
	Skinning = 1 << 1,
	Debug = 1 << 2,
	Deferred = 1 << 3,

	End = 99999
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