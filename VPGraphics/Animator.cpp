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

void Animator::Update(double dt, std::map<uint32_t, std::shared_ptr<RenderData>>& renderlist)
{

	for (auto& data : renderlist)
	{
		std::shared_ptr<RenderData> curData = data.second;
		std::weak_ptr<ModelData> curModel;

		if (data.second->isPlay)
		{

			if (!curData->curAnimation.empty())
			{
				curModel = m_ResourceManager.lock()->Get<ModelData>(curData->curAnimation);
			}
			else
			{
				curModel = m_ResourceManager.lock()->Get<ModelData>(curData->FBX);
			}

			if (curModel.lock() != nullptr && curData->Filter == MeshFilter::Skinning)
			{
				//UpdateWorld(curData->duration, curModel);
				UpdateWorld(curData);
				UpdateMatrixPallete(curData);
			}
		}
	}
}

void Animator::UpdateWorld(double dt, std::weak_ptr<ModelData> ob)
{
	//오브젝트마다 애니메이션의 길이 시간등등이 다른데 시간을 어떻게 처리할까???
	//같은 데이터의 포인터를 사용하니까 같은 애니메이션 쓰는 애들은 시간이 두배가되서 속도가 2배 빨라져
	//같은 데이터를 쓰지만 각자가 가지고 있는 고유의 데이터야하는 아이러니?
	//이건 애니메이션 데이터는 그대로 두고 각자의 시간과 애니메이션 따로 놀면된다
	//여기서 터지면 loader에 데이터를 안읽어왔을 확률 높음

	std::shared_ptr<ModelData> curOb = ob.lock();

	if (curOb->m_Animations.empty())
	{
		return;
	}

	//double& time = ob->playTime; //현재 애니메이션 플레이시간
	double time = dt; //현재 애니메이션 플레이시간
	double speed = curOb->m_Animations[0]->m_TickFrame;
	time = dt * speed;

	if (time > curOb->m_Animations[0]->m_Duration)
	{
		time -= curOb->m_Animations[0]->m_Duration;
	}
	else
	{
		for (auto& ani : curOb->m_Animations[0]->m_Channels)
		{
			int cur = 0;
			for (int i = 0; i < ani->totals.size(); i++)
			{
				if (i >= time)
				{
					break;
				}
				cur = i;
			}

			int next = cur + 1;
			if (next >= ani->totals.size())
			{
				next = 0;
			}

			float t = time - cur;

			std::shared_ptr<Node> curAni = ani->node.lock();
			curAni->m_Local = VPMath::Matrix::Lerp(ani->totals[cur].second, ani->totals[next].second, t).Transpose();
		}
	}

	CalcWorld(curOb->m_RootNode);
}

//test
void Animator::UpdateWorld(std::weak_ptr<RenderData> ob)
{
	std::shared_ptr<RenderData> curOb = ob.lock();
	std::weak_ptr<ModelData> curModel;
	std::weak_ptr<ModelData> preModel;
	curModel = m_ResourceManager.lock()->Get<ModelData>(curOb->curAnimation);
	preModel = m_ResourceManager.lock()->Get<ModelData>(curOb->preAnimation);

	if (preModel.lock() != nullptr && curModel.lock() != nullptr)
	{
		//이전 애니메이션이랑 지금 애니메이션이 유효한가
		if (curOb->isChange)
		{

			std::map<std::wstring, VPMath::Matrix> preAni;

			for (auto& ani : preModel.lock()->m_Animations[0]->m_Channels)
			{
				int cur = 0;
				for (int i = 0; i < ani->totals.size(); i++)
				{
					if (i >= curOb->preDuration)
					{
						break;
					}
					cur = i;
				}

				preAni.insert(std::pair<std::wstring, VPMath::Matrix >(ani->nodename, ani->totals[cur].second));
			}



			for (auto& ani : curModel.lock()->m_Animations[0]->m_Channels)
			{
				int cur = 0;
				for (int i = 0; i < ani->totals.size(); i++)
				{
					if (i >= curOb->duration)
					{
						break;
					}
					cur = i;
				}

				int next = cur + 1;
				if (next >= ani->totals.size())
				{
					next = 0;
				}

				float t = curOb->duration - cur;

				std::shared_ptr<Node> curAni = ani->node.lock();
				curAni->m_Local = VPMath::Matrix::Lerp(ani->totals[cur].second, preAni[ani->nodename], t).Transpose();
			}

		}
		else
		{
			UpdateWorld(curOb->duration, curModel);

		}
	}
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

VPMath::Matrix Animator::CalcMatrix(double time, std::vector<std::shared_ptr<Key>> channel)
{
	std::shared_ptr<Key> cur = channel[0];
	std::shared_ptr<Key> next;

	for (auto& key : channel)
	{
		if (key->time > time)
		{
			next = key;
			float t = static_cast<float>(abs(time - (next)->time) / abs((cur)->time - (next)->time));
			VPMath::Vector3 afterLerp = VPMath::Vector3::Lerp((next)->value, (cur)->value, t);

			return VPMath::Matrix::CreateTranslation(afterLerp);
		}

		cur = key;
	}


}

VPMath::Matrix Animator::CalcRotation(double time, std::vector<std::shared_ptr<Key>> rotationKey)
{

	std::shared_ptr<Key> cur = rotationKey[0];
	std::shared_ptr<Key> next;

	for (auto& key : rotationKey)
	{
		if (key->time > time)
		{
			next = key;
			float t = static_cast<float>(abs(time - (next)->time) / abs((cur)->time - (next)->time));
			VPMath::Quaternion afterLerp = VPMath::Quaternion::Slerp((next)->rotation, (cur)->rotation, t);
			return VPMath::Matrix::CreateFromQuaternion(afterLerp);
		}

		cur = key;
	}


}

void Animator::UpdateMatrixPallete(std::shared_ptr<RenderData>& curData)
{
	std::shared_ptr<ResourceManager> resourcemanager = m_ResourceManager.lock();
	std::shared_ptr<ModelData> ob = resourcemanager->Get<ModelData>(curData->FBX).lock();

	for (auto& mesh : ob->m_Meshes)
	{
		if (mesh->IsSkinned())
		{
			std::shared_ptr<SkinnedMesh> skinned = std::dynamic_pointer_cast<SkinnedMesh>(mesh);

			for (int i = 0; i < skinned->m_BoneData.size(); i++)
			{
				VPMath::Matrix nodeworld = skinned->m_BoneData[i]->node.lock()->m_World; //glocal
				VPMath::Matrix offset = skinned->m_BoneData[i]->offsetMatrix;

				skinned->Matrix_Pallete->offset[i] = (nodeworld * offset);
				{
					std::wstring id = std::to_wstring(curData->EntityID);
					resourcemanager->Get<ConstantBuffer<MatrixPallete>>(id).lock()->m_struct.offset[i] = (nodeworld );
				}
			}
		}
	}


	//mixamo 기준 hips가 pelvis 이거로 상하체구분을 해보자
	std::shared_ptr<Node> pelvis = ob->m_RootNode->m_Childs[1]; //hips

	//std::shared_ptr<Node> upBody = pelvis->m_Childs[0];


}
