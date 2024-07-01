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

		if (curModel != nullptr && curData->Filter == MeshFilter::Skinning)
		{
			UpdateWorld(dt, curModel);
			UpdateMatrixPallete(curModel);
		}
	}
}

void Animator::UpdateWorld(double dt, std::shared_ptr<ModelData> ob)
{
	//������Ʈ���� �ִϸ��̼��� ���� �ð������ �ٸ��� �ð��� ��� ó���ұ�???
	//���� �������� �����͸� ����ϴϱ� ���� �ִϸ��̼� ���� �ֵ��� �ð��� �ι谡�Ǽ� �ӵ��� 2�� ������
	//���� �����͸� ������ ���ڰ� ������ �ִ� ������ �����;��ϴ� ���̷���?
	//�̰� �ִϸ��̼� �����ʹ� �״�� �ΰ� ������ �ð��� �ִϸ��̼� ���� ���ȴ�
	//���⼭ ������ loader�� �����͸� ���о���� Ȯ�� ����
	if (ob->m_Animations.empty())
	{
		return;
	}

	//double& time = ob->playTime; //���� �ִϸ��̼� �÷��̽ð�
	static double time = 0; //���� �ִϸ��̼� �÷��̽ð�
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
				scale = DirectX::SimpleMath::Matrix::CreateTranslation(afterLerp);

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


		// T R S * local �о�ö� ��ġ ���Ѽ� �����ö��� S R T�� �ƴϴ�
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

				skinned->Matrix_Pallete->offset[i] = (nodeworld * offset);
			}
		}
	}
}
