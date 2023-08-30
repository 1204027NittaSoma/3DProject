#include "BulletWeapon.h"
#include "../../Scene/SceneManager.h"

void BulletWeapon::Init()
{
	m_pos = {};

	m_speed = 0.6f;

	m_limit = 120;

	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Weapon/Bullet.gltf"));

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Bullet", m_model, KdCollider::TypeDamage);

	// デバッグ用
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void BulletWeapon::Update()
{
	
	m_pos += m_moveVec * m_speed;

	m_limit--;
	if (m_limit <= 0)
	{
		m_isExpired = true;
	}

	m_mWorld = m_ownerMat * Math::Matrix::CreateTranslation(m_pos);
}

void BulletWeapon::PostUpdate()
{
	
}

void BulletWeapon::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void BulletWeapon::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void BulletWeapon::OnHit()
{
	m_isExpired = true;
}
