#include "Field.h"
#include "Room/CrossRoom.h"
#include "Room/CurveRoom.h"
#include "Room/StraightRoom.h"
#include "../../../Scene/SceneManager.h"

void Field::Init()
{
	SetNum[0] = "Set1";
	SetNum[1] = "Set2";
	SetNum[2] = "Set3";
	SetNum[3] = "Set4";
	SetNum[4] = "Set5";
	SetNum[5] = "Set6";
	SetNum[6] = "Set7";
	SetNum[7] = "Set8";
	SetNum[8] = "Set9";
	SetNum[9] = "Set10";
	SetNum[10] = "Set11";
	SetNum[11] = "Set12";
	SetNum[12] = "Set13";
	SetNum[13] = "Set14";
	SetNum[14] = "Set15";
	SetNum[15] = "Set16";

	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Map/Field/MapBase.gltf"));

	m_pCollider = std::make_unique < KdCollider >();
	m_pCollider->RegisterCollisionShape("DangeonModel", m_model, KdCollider::TypeGround);

}

void Field::Update()
{
	if (!bEntryDangeon)return;

	if (!bCreateDangeon)
	{
		CreateDangeon();
	}
	


}

void Field::PostUpdate()
{
	if (!bEntryDangeon)return;

	const KdModelWork::Node* pEntryNode;
	for (int i = 0; i < MaxSetPos; i++)
	{
		pEntryNode = m_model->FindNode(SetNum[i]);
		if (pEntryNode)
		{
			m_dangeonMat[i] = pEntryNode->m_worldTransform * m_mWorld;
		}
	}
	bSetDangeonMat = true;


}

void Field::DrawLit()
{
	if (!bEntryDangeon)return;

	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Field::GenerateDepthMapFromLight()
{
	if (!bEntryDangeon)return;
}

void Field::CreateDangeon()
{
	if (!bSetDangeonMat)return;

	std::shared_ptr<CrossRoom> cross;
	std::shared_ptr<CurveRoom> curve;
	std::shared_ptr<StraightRoom> straight;

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<int> intRand(0, 2);

	int ChusenNum = 16;

	for (int i = 0; i < ChusenNum; i++)
	{
		int rand = intRand(mt);
		switch (rand)
		{
		case 0:
			cross = std::make_shared<CrossRoom>();
			m_roomList.push_back(cross);
			break;
		case 1:
			curve = std::make_shared<CurveRoom>();
			m_roomList.push_back(curve);
			break;
		case 2:
			straight = std::make_shared<StraightRoom>();
			m_roomList.push_back(straight);
			break;
		}
	}

	auto it = m_roomList.begin();


	for (int i = 0; i < MaxSetPos; i++)
	{
		m_room = *it;
		m_room->SetMatrix(m_dangeonMat[i]);
		SceneManager::Instance().AddObject(m_room);
		if (it == m_roomList.end())
		{
			break;
		}
		it++;
	}
	
	bCreateDangeon = true;
}

void Field::ReleaseDangeon()
{
	auto it = m_roomList.begin();

	while (it != m_roomList.end())
	{
		if ((*it)->IsExpired())
		{
			it = m_roomList.erase(it);
		}
		else
		{
			++it;
		}
	}
}
