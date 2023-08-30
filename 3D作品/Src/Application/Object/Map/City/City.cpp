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

	//�����蔻�菉����
	m_pCollider = std::make_unique < KdCollider >();
	m_pCollider->RegisterCollisionShape("CityModel", m_model, KdCollider::TypeGround);

	// �f�o�b�O�p
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void City::Update()
{


	if (bSetEntry)
	{
		KdCollider::SphereInfo sphereInfo;
		// ���̒��S�ʒu��ݒ�
		sphereInfo.m_sphere.Center = m_entryPos;
		// ���̔��a��ݒ�
		sphereInfo.m_sphere.Radius = 2.0f;
		// �����蔻����������^�C�v�̐ݒ�
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
