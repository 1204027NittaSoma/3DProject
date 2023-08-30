#include "StraightRoom.h"

void StraightRoom::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Map/Field/StraightMap.gltf"));

	m_pCollider = std::make_unique < KdCollider >();
	m_pCollider->RegisterCollisionShape("StraightModel", m_model, KdCollider::TypeGround);
}

void StraightRoom::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void StraightRoom::GenerateDepthMapFromLight()
{
}
