#include "CrossRoom.h"

void CrossRoom::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Map/Field/CrossMap.gltf"));

	m_pCollider = std::make_unique < KdCollider >();
	m_pCollider->RegisterCollisionShape("CrossModel", m_model, KdCollider::TypeGround);
}

void CrossRoom::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void CrossRoom::GenerateDepthMapFromLight()
{
}
