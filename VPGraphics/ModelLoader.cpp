#include "pch.h"
#include "ModelLoader.h"

#include "DirectXHelpers.h"
#include "ResourceManager.h"

#include "ModelData.h"
#include "Material.h"
#include "Node.h"
#include "Mesh.h"
#include "Animation.h"

#include "ShaderResourceView.h"
#include "ConstantBuffer.h"

#include "Desc.h"
#include "CBuffer.h"

#include "pbrmaterial.h"


ModelLoader::ModelLoader(std::shared_ptr<ResourceManager> manager, std::shared_ptr<Device> device) : m_Device(device), m_ResourceManager(manager)
{

}

ModelLoader::~ModelLoader()
{
}

void ModelLoader::Initialize()
{
	//여기서 리소스 많이 들어가면 dt ㅈㄴ 늘어나서 애니메이션이 터짐 - dt값이 튀어서 - 늘어날때마다 매번 함수 넣어줄 수는 없자나
	LoadModel("Flair.fbx", Filter::SKINNING);
	LoadModel("cerberus.fbx", Filter::STATIC);
	LoadModel("Cube.fbx", Filter::STATIC);
}

bool ModelLoader::LoadModel(std::string filename, Filter filter)
{
	//std::filesystem::path path = ToWString(std::string(filename));
	
#ifdef _DEBUG
	const std::string filePath = "..\\..\\..\\Resource\\FBX\\" + filename;
#else
	const std::string filePath = "..\\Data\\FBX\\" + filename;
#endif
	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);    // $assimp_fbx$ 노드 생성안함

	//플래그를 모델을 어떻게 읽을지 정해야하니까 매번 어케 읽을지 셋팅해주는게 좋아임
	unsigned int importFlags{};


	switch (filter)
	{
		case Filter::STATIC:
			importFlags = aiProcess_Triangulate | // 삼각형으로 변환
				aiProcess_GenNormals |	// 노말 생성/
				aiProcess_GenUVCoords |		// UV 생성
				aiProcess_CalcTangentSpace |  // 탄젠트 생성			
				aiProcess_GenBoundingBoxes | // 바운딩 박스 생성
				aiProcess_PreTransformVertices | // 노드의 변환행렬을 적용한 버텍스 생성 /주의 이 단계에서는 애니메이션이 제거됩니다.
				aiProcess_ConvertToLeftHanded;	// 왼손 좌표계로 변환
			break;
		case Filter::SKINNING:
			importFlags = aiProcess_Triangulate | // 삼각형으로 변환
				aiProcess_GenNormals |	// 노말 생성/
				aiProcess_GenUVCoords |		// UV 생성
				aiProcess_CalcTangentSpace |  // 탄젠트 생성			
				aiProcess_GenBoundingBoxes | // 바운딩 박스 생성
				//aiProcess_LimitBoneWeights | // 본에 영향을 받는 정점의 최대 개수를 4개로 제한 - 일부 메쉬는 이거에 영향을 받아 뒤틀린다.. 이거 처리가 필요하다
				/*aiProcess_FlipUVs|
				aiProcess_FlipWindingOrder|*/
				aiProcess_ConvertToLeftHanded;	// 왼손 좌표계로 변환
			break;
		case Filter::END:
			break;
		default:
			break;
	}



	const aiScene* scene = importer.ReadFile(filePath, importFlags);
	if (!scene)
	{
		std::wstring wfilename;
		wfilename.assign(filename.begin(),filename.end());

		wfilename = L"Error loading files : " + wfilename;
		LPCWSTR name = wfilename.c_str();
		MessageBox(0, name, 0, 0);
		return false;
	}

	ProcessSceneData(filename, scene, filter);

	importer.FreeScene();
	return true;

}

//데이터 가공이 필요하다
void ModelLoader::ProcessSceneData(std::string name, const aiScene* scene, Filter filter)
{
	std::shared_ptr<ModelData> newData = std::make_shared<ModelData>();
	newData->m_name.assign(name.begin(), name.end());
	newData->m_RootNode = std::make_shared<Node>();
	newData->RS = m_ResourceManager.lock()->Get<RenderState>(L"Solid");

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		ProcessMesh(newData, (scene->mMeshes[i]), i, filter);
	}

	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		ProcessMaterials(newData, scene->mMaterials[i]);
	}

	if (Filter::SKINNING == filter)
	{
		ProcessNode(nullptr, newData->m_RootNode, scene->mRootNode, newData->m_Meshes);
		ProcessBoneNodeMapping(newData);
	}

	for (unsigned int i = 0; i < scene->mNumAnimations; i++)
	{
		ProcessAnimation(newData, scene->mAnimations[i]);
	}

	std::wstring key;
	key.assign(name.begin()/*+ 12*/, name.end());

	m_ResourceManager.lock()->Add<ModelData>(key, newData);
}

//메쉬 저장
void ModelLoader::ProcessMesh(std::shared_ptr<ModelData> Model, aiMesh* mesh, unsigned int index, Filter filter)
{
	std::wstring str_index = std::to_wstring(index);

	aiMesh* curMesh = mesh;

	std::shared_ptr<Mesh> newMesh;
	switch (filter)
	{
		case Filter::STATIC:
			newMesh = std::make_shared<StaticMesh>();
			break;
		case Filter::SKINNING:
			newMesh = std::make_shared<SkinnedMesh>();
			break;
	}

	newMesh->m_primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	newMesh->m_material = mesh->mMaterialIndex;

	///메쉬에따라 읽는방식과 세부 설정을 따로 둬야할듯
	std::vector<BaseVertex> TextureVertices;
	std::vector<SkinningVertex> SkinningVertices;

	D3D11_BUFFER_DESC desc = BufferDESC::Vertex::Default;
	D3D11_SUBRESOURCE_DATA data;

	std::vector<UINT> indexbuffer;
	for (unsigned int i = 0; i < curMesh->mNumFaces; i++)
	{
		ProcessIndexBuffer(indexbuffer, curMesh->mFaces + i);
	}

	D3D11_BUFFER_DESC idesc = BufferDESC::Index::Default;
	idesc.ByteWidth = sizeof(UINT) * curMesh->mNumFaces * 3;

	D3D11_SUBRESOURCE_DATA idata;
	idata.pSysMem = &(indexbuffer[0]);

	//face 당 3개의 인덱스 사용 - 삼각형으로 면을 구성하기때문
	newMesh->m_IB = m_ResourceManager.lock()->Create<IndexBuffer>(Model->m_name + L"_" + str_index + L"_IB", idesc, idata, curMesh->mNumFaces * 3);

	switch (filter)
	{
		case Filter::STATIC:
			for (unsigned int i = 0; i < curMesh->mNumVertices; i++)
			{
				ProcessVertexBuffer(TextureVertices, curMesh, i);
			}
			desc.ByteWidth = sizeof(BaseVertex) * curMesh->mNumVertices;
			data.pSysMem = &(TextureVertices[0]);
			newMesh->m_VB = m_ResourceManager.lock()->Create<VertexBuffer>(Model->m_name + L"_" + str_index + L"_VB", desc, data, sizeof(BaseVertex));
			break;

		case Filter::SKINNING:
		{

			for (unsigned int i = 0; i < curMesh->mNumVertices; i++)
			{
				ProcessVertexBuffer(SkinningVertices, curMesh, i);
			}

			std::shared_ptr<SkinnedMesh> newSkin = std::dynamic_pointer_cast<SkinnedMesh>(newMesh);

			//process bone
			for (unsigned int i = 0; i < mesh->mNumBones; i++)
			{
				aiBone* curAiBone = mesh->mBones[i];

				std::shared_ptr<Bone> curBone = std::make_shared<Bone>();
				std::string name = curAiBone->mName.C_Str();
				curBone->name.assign(name.begin(), name.end());
				curBone->Boneindex = i;

				newSkin->m_BoneData.push_back(curBone);

				DirectX::XMFLOAT4X4 temp;

				for (int j = 0; j < static_cast<int>(curAiBone->mNumWeights); j++)
				{
					curBone->weights.push_back(curAiBone->mWeights[j].mWeight);
					curBone->vertexids.push_back(curAiBone->mWeights[j].mVertexId);
				}

				temp._11 = curAiBone->mOffsetMatrix.a1;
				temp._12 = curAiBone->mOffsetMatrix.a2;
				temp._13 = curAiBone->mOffsetMatrix.a3;
				temp._14 = curAiBone->mOffsetMatrix.a4;

				temp._21 = curAiBone->mOffsetMatrix.b1;
				temp._22 = curAiBone->mOffsetMatrix.b2;
				temp._23 = curAiBone->mOffsetMatrix.b3;
				temp._24 = curAiBone->mOffsetMatrix.b4;

				temp._31 = curAiBone->mOffsetMatrix.c1;
				temp._32 = curAiBone->mOffsetMatrix.c2;
				temp._33 = curAiBone->mOffsetMatrix.c3;
				temp._34 = curAiBone->mOffsetMatrix.c4;

				temp._41 = curAiBone->mOffsetMatrix.d1;
				temp._42 = curAiBone->mOffsetMatrix.d2;
				temp._43 = curAiBone->mOffsetMatrix.d3;
				temp._44 = curAiBone->mOffsetMatrix.d4;

				curBone->offsetMatrix = DirectX::XMLoadFloat4x4(&temp);
				curBone->offsetMatrixInverse = curBone->offsetMatrix.Invert();
			}

			ProcessBoneMapping(SkinningVertices, curMesh, newMesh);
			desc.ByteWidth = sizeof(SkinningVertex) * curMesh->mNumVertices;
			data.pSysMem = &(SkinningVertices[0]);
			newMesh->m_VB = m_ResourceManager.lock()->Create<VertexBuffer>(Model->m_name + L"_" + str_index + L"_VB", desc, data, sizeof(SkinningVertex));
		}
		break;
		case Filter::END:
			break;
		default:
			break;
	}

	Model->m_Meshes.push_back(newMesh);
}
//머테리얼 저장
void ModelLoader::ProcessMaterials(std::shared_ptr<ModelData> Model, aiMaterial* material)
{
	std::shared_ptr<Material> newMaterial = std::make_shared<Material>(m_Device.lock());

	// Diffuse
	std::wstring basePath = L"../../../Resource/Texture/";
	std::filesystem::path path;
	std::wstring finalPath;
	std::string name = material->GetName().C_Str();

	std::vector<std::pair<aiTextureType, std::string>> textureProperties;

	int max_size = aiTextureType_TRANSMISSION + 1;
	textureProperties.resize(max_size);

	for (size_t i = 0; i < max_size; i++)
	{
		textureProperties[i].first = (aiTextureType)i;
		aiString texturePath;
		if (AI_SUCCESS == material->GetTexture((aiTextureType)i, 0, &texturePath))
		{
			path = std::string(texturePath.C_Str());
			textureProperties[i].second = path.filename().string();
		}
	}

	aiColor3D color(1.f, 1.f, 1.f);
	if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color))
	{
		//m_Color ={ color.r, color.g, color.b , 1};
	}

	///Albedo
	path = (textureProperties[aiTextureType_DIFFUSE].second);
	if (!path.empty())
	{
		finalPath = basePath + path.filename().wstring();
		newMaterial->AlbeoPath = finalPath;

		newMaterial->m_AlbedoSRV = m_ResourceManager.lock()->Create<ShaderResourceView>(finalPath, path);
		newMaterial->m_Data.useAMRO.x += 1;

	}
	else
	{
		finalPath = basePath + L"base.png";
		newMaterial->AlbeoPath = finalPath;
		newMaterial->m_AlbedoSRV = m_ResourceManager.lock()->Create<ShaderResourceView>(L"../Resource/Texture/base.png", L"base.png");
	}

	///Normal
	path = (textureProperties[aiTextureType_NORMALS].second);
	if (!path.empty())
	{
		finalPath = basePath + path.filename().wstring();
		newMaterial->NormalPath = finalPath;
		newMaterial->m_NormalSRV = m_ResourceManager.lock()->Create<ShaderResourceView>(finalPath, path);
		newMaterial->m_Data.useNE.x += true;

		//m_pNormal = ResourceManager::Instance->CreateTextureResource(finalPath);
		//m_MaterialMapFlags |= MaterialMapFlags::NORMAL;
	}
	else
	{
		finalPath = basePath + path.filename().wstring();
		newMaterial->NormalPath = finalPath;
		newMaterial->m_NormalSRV = m_ResourceManager.lock()->Create<ShaderResourceView>(L"../Resource/base.png", L"../Resource/base.png");

	}

	path = (textureProperties[aiTextureType_SPECULAR].second);
	if (!path.empty())
	{
		finalPath = basePath + path.filename().wstring();
		//newMaterial->m_SpecularFilePath = finalPath;
		//newMaterial->m_SpecularSRV->Load(finalPath);
		//m_pSpecular = ResourceManager::Instance->CreateTextureResource(finalPath);
		//m_MaterialMapFlags |= MaterialMapFlags::SPECULAR;
	}

	path = (textureProperties[aiTextureType_EMISSIVE].second);
	if (!path.empty())
	{
		finalPath = basePath + path.filename().wstring();
		newMaterial->EmissivePath = finalPath;
		newMaterial->m_EmissiveSRV = m_ResourceManager.lock()->Create<ShaderResourceView>(finalPath, path);
		newMaterial->m_Data.useNE.y += true;
	}

	path = (textureProperties[aiTextureType_OPACITY].second);
	if (!path.empty())
	{
		finalPath = basePath + path.filename().wstring();
		//m_pOpacity = ResourceManager::Instance->CreateTextureResource(finalPath);
		//m_MaterialMapFlags |= MaterialMapFlags::OPACITY;
	}

	///Metalic
	path = (textureProperties[aiTextureType_METALNESS].second);
	if (!path.empty())
	{
		finalPath = basePath + path.filename().wstring();
		newMaterial->MetalicPath = finalPath;
		newMaterial->m_MetalicSRV = m_ResourceManager.lock()->Create<ShaderResourceView>(finalPath, path);
		newMaterial->m_Data.useAMRO.y += 1;

	}

	///Roughness
	path = (textureProperties[aiTextureType_SHININESS].second);
	if (!path.empty())
	{
		finalPath = basePath + path.filename().wstring();
		newMaterial->RoughnessPath = finalPath;
		newMaterial->m_RoughnessSRV = m_ResourceManager.lock()->Create<ShaderResourceView>(finalPath, path);
		newMaterial->m_Data.useAMRO.z += 1;

	}

	///AO
	path = (textureProperties[aiTextureType_AMBIENT_OCCLUSION].second);
	if (!path.empty())
	{
		finalPath = basePath + path.filename().wstring();
		newMaterial->AlbeoPath = finalPath;
		newMaterial->m_AlbedoSRV = m_ResourceManager.lock()->Create<ShaderResourceView>(finalPath, path);
		newMaterial->m_Data.useAMRO.w += 1;
	}

	Model->m_Materials.push_back(newMaterial);
}
void ModelLoader::ProcessBoneNodeMapping(std::shared_ptr<ModelData> Model)
{
	for (auto& mesh : Model->m_Meshes)
	{
		if (mesh->IsSkinned())
		{
			std::shared_ptr<SkinnedMesh> curMesh = dynamic_pointer_cast<SkinnedMesh>(mesh);
			for (auto& bone : curMesh->m_BoneData)
			{
				std::shared_ptr<Node> temp = FindNode(bone->name, Model->m_RootNode);
				if (temp != nullptr)
				{
					bone->node = temp;
					temp->m_Bones.push_back(bone);
				}
			}
		}
	}
}
void ModelLoader::ProcessAnimation(std::shared_ptr<ModelData> Model, aiAnimation* animation)
{
	std::shared_ptr<Animation> _Animation = std::make_shared<Animation>();
	_Animation->m_Duration = animation->mDuration;
	_Animation->m_TickFrame = animation->mTicksPerSecond;

	for (unsigned int i = 0; i < animation->mNumChannels; i++)
	{
		_Animation->m_Channels.push_back(std::make_shared<Channel>());

		std::shared_ptr<Channel> ob_Channel = _Animation->m_Channels.back();
		aiNodeAnim* curChannel = animation->mChannels[i];

		const char* name = curChannel->mNodeName.C_Str();
		ob_Channel->nodename = std::wstring(name, name + curChannel->mNodeName.length);
		ob_Channel->node = FindNode(ob_Channel->nodename, Model->m_RootNode);

		double prevTime = 0;

		for (unsigned int j = 0; j < curChannel->mNumPositionKeys; j++)
		{
			aiVectorKey curAnimation = curChannel->mPositionKeys[j];
			//if (curAnimation.mTime - prevTime > 0.05)
			{
				ob_Channel->positionkey.push_back(std::make_shared<Key>());

				std::shared_ptr<Key> curKey = ob_Channel->positionkey.back();

				curKey->time = curAnimation.mTime;

				curKey->value.x = curAnimation.mValue.x;
				curKey->value.y = curAnimation.mValue.y;
				curKey->value.z = curAnimation.mValue.z;

			}
			prevTime = curAnimation.mTime;
		}

		prevTime = 0;

		for (unsigned int j = 0; j < curChannel->mNumScalingKeys; j++)
		{
			aiVectorKey curAnimation = curChannel->mScalingKeys[j];
			//if (curAnimation.mTime - prevTime > 0.05)
			{
				ob_Channel->scalingkey.push_back(std::make_shared<Key>());

				std::shared_ptr<Key> curKey = ob_Channel->scalingkey.back();

				curKey->time = curAnimation.mTime;

				curKey->value.x = curAnimation.mValue.x;
				curKey->value.y = curAnimation.mValue.y;
				curKey->value.z = curAnimation.mValue.z;
			}
			prevTime = curAnimation.mTime;

		}

		prevTime = 0;
		for (unsigned int j = 0; j < curChannel->mNumRotationKeys; j++)
		{
			aiQuatKey curAnimation = curChannel->mRotationKeys[j];
			//if (curAnimation.mTime - prevTime > 0.05)
			{
				ob_Channel->rotationkey.push_back(std::make_shared<Key>());

				std::shared_ptr<Key> curKey = ob_Channel->rotationkey.back();

				curKey->time = curAnimation.mTime;

				curKey->rotation.x = curAnimation.mValue.x;
				curKey->rotation.y = curAnimation.mValue.y;
				curKey->rotation.z = curAnimation.mValue.z;
				curKey->rotation.w = curAnimation.mValue.w;
			}
			prevTime = curAnimation.mTime;
		}

		//
		for (unsigned int j = 0; j < curChannel->mNumRotationKeys; j++)
		{
			DirectX::SimpleMath::Matrix rotation{};
			DirectX::SimpleMath::Matrix translate{};
			DirectX::SimpleMath::Matrix scale{};

			translate = DirectX::SimpleMath::Matrix::CreateTranslation(ob_Channel->positionkey[j]->value);

			scale = DirectX::SimpleMath::Matrix::CreateScale(ob_Channel->scalingkey[j]->value);

			scale = DirectX::SimpleMath::Matrix::CreateFromQuaternion(ob_Channel->rotationkey[j]->rotation);

			DirectX::SimpleMath::Matrix total = scale * rotation * translate;

			ob_Channel->totals.push_back(total);
		}
		
	}

	Model->m_Animations.push_back(_Animation);
}


//버텍스버퍼 저장
void ModelLoader::ProcessVertexBuffer(std::vector<SkinningVertex>& buffer, aiMesh* curMesh, unsigned int index)
{
	SkinningVertex vertex;

	vertex.pos.x = curMesh->mVertices[index].x;
	vertex.pos.y = curMesh->mVertices[index].y;
	vertex.pos.z = curMesh->mVertices[index].z;
	vertex.pos.w = 1;

	vertex.color = { 0.7f,0.7f, 0.7f, 1.0f };

	vertex.normal.x = curMesh->mNormals[index].x;
	vertex.normal.y = curMesh->mNormals[index].y;
	vertex.normal.z = curMesh->mNormals[index].z;

	vertex.tangent.x = curMesh->mTangents[index].x;
	vertex.tangent.y = curMesh->mTangents[index].y;
	vertex.tangent.z = curMesh->mTangents[index].z;

	vertex.bitangent.x = curMesh->mBitangents[index].x;
	vertex.bitangent.y = curMesh->mBitangents[index].y;
	vertex.bitangent.z = curMesh->mBitangents[index].z;

	vertex.TexCord.x = curMesh->mTextureCoords[0][index].x;
	vertex.TexCord.y = curMesh->mTextureCoords[0][index].y;

	//bone data size
	for (int i = 0; i < 8; i++)
	{
		vertex.BoneIndices[i] = 0.f;
		vertex.BoneWeights[i] = 0.f;
	}
	buffer.push_back(vertex);
}
void ModelLoader::ProcessVertexBuffer(std::vector<BaseVertex>& buffer, aiMesh* curMesh, unsigned int index)
{
	BaseVertex vertex;

	vertex.pos.x = curMesh->mVertices[index].x;
	vertex.pos.y = curMesh->mVertices[index].y;
	vertex.pos.z = curMesh->mVertices[index].z;
	vertex.pos.w = 1;

	vertex.color = { 0.7f,0.7f, 0.7f, 1.0f };

	vertex.normal.x = curMesh->mNormals[index].x;
	vertex.normal.y = curMesh->mNormals[index].y;
	vertex.normal.z = curMesh->mNormals[index].z;

	vertex.tangent.x = curMesh->mTangents[index].x;
	vertex.tangent.y = curMesh->mTangents[index].y;
	vertex.tangent.z = curMesh->mTangents[index].z;

	vertex.bitangent.x = curMesh->mBitangents[index].x;
	vertex.bitangent.y = curMesh->mBitangents[index].y;
	vertex.bitangent.z = curMesh->mBitangents[index].z;

	vertex.TexCord.x = curMesh->mTextureCoords[0][index].x;
	vertex.TexCord.y = curMesh->mTextureCoords[0][index].y;

	buffer.push_back(vertex);
}

//인덱스버퍼 저장
void ModelLoader::ProcessIndexBuffer(std::vector<UINT>& buffer, aiFace* curFace)
{
	for (unsigned int i = 0; i < curFace->mNumIndices; i++)
	{
		buffer.push_back(curFace->mIndices[i]);
	}
}

void ModelLoader::ProcessNode(std::shared_ptr<Node> parents, std::shared_ptr<Node> ob_node, aiNode* node, std::vector<std::shared_ptr<Mesh>>& meshes)
{
	std::string Name = node->mName.C_Str();
	ob_node->name.assign(Name.begin(), Name.end());

	//local
	DirectX::XMFLOAT4X4 temp;
	temp._11 = node->mTransformation.a1;
	temp._12 = node->mTransformation.a2;
	temp._13 = node->mTransformation.a3;
	temp._14 = node->mTransformation.a4;

	temp._21 = node->mTransformation.b1;
	temp._22 = node->mTransformation.b2;
	temp._23 = node->mTransformation.b3;
	temp._24 = node->mTransformation.b4;

	temp._31 = node->mTransformation.c1;
	temp._32 = node->mTransformation.c2;
	temp._33 = node->mTransformation.c3;
	temp._34 = node->mTransformation.c4;

	temp._41 = node->mTransformation.d1;
	temp._42 = node->mTransformation.d2;
	temp._43 = node->mTransformation.d3;
	temp._44 = node->mTransformation.d4;

	ob_node->m_Local = DirectX::XMLoadFloat4x4(&temp);
	//assimp는 row major	
	/*
		X1  Y1  Z1  T1
		X2  Y2  Z2  T2
		X3  Y3  Z3  T3
		0   0   0   1

	*/


	ob_node->m_LocalInverse = ob_node->m_Local.Invert();

	if (parents != nullptr)
	{
		ob_node->HasParents = true;
		ob_node->m_Parents = parents;

		//이게 local로써 constantbuffer에 들어가야함 이게 계속 바뀌어야함 물론 원본이 아니라 밖에서 이걸 이용해 만든 것들 그러면 이걸 상수 버퍼로 만들어버려
		ob_node->m_World = (ob_node->m_Parents.lock()->m_World * ob_node->m_Local);
		ob_node->m_WorldInverse = ob_node->m_World.Invert();
	}
	else
	{
		ob_node->m_World = ob_node->m_Local;
		ob_node->m_WorldInverse = ob_node->m_World.Invert();
	}

	if (node->mNumMeshes != 0)
	{
		//ob_node->m_Meshes.resize(node->mNumMeshes);
		ob_node->index = *node->mMeshes;

		ob_node->m_Meshes.push_back(meshes[ob_node->index]);

		for (auto& mesh : ob_node->m_Meshes)
		{
			std::shared_ptr<SkinnedMesh> curMesh = std::dynamic_pointer_cast<SkinnedMesh>(mesh.lock());
			curMesh->m_node = ob_node;
		}
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ob_node->m_Childs.push_back(std::make_shared<Node>());
		ProcessNode(ob_node, ob_node->m_Childs.back(), node->mChildren[i], meshes);
	}
}

void ModelLoader::ProcessBoneMapping(std::vector<SkinningVertex>& buffer, aiMesh* curAiMesh, std::shared_ptr<Mesh> curMesh)
{

	std::shared_ptr<SkinnedMesh> curSkin = std::dynamic_pointer_cast<SkinnedMesh>(curMesh);

	UINT meshBoneCount = curSkin->m_BoneData.size();


	for (UINT i = 0; i < meshBoneCount; ++i)
	{
		std::shared_ptr<Bone> curBone = curSkin->m_BoneData[i];

		//버텍스 버퍼 만들기위해 버텍스 내용채움
		for (int j = 0; j < curBone->vertexids.size(); j++)
		{
			int curVertexId = curBone->vertexids[j];
			float curWeight = curBone->weights[j];

			//i가 현재 본의 인덱스 
			int size = 8;

			if (buffer[curVertexId].BoneIndices[0] == 0.f ||
				buffer[curVertexId].BoneIndices[1] == 0.f ||
				buffer[curVertexId].BoneIndices[2] == 0.f ||
				buffer[curVertexId].BoneIndices[3] == 0.f ||
				buffer[curVertexId].BoneIndices[4] == 0.f ||
				buffer[curVertexId].BoneIndices[5] == 0.f ||
				buffer[curVertexId].BoneIndices[6] == 0.f ||
				buffer[curVertexId].BoneIndices[7] == 0.f
				)
			{

				for (int j = 0; j < size; j++)
				{
					if (buffer[curVertexId].BoneIndices[j] == 0.0f)
					{
						buffer[curVertexId].BoneIndices[j] = static_cast<float>(i);
						break;
					}
				}
			}


			if (buffer[curVertexId].BoneWeights[0] == 0.0f ||
				buffer[curVertexId].BoneWeights[1] == 0.0f ||
				buffer[curVertexId].BoneWeights[2] == 0.0f ||
				buffer[curVertexId].BoneWeights[3] == 0.0f ||
				buffer[curVertexId].BoneWeights[4] == 0.0f ||
				buffer[curVertexId].BoneWeights[5] == 0.0f ||
				buffer[curVertexId].BoneWeights[6] == 0.0f ||
				buffer[curVertexId].BoneWeights[7] == 0.0f
				)
			{
				for (int j = 0; j < size; j++)
				{
					if (buffer[curVertexId].BoneWeights[j] == 0.0f)
					{
						buffer[curVertexId].BoneWeights[j] = curWeight;
						break;
					}
				}
			}
		}

	}
		int a = 3;
}

std::shared_ptr<Node> ModelLoader::FindNode(std::wstring nodename, std::shared_ptr<Node> RootNode)
{
	//탐색방법도 일단 중요할듯

	if (RootNode->name == nodename)
	{
		return RootNode;
	}
	else
	{
		if (!RootNode->m_Childs.empty())
		{

			for (auto& node : RootNode->m_Childs)
			{
				std::shared_ptr<Node> temp = FindNode(nodename, node);

				if (temp != nullptr)
				{
					return temp;
				}
			}

		}

		return nullptr;
	}
}
