#include "pch.h"
#include "ModelData.h"

ModelData::~ModelData()
{

}

void ModelData::Release()
{
	for (auto& mesh : m_Meshes)
	{
		mesh.reset();
	}


	for (auto& material : m_Materials)
	{
		material.reset();
	}


	for (auto& ani : m_Animations)
	{
		ani.reset();
	}
}
