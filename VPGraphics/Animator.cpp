#include "pch.h"	

#include "Animator.h"
#include "ModelData.h"
#include "Animation.h"
#include "Node.h"
#include "Mesh.h"
#include "ResourceManager.h"

Animator::Animator(std::weak_ptr<ResourceManager> manager) : m_ResourceManager(manager)
{

}

Animator::~Animator()
{

}

void Animator::Update(double dt, std::vector<std::shared_ptr<ModelData>>& models)
{
	for (auto& model : models)
	{
		UpdateWorld(dt, model);
		UpdateMatrixPallete(model);
	}
}

void Animator::Update(double dt, std::map<uint32_t, std::shared_ptr<RenderData>>& renderlist)
{

	for (auto& data : renderlist)
	{
		std::shared_ptr<RenderData> curData = data.second;
		std::shared_ptr<ModelData> curModel = m_ResourceManager.lock()->Get<ModelData>(curData->FBX).lock();

		if (curModel != nullptr)
		{
			UpdateWorld(dt, curModel);
			UpdateMatrixPallete(curModel);
		}
	}
}

void Animator::UpdateWorld(double dt, std::shared_ptr<ModelData> ob)
{
	//오브젝트마다 애니메이션의 길이 시간등등이 다른데 시간을 어떻게 처리할까???
	//같은 데이터의 포인터를 사용하니까 같은 애니메이션 쓰는 애들은 시간이 두배가되서 속도가 2배 빨라져
	//같은 데이터를 쓰지만 각자가 가지고 있는 고유의 데이터야하는 아이러니?
	//이건 애니메이션 데이터는 그대로 두고 각자의 시간과 애니메이션 따로 놀면된다
	//여기서 터지면 loader에 데이터를 안읽어왔을 확률 높음
	if (ob->m_Animations.empty())
	{
		return;
	}

	//double& time = ob->playTime; //현재 애니메이션 플레이시간
	static double time = 0; //현재 애니메이션 플레이시간
	double speed = ob->m_Animations[0]->m_TickFrame;
	//double speed = 1;
	time += dt * speed;

	if (time > ob->m_Animations[0]->m_Duration)
	{
		time -= ob->m_Animations[0]->m_Duration;
		//time = 0;
	}

	for (auto& ani : ob->m_Animations[0]->m_Channels)
	{
		DirectX::SimpleMath::Matrix rotation{};
		DirectX::SimpleMath::Matrix translate{};
		DirectX::SimpleMath::Matrix scale{};

		translate = CalcMatrix(time, ani->positionkey);
		rotation = CalcRotation(time, ani->rotationkey);
		scale = CalcMatrix(time, ani->scalingkey);

		// T R S * local 읽어올때 전치 시켜서 가져올때는 S R T가 아니다
		//XMMATRIX total = translate * rotation * scale;
		DirectX::SimpleMath::Matrix total = scale * rotation * translate;
		ani->node.lock()->m_Local = total.Transpose();
		ani->node.lock()->m_LocalInverse = ani->node.lock()->m_Local.Invert();
	}

	CalcWorld(ob->m_RootNode);
}

void Animator::CalcWorld(std::shared_ptr<Node> RootNode)
{
	if (!RootNode->HasParents)
	{
		RootNode->m_World = RootNode->m_Local;
		RootNode->m_WorldInverse = RootNode->m_LocalInverse;
	}
	else
	{
		RootNode->m_World = RootNode->m_Parents.lock()->m_World * RootNode->m_Local;
		RootNode->m_WorldInverse = RootNode->m_World.Invert();
	}

	for (auto& node : RootNode->m_Childs)
	{
		CalcWorld(node);
	}
}

DirectX::SimpleMath::Matrix Animator::CalcMatrix(double time, std::vector<std::shared_ptr<Key>> channel)
{
	auto next = std::lower_bound(channel.begin(), channel.end(), time,
		[](const std::shared_ptr<Key> key, double t) { return key->time <= t; });

	std::vector<std::shared_ptr<Key>>::iterator cur;

	if (next == channel.end())
	{
		cur = channel.begin();
	}
	else
	{
		cur = next - 1;
	}

	float t = static_cast<float>(abs(time - (*next)->time) / abs((*cur)->time - (*next)->time));
	DirectX::SimpleMath::Vector3 afterLerp = DirectX::SimpleMath::Vector3::Lerp((*next)->value, (*cur)->value, t);
	return DirectX::SimpleMath::Matrix::CreateTranslation(afterLerp);
}


DirectX::SimpleMath::Matrix Animator::CalcRotation(double time, std::vector<std::shared_ptr<Key>> rotationKey)
{
	auto next = std::lower_bound(rotationKey.begin(), rotationKey.end(), time,
		[](const std::shared_ptr<Key> key, double t) { return key->time < t; });

	std::vector<std::shared_ptr<Key>>::iterator cur;


	if (next == rotationKey.end())
	{
		cur = rotationKey.begin();
	}
	else
	{
		cur = next - 1;
	}

	float t = static_cast<float>(abs(time - (*next)->time) / abs((*cur)->time - (*next)->time));
	DirectX::SimpleMath::Quaternion afterLerp = DirectX::SimpleMath::Quaternion::Slerp((*next)->rotation, (*cur)->rotation, t);
	return DirectX::SimpleMath::Matrix::CreateFromQuaternion(afterLerp);
}

void Animator::UpdateMatrixPallete(std::shared_ptr<ModelData> ob)
{
	for (auto& mesh : ob->m_Meshes)
	{
		if (mesh->IsSkinned())
		{
			std::shared_ptr<SkinnedMesh> skinned = std::dynamic_pointer_cast<SkinnedMesh>(mesh);

			for (int i = 0; i < skinned->m_BoneData.size(); i++)
			{
				DirectX::SimpleMath::Matrix nodeworld = skinned->m_BoneData[i]->node.lock()->m_World; //glocal
				DirectX::SimpleMath::Matrix offset = skinned->m_BoneData[i]->offsetMatrix;

				skinned->Matrix_Pallete->pallete[i] = (nodeworld * offset);
			}
		}
	}
}
