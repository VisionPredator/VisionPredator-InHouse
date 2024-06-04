#pragma once
enum class MeshFilter
{

	Axis = 0,
	Box = 1,
	Grid,
	TextureBox,
	Static,
	Skinning,

	None
};

enum class PassState : unsigned int
{
	None = 0,
	Static = 1 << 0,
	Texture = 1 << 1,
	Skinning = 1 << 2,

	End
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