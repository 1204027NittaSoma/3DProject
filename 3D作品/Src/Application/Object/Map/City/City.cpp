#include "City.h"
#include "../../../Scene/SceneManager.h"

void City::Init()
{
	Math::Matrix transMat;
	transMat = Math::Matrix::CreateTranslation(0, 0, 0);
	Math::Matrix scaleMat;
	scaleMat = Math::Matrix::CreateScale(1);
	m_mWorld = scaleMat * transMat;

	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Map/City/Map_City.gltf"));

	m_pos = {};

	//当たり判定初期化
	m_pCollider = std::make_unique < KdCollider >();
	m_pCollider->RegisterCollisionShape("CityModel", m_model, KdCollider::TypeGround);

	// デバッグ用
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void City::Update()
{


	if (bSetEntry)
	{
		KdCollider::SphereInfo sphereInfo;
		// 球の中心位置を設定
		sphereInfo.m_sphere.Center = m_entryPos;
		// 球の半径を設定
		sphereInfo.m_sphere.Radius = 2.0f;
		// 当たり判定をしたいタイプの設定
		sphereInfo.m_type = KdCollider::TypePlayer;

		Math::Vector3 hitDir = {};
		Math::Vector3 hitPos = {};
		float maxOverLap = 0;

		if (CollisionSphere(sphereInfo, SceneManager::Instance().GetObjList(), maxOverLap, hitDir, hitPos))
		{
			bEntryDangeon = true;
		}
		else
		{
			bEntryDangeon = false;
		}
	}
}

void City::PostUpdate()
{
	if (!bSetEntry)
	{
		const KdModelWork::Node* pEntryNode = m_model->FindNode("EntryPos");
		if (pEntryNode)
		{
			m_entryPos = pEntryNode->m_worldTransform.Translation();
			bSetEntry = true;
		}
	}

}

void City::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void City::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}
