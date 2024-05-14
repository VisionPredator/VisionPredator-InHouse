#include "pch.h"	

#include "Animator.h"
#include "Object.h"


Animator::Animator()
{

}

Animator::~Animator()
{

}

void Animator::Update(double dt, std::map<std::wstring, Object*>& objects)
{
	for (auto& ob : objects)
	{
		UpdateWorld(dt, ob.second);
		UpdateMatrixPallete(ob.second);
	}
}

void Animator::UpdateWorld(double dt, Object* ob)
{
	//오브젝트마다 애니메이션의 길이 시간등등이 다른데 시간을 어떻게 처리할까???
	//같은 데이터의 포인터를 사용하니까 같은 애니메이션 쓰는 애들은 시간이 두배가되서 속도가 2배 빨라져
	//같은 데이터를 쓰지만 각자가 가지고 있는 고유의 데이터야하는 아이러니?
	//이건 애니메이션 데이터는 그대로 두고 각자의 시간과 애니메이션 따로 놀면된다
	//여기서 터지면 loader에 데이터를 안읽어왔을 확률 높음
	if (ob->Animations().empty())
	{
		return;
	}

	double& time = ob->playTime;
	double speed = ob->CurAnimation(0)->m_TickFrame;
	time += dt * speed;

	if (time > ob->CurAnimation(0)->m_Duration)
	{
		time -= ob->CurAnimation(0)->m_Duration;
		//time = 0;
	}

	for (auto& ani : ob->CurAnimation(0)->m_Channels)
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
		ani->node->m_Local = total.Transpose();
		ani->node->m_LocalInverse = ani->node->m_Local.Invert();
	}

	CalcWorld(ob->RootNode());
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

void Animator::UpdateMatrixPallete(Object* ob)
{
	for (auto& mesh : ob->Meshes())
	{
		for (int i = 0; i < mesh->m_BoneData.size(); i++)
		{
			DirectX::SimpleMath::Matrix nodeworld = mesh->m_BoneData[i]->node->m_World; //glocal
			DirectX::SimpleMath::Matrix offset = mesh->m_BoneData[i]->offsetMatrix;

			mesh->Matrix_Pallete->pallete[i] = (nodeworld * offset);
		}
	}
}
