#include "CurveRoom.h"

void CurveRoom::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Map/Field/CurveMap.gltf"));

	m_pCollider = std::make_unique < KdCollider >();
	m_pCollider->RegisterCollisionShape("CurveModel", m_model, KdCollider::TypeGround);
}

void CurveRoom::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void CurveRoom::GenerateDepthMapFromLight()
{

}
