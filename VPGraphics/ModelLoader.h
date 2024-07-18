#pragma once
#include "pch.h"

#pragma region assimp
#include "scene.h"
#include "Importer.hpp"
#include "postprocess.h"
#pragma endregion assimp

#include "VertexData.h"



class Device;
class ResourceManager;
class ModelData;
class Node;
class Mesh;

enum class Filter
{
	STATIC = 0,
	SKINNING,

	END
};

class ModelLoader
{
public:
	ModelLoader(std::shared_ptr<ResourceManager> manager, std::shared_ptr<Device> device);
	~ModelLoader();

	void Initialize();
	bool LoadModel(std::string filename, Filter filter);
	void ProcessSceneData(std::string name, const aiScene* scene, Filter filter);


private:
	std::vector<const aiScene*> m_SceneList;

	void ProcessMesh(std::shared_ptr<ModelData> Model, aiMesh* mesh, unsigned int index, Filter filter);
	void ProcessMaterials(std::shared_ptr<ModelData> Model, aiMaterial* material);
	void ProcessBoneNodeMapping(std::shared_ptr<ModelData> Model);
	void ProcessAnimation(std::shared_ptr<ModelData> Model, aiAnimation* animation);

	void ProcessVertexBuffer(std::vector<SkinningVertex>& buffer, aiMesh* curMesh, unsigned int index);
	void ProcessVertexBuffer(std::vector<BaseVertex>& buffer, aiMesh* curMesh, unsigned int index);

	void ProcessIndexBuffer(std::vector<UINT>& buffer, aiFace* curFace);
	void ProcessNode(std::shared_ptr<Node> parents, std::shared_ptr<Node> ob_node, aiNode* node, std::vector<std::shared_ptr<Mesh>>& meshes);

	void ProcessBoneMapping(std::vector<SkinningVertex>& buffer, aiMesh* curAiMesh, std::shared_ptr<Mesh> curMesh);
	std::shared_ptr<Node>FindNode(std::wstring nodename, std::shared_ptr<Node> RootNode);


	std::weak_ptr<Device> m_Device;
	std::weak_ptr<ResourceManager> m_ResourceManager;

	std::map<std::wstring, int> BoneMapping;

private:

};
