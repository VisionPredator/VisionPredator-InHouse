#pragma once


#pragma region STL
#include <vector>
#include <filesystem>
#include <iostream>
#include <string>
#pragma  endregion STL

#pragma region assimp
#include "../assimp/scene.h"
#include "../assimp/Importer.hpp"
#include "../assimp/postprocess.h"
#pragma endregion assimp


//#include "Device.h"
#include "Object.h"
#include "VertexData.h"



class ResourceManager;


enum class Filter
{
	STATIC = 0,
	SKINNING,

	END
};

class ModelLoader
{
public:
	ModelLoader(ResourceManager* manager);
	~ModelLoader();

	void Initialize();
	bool LoadModel(std::string filename, Filter filter);
	void ProcessSceneData(std::string name, const aiScene* scene, Filter filter);


private:

	std::vector<const aiScene*> m_SceneList;
	void ProcessMesh(ModelData* Model, aiMesh* mesh, unsigned int index, Filter filter);
	void ProcessMaterials(ModelData* Model, aiMaterial* material);
	void ProcessBoneNodeMapping(ModelData* Model);
	void ProcessAnimation(ModelData* Model, aiAnimation* animation);

	void ProcessVertexBuffer(std::vector<SkinningVertex>& buffer, aiMesh* curMesh, unsigned int index);
	void ProcessVertexBuffer(std::vector<TextureVertex>& buffer, aiMesh* curMesh, unsigned int index);

	void ProcessIndexBuffer(std::vector<UINT>& buffer, aiFace* curFace);
	void ProcessNode(Node* parents, Node* ob_node, aiNode* node, std::vector<Mesh*>& meshes);

	void ProcessBoneMapping(std::vector<SkinningVertex>& buffer, aiMesh* curAiMesh, Mesh* curMesh);
	Node* FindNode(std::wstring nodename, Node* RootNode);

	Device* m_device;

	ResourceManager* m_ResourceManager;

	std::map<std::wstring, int> BoneMapping;

private:

};
