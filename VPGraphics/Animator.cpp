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
			UpdateWorld(curData->duration, curModel);
			UpdateMatrixPallete(curData);
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
			DirectX::SimpleMath::Matrix rotation{};
			DirectX::SimpleMath::Matrix translate{};
			DirectX::SimpleMath::Matrix scale{};

			std::shared_ptr<Key> cur;
			std::shared_ptr<Key> next;

			for (auto& pos : ani->positionkey)
			{
				if (pos->time > time)
				{
					next = pos;

					float t = static_cast<float>(abs(time - (next)->time) / abs((cur)->time - (next)->time));
					DirectX::SimpleMath::Vector3 afterLerp = DirectX::SimpleMath::Vector3::Lerp((next)->value, (cur)->value, t);
					translate = DirectX::SimpleMath::Matrix::CreateTranslation(afterLerp);

					break;
				}

				cur = pos;
			}

			for (auto& scal : ani->scalingkey)
			{
				if (scal->time > time)
				{
					std::shared_ptr<Key> next = scal;

					float t = static_cast<float>(abs(time - (next)->time) / abs((cur)->time - (next)->time));
					DirectX::SimpleMath::Vector3 afterLerp = DirectX::SimpleMath::Vector3::Lerp((next)->value, (cur)->value, t);
					scale = DirectX::SimpleMath::Matrix::CreateScale(afterLerp);

					break;
				}

				cur = scal;
			}

			for (auto& rot : ani->rotationkey)
			{
				if (rot->time > time)
				{
					std::shared_ptr<Key> next = rot;


					float t = static_cast<float>(abs(time - (next)->time) / abs((cur)->time - (next)->time));
					DirectX::SimpleMath::Quaternion afterLerp = DirectX::SimpleMath::Quaternion::Slerp((next)->rotation, (cur)->rotation, t);
					rotation = DirectX::SimpleMath::Matrix::CreateFromQuaternion(afterLerp);

					break;
				}

				cur = rot;
			}


			// T R S * local 읽어올때 전치 시켜서 가져올때는 S R T가 아니다
			//XMMATRIX total = translate * rotation * scale;
			DirectX::SimpleMath::Matrix total = scale * rotation * translate;

			std::shared_ptr<Node> curAni = ani->node.lock();
			curAni->m_Local = total.Transpose();
		}
	}

	CalcWorld(curOb->m_RootNode);
}

//test
void Animator::UpdateWorld(std::weak_ptr<RenderData> ob)
{
	std::shared_ptr<RenderData> curOb = ob.lock();
	if (curOb->isChange)
	{
		std::weak_ptr<ModelData> curModel;
		std::weak_ptr<ModelData> preModel;

		curModel = m_ResourceManager.lock()->Get<ModelData>(curOb->curAnimation);
		preModel = m_ResourceManager.lock()->Get<ModelData>(curOb->preAnimation);

		//이전 애니메이션이랑 지금 애니메이션이 유효한가
		if (preModel.lock() != nullptr && curModel.lock() != nullptr)
		{
			DirectX::SimpleMath::Matrix rotation{};
			DirectX::SimpleMath::Matrix translate{};
			DirectX::SimpleMath::Matrix scale{};

			std::shared_ptr<Key> curT;
			std::shared_ptr<Key> curS;
			std::shared_ptr<Key> curR;

			std::shared_ptr<Key> preT;
			std::shared_ptr<Key> preS;
			std::shared_ptr<Key> preR;

			for (auto& ani : curModel.lock()->m_Animations[0]->m_Channels)
			{

				for (auto& pos : ani->positionkey)
				{
					if (pos->time > curOb->duration)
					{
						break;
					}

					curT = pos;
				}

				for (auto& scal : ani->scalingkey)
				{
					if (scal->time > curOb->duration)
					{
						break;
					}

					curS = scal;
				}

				for (auto& rot : ani->rotationkey)
				{
					if (rot->time > curOb->duration)
					{
						break;
					}

					curR = rot;
				}
			}

			for (auto& ani : preModel.lock()->m_Animations[0]->m_Channels)
			{

				for (auto& pos : ani->positionkey)
				{
					if (pos->time > curOb->preDuration)
					{
						break;
					}

					preT = pos;
				}

				for (auto& scal : ani->scalingkey)
				{
					if (scal->time > curOb->preDuration)
					{
						break;
					}

					preS = scal;
				}

				for (auto& rot : ani->rotationkey)
				{
					if (rot->time > curOb->preDuration)
					{
						break;
					}

					preR = rot;
				}
			}


			DirectX::SimpleMath::Vector3 afterLerpT = DirectX::SimpleMath::Vector3::Lerp(curT->value, preT->value, 0.5);
			translate = DirectX::SimpleMath::Matrix::CreateTranslation(afterLerpT);

			DirectX::SimpleMath::Vector3 afterLerpS = DirectX::SimpleMath::Vector3::Lerp(curS->value, preS->value, 0.5);
			scale = DirectX::SimpleMath::Matrix::CreateScale(afterLerpS);

			DirectX::SimpleMath::Quaternion afterLerpR = DirectX::SimpleMath::Quaternion::Lerp(curR->rotation, preR->rotation, 0.5);
			rotation = DirectX::SimpleMath::Matrix::CreateFromQuaternion(afterLerpR);

			// T R S * local 읽어올때 전치 시켜서 가져올때는 S R T가 아니다
			//XMMATRIX total = translate * rotation * scale;
			DirectX::SimpleMath::Matrix total = scale * rotation * translate;

			//std::shared_ptr<Node> curAni = ani->node.lock();
			//curAni->m_Local = total.Transpose();
		}
	}
	else
	{
		//기존에 하던대로
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

DirectX::SimpleMath::Matrix Animator::CalcMatrix(double time, std::vector<std::shared_ptr<Key>> channel)
{
	std::shared_ptr<Key> cur = channel[0];
	std::shared_ptr<Key> next;

	for (auto& key : channel)
	{
		if (key->time > time)
		{
			next = key;
			float t = static_cast<float>(abs(time - (next)->time) / abs((cur)->time - (next)->time));
			DirectX::SimpleMath::Vector3 afterLerp = DirectX::SimpleMath::Vector3::Lerp((next)->value, (cur)->value, t);

			return DirectX::SimpleMath::Matrix::CreateTranslation(afterLerp);
		}

		cur = key;
	}


}

DirectX::SimpleMath::Matrix Animator::CalcRotation(double time, std::vector<std::shared_ptr<Key>> rotationKey)
{

	std::shared_ptr<Key> cur = rotationKey[0];
	std::shared_ptr<Key> next;

	for (auto& key : rotationKey)
	{
		if (key->time > time)
		{
			next = key;
			float t = static_cast<float>(abs(time - (next)->time) / abs((cur)->time - (next)->time));
			DirectX::SimpleMath::Quaternion afterLerp = DirectX::SimpleMath::Quaternion::Slerp((next)->rotation, (cur)->rotation, t);
			return DirectX::SimpleMath::Matrix::CreateFromQuaternion(afterLerp);
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
				DirectX::SimpleMath::Matrix nodeworld = skinned->m_BoneData[i]->node.lock()->m_World; //glocal
				DirectX::SimpleMath::Matrix offset = skinned->m_BoneData[i]->offsetMatrix;

				skinned->Matrix_Pallete->offset[i] = (nodeworld * offset);
				{
					std::wstring id = std::to_wstring(curData->EntityID);
					resourcemanager->Get<ConstantBuffer<MatrixPallete>>(id).lock()->m_struct.offset[i] = (nodeworld * offset);
				}
			}
		}
	}
}
