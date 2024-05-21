#include "pch.h"	

#include "Animator.h"
#include "Object.h"


Animator::Animator()
{

}

Animator::~Animator()
{

}

void Animator::Update(double dt, std::map<std::wstring, std::pair<PassState, ModelData*>>& models)
{
	for (auto& model : models)
	{
		UpdateWorld(dt, model.second.second);
		UpdateMatrixPallete(model.second.second);
	}
}

void Animator::UpdateWorld(double dt, ModelData* ob)
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

		// T R S * local �о�ö� ��ġ ���Ѽ� �����ö��� S R T�� �ƴϴ�
		//XMMATRIX total = translate * rotation * scale;
		DirectX::SimpleMath::Matrix total = scale * rotation * translate;
		ani->node->m_Local = total.Transpose();
		ani->node->m_LocalInverse = ani->node->m_Local.Invert();
	}

	CalcWorld(ob->m_RootNode);
}

void Animator::CalcWorld(Node* RootNode)
{
	if (!RootNode->HasParents)
	{
		RootNode->m_World = RootNode->m_Local;
		RootNode->m_WorldInverse = RootNode->m_LocalInverse;
	}
	else
	{
		RootNode->m_World = RootNode->m_Parents->m_World * RootNode->m_Local;
		RootNode->m_WorldInverse = RootNode->m_World.Invert();
	}

	for (auto& node : RootNode->m_Childs)
	{
		CalcWorld(node);
	}
}

DirectX::SimpleMath::Matrix Animator::CalcMatrix(double time, std::vector<Key*> channel)
{
	auto next = std::lower_bound(channel.begin(), channel.end(), time,
		[](const Key* key, double t) { return key->time < t; });

	std::vector<Key*>::iterator cur;

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


DirectX::SimpleMath::Matrix Animator::CalcRotation(double time, std::vector<Key*> rotationKey)
{
	auto next = std::lower_bound(rotationKey.begin(), rotationKey.end(), time,
		[](const Key* key, double t) { return key->time < t; });

	std::vector<Key*>::iterator cur;

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

void Animator::UpdateMatrixPallete(ModelData* ob)
{
	for (auto& mesh : ob->m_Meshes)
	{
		if (mesh->IsSkinned())
		{
			SkinnedMesh* skinned = dynamic_cast<SkinnedMesh*>(mesh);

			for (int i = 0; i < skinned->m_BoneData.size(); i++)
			{
				DirectX::SimpleMath::Matrix nodeworld = skinned->m_BoneData[i]->node->m_World; //glocal
				DirectX::SimpleMath::Matrix offset = skinned->m_BoneData[i]->offsetMatrix;

				skinned->Matrix_Pallete->pallete[i] = (nodeworld * offset);
			}
		}
	}
}
