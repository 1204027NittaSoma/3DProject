#include "Ground.h"


void Ground::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Ground::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Ground::Init()
{
	Math::Matrix transMat;
	transMat = Math::Matrix::CreateTranslation(0, -2, 0);
	Math::Matrix scaleMat;
	scaleMat = Math::Matrix::CreateScale(5, 1, 1);
	m_mWorld = scaleMat * transMat;

	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Stage/stage.gltf"));

	m_pos = {};

	//“–‚½‚è”»’è‰Šú‰»
	m_pCollider = std::make_unique < KdCollider >();
	m_pCollider->RegisterCollisionShape("StageModel", m_model, KdCollider::TypeGround);
}
