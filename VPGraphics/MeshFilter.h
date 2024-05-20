#pragma once
enum class MeshFilter
{

	Axis = 0,
	Box = 1,
	Grid,
	TextureBox,
	LoadModel,
	Skinning,

	None
};

enum class PassState
{
	Base = 0,
	Texture,
	Skinning,

	End
};